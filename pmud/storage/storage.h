#pragma once

#include <memory>
#include <string>
#include "hiredis/hiredis.h"

namespace primordia::mud::storage {

using namespace std;

class Storage {
public:
    virtual ~Storage() {};
    // virtual bool store(const string& key, const string& value);
    // virtual bool store(const string& key, int value);
};

class RedisStorage : public Storage {
    const string& m_host;
    int m_port;
    unique_ptr<redisContext> m_connection;

  public:
    RedisStorage(const string& host, int port) : m_host(host), m_port(port) {}
    
    bool init() {
      m_connection = unique_ptr<redisContext>(redisConnect(m_host.c_str(), m_port));
      return m_connection.get() != nullptr;
    }

    // bool store(const string& key, const string& value) override {}
    // bool store(const string& key, int value) override {}

};

}