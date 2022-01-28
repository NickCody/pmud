#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <queue>
#include <memory>
#include <cstdlib>
#include "include/fmt/format.h"

#include "caf/all.hpp"

#include "common/stopwatch.h"

auto g_busy_spin_millis = std::chrono::microseconds(0);
constexpr auto RESULT_WIDTH = 14;
constexpr auto NANOS_PER_MICRO = 1000;

uint64_t g_actor_work_count = 0;
uint64_t g_actor_x_work_count = 0;
uint64_t g_actor_pool_work_count = 0;
uint64_t g_par_work_count = 0;
uint64_t g_func_work_count = 0;

uint64_t g_actor_send_count = 0;
uint64_t g_actor_x_send_count = 0;
uint64_t g_actor_pool_send_count = 0;
uint64_t g_par_send_count = 0;
uint64_t g_func_send_count = 0;

using namespace caf;
using namespace std;

mutex g_mutex;
queue<uint64_t> g_queue;
queue<uint64_t> g_count_queue;
atomic<bool> g_par_continue;
volatile bool g_random_work;

struct WorkConfig {
  size_t par_threads;
  size_t actor_threads;
  size_t actor_pool_size;
  size_t iterations;
  std::chrono::microseconds work_send_delay;
};

// =-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// busy spin

void busy_spin(std::chrono::microseconds spin_time) {
  const auto start = std::chrono::system_clock::now();

  std::chrono::microseconds actual_spin_time;
  if (g_random_work) {
    actual_spin_time = std::chrono::microseconds(rand() % (int)spin_time.count());
  } else {
    actual_spin_time = spin_time;
  }
  while (true) {
    const auto now = std::chrono::system_clock::now();
    if ((now - start) >= actual_spin_time)
      break;
  }
}

void print_queue_analysis(uint64_t num_threads) {
  std::vector<uint64_t> queue_analysis;
  queue_analysis.reserve(num_threads);
  for (uint64_t i = 0; i < num_threads; i++) {
    queue_analysis[i] = 0;
  }
  while (!g_count_queue.empty()) {
    queue_analysis[g_count_queue.front()]++;
    g_count_queue.pop();
  }

  for (uint64_t i = 0; i < num_threads; i++) {
    fmt::print("queue_analysis[{}] = {}\n", i, queue_analysis[i]);
  }
}

void print_counts() {
  fmt::print(" {:>30} = {:>15}\n", "g_par_work_count", common::numberFormatWithCommas(g_par_work_count));
  fmt::print(" {:>30} = {:>15}\n", "g_par_send_count", common::numberFormatWithCommas(g_par_send_count));

  fmt::print(" {:>30} = {:>15}\n", "g_actor_pool_work_count", common::numberFormatWithCommas(g_actor_pool_work_count));
  fmt::print(" {:>30} = {:>15}\n", "g_actor_pool_send_count", common::numberFormatWithCommas(g_actor_pool_send_count));
}

// =-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Actor setup
//

struct Foo {
  uint64_t bar;
};

struct CounterData {
  std::shared_ptr<uint64_t> count;
};

CAF_BEGIN_TYPE_ID_BLOCK(class_perf, first_custom_type_id)

CAF_ADD_TYPE_ID(class_perf, (Foo))

CAF_ADD_ATOM(class_perf, work)
CAF_ADD_ATOM(class_perf, report)

CAF_END_TYPE_ID_BLOCK(class_perf)

template <class Inspector> bool inspect(Inspector& f, Foo& x) {
  return f.object(x).fields(f.field("bar", x.bar));
}

behavior posh_pool(stateful_actor<Foo>* self, const actor& count_actor) {
  return {
    [=](work) {
      self->send(count_actor, work_v);
      busy_spin(g_busy_spin_millis);
    },
  };
}

behavior posh_counter(stateful_actor<CounterData>* self) {
  self->state.count = std::make_shared<uint64_t>(0);
  return {
    [=](work) { (*self->state.count)++; },
    [=](report) -> uint64_t { return *self->state.count; },
  };
}

void par_func(int max_to_read, uint64_t id) {
  while (true) {
    int num_read = 0;
    {
      std::lock_guard<std::mutex> guard(g_mutex);
      if (!g_par_continue && g_queue.empty())
        return;

      while (!g_queue.empty() && (max_to_read == -1 || num_read < max_to_read)) {
        g_count_queue.push(id);
        g_queue.pop();
        num_read++;
      }
    }
    for (int i = 0; i < num_read; i++) {
      busy_spin(g_busy_spin_millis);
    }
  }
}

