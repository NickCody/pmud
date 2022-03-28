#pragma once

#include <string>
#include <fmt/format.h>
#include "common/hash-library/sha256.h"

namespace primordia::mud::security {

  using namespace std;
  using namespace fmt;

  struct CredentialHash {
    size_t iterations;
    string salt;
    string hash;

    string to_string() {
      return format("{}:{}:{}", iterations, salt, hash);
    }
  };

  CredentialHash hash_credentials(const string& passphrase) {
    SHA256 sha256;
    CredentialHash hash{ 0, "", sha256(passphrase) };
    return hash;
  }

} // namespace primordia::mud::security