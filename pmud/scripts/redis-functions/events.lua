local function eventUserLogin(keys, args)
  local stream_name = keys[1]
  assert(args[1] == 'username')
  assert(args[3] == 'password')

  return redis.call('XADD', stream_name, "*", args[1], args[2], args[3], args[4])
end

redis.register_function('eventUserLogin', eventUserLogin)