std::chrono::microseconds time_actor_pool(actor_system& sys, const WorkConfig& work_config) {
  common::Stopwatch<uint64_t, std::chrono::microseconds> watch;
  g_actor_pool_send_count = 0;
  g_actor_pool_work_count = 0;

  double micros = watch.duration([&] {
    scoped_actor self{ sys };
    auto counter = sys.spawn(posh_counter);
    scoped_execution_unit ctx{ &sys };
    auto pool = actor_pool::make(
        &ctx, work_config.actor_pool_size, [&] { return sys.spawn(posh_pool, counter); }, actor_pool::round_robin());

    for (uint64_t i = 0; i < work_config.iterations; i++) {
      g_actor_pool_send_count++;
      self->send(pool, work_v);
      std::this_thread::sleep_for(work_config.work_send_delay);
    }

    while (g_actor_pool_work_count < g_actor_pool_send_count) {
      self->request(counter, infinite, report_v)
          .receive([=](uint64_t c) { g_actor_pool_work_count = c; }, [&](error& err) { fmt::print("Error: {}\n", to_string(err)); });
    }

    self->send_exit(counter, exit_reason::user_shutdown);
    self->send_exit(pool, exit_reason::user_shutdown);
    self->await_all_other_actors_done();
  });

  sys.await_all_actors_done();

  return std::chrono::microseconds((uint64_t)micros);
}

// -==------=-==-=-=-===-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-===-=-=-==
// time_par_func
// -==------=-==-=-=-===-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-===-=-=-==

std::chrono::microseconds time_par_func(const WorkConfig& work_config) {
  common::Stopwatch<uint64_t, std::chrono::microseconds> watch;
  g_par_continue.store(true);
  int num_to_read = work_config.par_threads == 1 ? -1 : 1;
  g_par_send_count = 0;
  g_par_work_count = 0;

  while (!g_queue.empty())
    g_queue.pop();

  while (!g_count_queue.empty())
    g_count_queue.pop();

  double micros = watch.duration([&] {
    // prime queue
    for (uint64_t i = 0; i < work_config.iterations; i++) {
      g_queue.push(g_par_send_count);
      g_par_send_count++;
      std::this_thread::sleep_for(work_config.work_send_delay);
    }

    // create threads
    std::vector<std::thread> thread_par_func;
    for (uint64_t i = 0; i < work_config.par_threads; i++) {
      thread_par_func.push_back(std::thread(par_func, num_to_read, i));
    }

    // teardown
    g_par_continue.store(false);
    for (std::thread& thread : thread_par_func) {
      if (thread.joinable())
        thread.join();
    }
  });

  g_par_work_count = g_count_queue.size();

  return std::chrono::microseconds((uint64_t)micros);
}

void print_config_row(const WorkConfig& work_config) {
  fmt::print("{},{},{},{},{},{}",
             work_config.actor_threads,
             work_config.iterations,
             g_random_work,
             work_config.par_threads,
             work_config.actor_pool_size,
             g_busy_spin_millis.count());
}

void harness(actor_system& sys, const WorkConfig& work_config) {
  std::chrono::microseconds par_time = time_par_func(work_config);
  std::chrono::microseconds actor_time = time_actor_pool(sys, work_config);

  print_config_row(work_config);
  fmt::print(",{},{}\n", "parallel function", par_time.count());
  print_config_row(work_config);
  fmt::print(",{},{}\n", "actor pool", actor_time.count());
}

void caf_main(actor_system& sys) {
  list<size_t> par_threads = get_or(sys.config(), "class-perf.par-threads", list<size_t>((size_t)1));
  list<size_t> actor_pool_size = get_or(sys.config(), "class-perf.actor-pool-size", list<size_t>((size_t)1));
  list<caf::timespan> work_per_iteration =
      get_or(sys.config(), "class-perf.work-per-iteration", std::list<caf::timespan>({ caf::timespan(NANOS_PER_MICRO * 10) }));
  g_random_work = get_or(sys.config(), "class-perf.random-work", false);

  fmt::print("{},{},{},{},{},{},{},{}\n", "actor_threads", "iterations", "random", "par_threads", "pool_size", "busy_spin", "method", "result");

  for (auto par_threads : par_threads) {
    for (auto actor_pool : actor_pool_size) {
      for (auto work : work_per_iteration) {
        g_busy_spin_millis = std::chrono::duration_cast<std::chrono::microseconds>(work);
        WorkConfig work_config = {
          par_threads,
          get_or(sys.config(), "caf.scheduler.max-threads", (size_t)1),
          actor_pool,
          get_or(sys.config(), "class-perf.iterations", (size_t)100),
          std::chrono::duration_cast<std::chrono::microseconds>(get_or(sys.config(), "class-perf.work-send-delay", caf::timespan(NANOS_PER_MICRO))),
        };
        harness(sys, work_config);
      }
    }
  }
}

CAF_MAIN(id_block::class_perf)