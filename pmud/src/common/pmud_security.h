#pragma once

#include <string>
#include <fmt/format.h>
#include <random>
#include <sstream>

#include "common/hash-library/sha256.h"

namespace primordia::mud::security {

  using namespace std;
  using namespace fmt;

  struct CredentialHash {
    size_t iterations; // NOTE: Not used right now
    string salt;
    string hash;

    string to_string() {
      return format("{}:{}:{}", iterations, salt, hash);
    }
  };

  string generate_random_salt() {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<uint16_t> dist(0, 15);
    char hex[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    stringstream salt;
    for (size_t i = 0; i < 8; i++) {
      salt << hex[dist(rng)];
    }

    return salt.str();
  }

  CredentialHash hash_credentials(const string& passphrase, const string& salt) {
    SHA256 sha256;
    string passphrase_salt = format("{}:{}", passphrase, salt);
    CredentialHash hash{ 0, salt, sha256(passphrase_salt) };
    return hash;
  }

} // namespace primordia::mud::security
