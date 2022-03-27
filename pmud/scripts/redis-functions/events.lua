
local function EventUserCreate(keys, args)
  local stream_name = keys[1]
  assert(stream_name == "events")
  assert(args[1] == 'username')

  return redis.call('XADD', stream_name, "*", "event", "EventUserCreate", args[1], args[2])
end

local function EventUserLogin(keys, args)
  local stream_name = keys[1]
  assert(stream_name == "events")
  assert(args[1] == 'username')

  return redis.call('XADD', stream_name, "*",  "event", "EventUserLogin", args[1], args[2])
end

local function EventUserLogout(keys, args)
  local stream_name = keys[1]
  assert(stream_name == "events")
  assert(args[1] == 'username')

  return redis.call('XADD', stream_name, "*", "event", "EventUserLogout", args[1], args[2])
end

redis.register_function('EventUserCreate', EventUserCreate)
redis.register_function('EventUserLogin', EventUserLogin)
redis.register_function('EventUserLogout', EventUserLogout)
