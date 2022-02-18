#pragma once

namespace primordia::mud::storage {

  class PlayerStorage {
  public:
    PlayerStorage(StoragePtr storage)
        : m_storage(storage) {}
  }

} // namespace primordia::mud::storage