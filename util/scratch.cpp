#include <optional>

struct Bar {
  int foo;
};

struct ModuleId {
  int id;
  int bits;
};

struct Partition {
  int num;
};

std::optional<int> getFoo() const {
  return Bar{ ModuleId{ 10, Partition{ 0 } }, *wise_enum::from_string<RedundantInstance>(m_config.instance) };
}

int main(int argc, char** argv) {
}