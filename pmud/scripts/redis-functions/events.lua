

local function EventUserCreate(keys, args)
  local stream_name = keys[1]
  assert(stream_name == "events")

  return redis.call('XADD', stream_name, "*", "event", "EventUserCreate", args[1], args[2], args[3], args[4])
end

local function EventUserLogin(keys, args)
  local stream_name = keys[1]
  assert(stream_name == "events")

  return redis.call('XADD', stream_name, "*",  "event", "EventUserLogin", args[1], args[2])
end

local function EventUserLogout(keys, args)
  local stream_name = keys[1]
  assert(stream_name == "events")

  return redis.call('XADD', stream_name, "*", "event", "EventUserLogout", args[1], args[2])
end

redis.register_function('EventUserCreate', EventUserCreate)
redis.register_function('EventUserLogin', EventUserLogin)
redis.register_function('EventUserLogout', EventUserLogout)
