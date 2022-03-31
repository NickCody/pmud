#include <gtest/gtest.h>
#include <fmt/format.h>
#include "common/pmud_security.h"

using namespace primordia::mud::security;

TEST(HashTest, BackAndForth) {
  auto salt = generate_random_salt();
  auto credentialHash = hash_credentials("password", salt);

  ASSERT_EQ(salt, credentialHash.salt);
  ASSERT_EQ(0, credentialHash.iterations);

  auto stringified = fmt::format("{}:{}:{}", credentialHash.iterations, credentialHash.salt, credentialHash.hash);
  ASSERT_EQ(stringified, credentialHash.to_string());
}

TEST(HashTest, SamePasshraseSameSalt) {
  auto salt = generate_random_salt();
  auto credentialHash1 = hash_credentials("password", salt);
  auto credentialHash2 = hash_credentials("password", salt);

  ASSERT_EQ(credentialHash1.hash, credentialHash2.hash);
}

TEST(HashTest, SamePasshraseDifferentSalt) {
  auto salt1 = generate_random_salt();
  auto salt2 = generate_random_salt();
  auto credentialHash1 = hash_credentials("password", salt1);
  auto credentialHash2 = hash_credentials("password", salt2);

  ASSERT_NE(credentialHash1.hash, credentialHash2.hash);
}
