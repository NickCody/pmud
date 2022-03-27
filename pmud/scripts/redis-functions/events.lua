
local function eventUserCreate(keys, args)
  local stream_name = keys[1]
  assert(stream_name == "events")
  assert(args[1] == 'username')
  assert(args[3] == 'password')

  return redis.call('XADD', stream_name, "*", "event", "EventUserCreate", args[1], args[2], args[3], args[4])
end

local function eventUserLogin(keys, args)
  local stream_name = keys[1]
  assert(stream_name == "events")
  assert(args[1] == 'username')

  return redis.call('XADD', stream_name, "*",  "event", "EventUserLogin", args[1], args[2])
end

local function eventUserLogout(keys, args)
  local stream_name = keys[1]
  assert(stream_name == "events")
  assert(args[1] == 'username')

  return redis.call('XADD', stream_name, "*", "event", "EventUserLogout", args[1], args[2])
end

redis.register_function('eventUserCreate', eventUserCreate)
redis.register_function('eventUserLogin', eventUserLogin)
redis.register_function('eventUserLogout', eventUserLogout)
