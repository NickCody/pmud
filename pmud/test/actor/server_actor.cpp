#include <algorithm>
#include <caf/exit_reason.hpp>
#include <chrono>
#include <memory>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <caf/all.hpp>
#define CAF_TEST_NO_MAIN
#include <caf/test/unit_test_impl.hpp>
#include <caf/test/dsl.hpp>

#include "storage/storage_actor.h"
#include "mocks/mock_storage.h"
#include "pnet/server_actor.h"
#include "pmud/test/mocks/mock_storage.h"
#include "event/event_recorder_actor.h"
#include "system/mud_config.h"

#include "common/global_type_id.h"

#include <zmq.hpp>

using namespace std;
using namespace caf;

namespace storage = primordia::mud::storage;
namespace mocks = primordia::mud::test::mocks;
namespace common = primordia::mud::common;
namespace event = primordia::mud::event;
namespace pnet = primordia::mud::pnet;

const auto TEST_TIMEOUT = chrono::seconds(5);

int main(int argc, char** argv) {
  using namespace caf;
  init_global_meta_objects<caf::id_block::pmud_caf_types>();
  core::init_global_meta_objects();
  return test::main(argc, argv);
}

struct basic_functionality {
  caf::actor_system_config cfg;
  caf::actor_system sys;
  caf::scoped_actor self;
  actor storage_actor;
  actor event_recorder_actor;
  primordia::mud::system::MudConfig mud_config{ "pmud", "0.0.0.0", 49201, 3, "console", "debug", "", true };

  primordia::mud::system::MudSystemPtr mud_system;

  basic_functionality()
      : sys(cfg),
        self(sys) {

    spdlog::set_level(spdlog::level::level_enum::debug);

    unique_ptr<storage::Storage> mock_storage = std::make_unique<mocks::MockStorage>();
    storage_actor = sys.spawn<storage::StorageActor>(std::move(mock_storage));
    event_recorder_actor = sys.spawn<event::EventRecorderActor>(actor_cast<strong_actor_ptr>(storage_actor));
    mud_system = make_shared<primordia::mud::system::MudSystem>(
        sys, mud_config, actor_cast<strong_actor_ptr>(storage_actor), actor_cast<strong_actor_ptr>(event_recorder_actor));
  }

  ~basic_functionality() {
    anon_send_exit(storage_actor, exit_reason::user_shutdown);
  }
};

CAF_TEST_FIXTURE_SCOPE(storage_tests, basic_functionality)

CAF_TEST(server creation) {
  bool server_success;
  actor server_actor = sys.spawn<pnet::ServerActor>(mud_system);
  self->request(server_actor, TEST_TIMEOUT, StartServer())
      .receive([&](int status) { server_success = (status == 0); },
               [&](const error& err) {
                 server_success = false;
                 spdlog::info("Error: {}", to_string(err));
               });

  CAF_CHECK(server_success);

  zmq::context_t context{ 1 };

  zmq::socket_t socket{ context, zmq::socket_type::stream };
  socket.connect("tcp://localhost:49201");

  zmq::message_t reply{};
  zmq::recv_result_t result = socket.recv(reply, zmq::recv_flags::none);

  CAF_CHECK(result.has_value());

  fmt::print("Got reply from server 1: {}\n", reply.to_string());
  fmt::print("Got reply from server (value): {}\n", result.value());

  // TODO: Nick, write a dedicated client

  // const std::string data{ "Hello" };
  // socket.send(zmq::buffer(data), zmq::send_flags::none);

  self->request(server_actor, TEST_TIMEOUT, GoodbyeServer())
      .receive([&](bool status) { server_success = status; },
               [&](const error& err) {
                 server_success = false;
                 spdlog::info("Error: {}", to_string(err));
               });
  CAF_CHECK(server_success);

  self->send_exit(event_recorder_actor, caf::exit_reason::user_shutdown);
}

CAF_TEST_FIXTURE_SCOPE_END()