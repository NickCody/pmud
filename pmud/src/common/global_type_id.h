/**
 *  global_type_id.h
 *
 *  Created on Thu Mar 24 2022
 */

#pragma once

#include <caf/all.hpp>
#include <caf/allowed_unsafe_message_type.hpp>
#include <caf/optional.hpp>
#include <map>
#include <string>

using StreamRecordFields_t = std::map<std::string, std::string>;

CAF_BEGIN_TYPE_ID_BLOCK(pmud_caf_types, caf::first_custom_type_id)

// Server
CAF_ADD_ATOM(pmud_caf_types, AcceptConnection)
CAF_ADD_ATOM(pmud_caf_types, StartServer)
CAF_ADD_ATOM(pmud_caf_types, GoodbyeServer)

// Connection
CAF_ADD_ATOM(pmud_caf_types, FromUserGetInput)
CAF_ADD_ATOM(pmud_caf_types, GoodbyeConnection)

// UserClient (Command + Connection)
CAF_ADD_ATOM(pmud_caf_types, ToUserPrompt)
CAF_ADD_ATOM(pmud_caf_types, ToUserEmit)

// Command / Controllers
CAF_ADD_ATOM(pmud_caf_types, PerformWelcome)
CAF_ADD_ATOM(pmud_caf_types, OnUserInput)

// Storage
//
CAF_ADD_TYPE_ID(pmud_caf_types, (StreamRecordFields_t))
CAF_ADD_ATOM(pmud_caf_types, StorageNoop)
CAF_ADD_ATOM(pmud_caf_types, StorageValueStore)
CAF_ADD_ATOM(pmud_caf_types, StorageValueGet)
CAF_ADD_ATOM(pmud_caf_types, StorageMapStore)
CAF_ADD_ATOM(pmud_caf_types, StorageMapGet)
CAF_ADD_ATOM(pmud_caf_types, StorageListStore)
CAF_ADD_ATOM(pmud_caf_types, StorageListGet)
CAF_ADD_ATOM(pmud_caf_types, StorageSetStore)
CAF_ADD_ATOM(pmud_caf_types, StorageSetGet)
CAF_ADD_ATOM(pmud_caf_types, StorageStreamStore)
CAF_ADD_ATOM(pmud_caf_types, StorageEventStore)

// Events
//
CAF_ADD_ATOM(pmud_caf_types, EventUserCreate)
CAF_ADD_ATOM(pmud_caf_types, EventUserLogin)
CAF_ADD_ATOM(pmud_caf_types, EventUserLogout)

// Player
//
CAF_ADD_ATOM(pmud_caf_types, LoginControllerStart)
CAF_ADD_ATOM(pmud_caf_types, LoginControllerEnd)

CAF_END_TYPE_ID_BLOCK(pmud_caf_types)

CAF_ALLOW_UNSAFE_MESSAGE_TYPE(StreamRecordFields_t)
CAF_ALLOW_UNSAFE_MESSAGE_TYPE(caf::optional<std::string>)
