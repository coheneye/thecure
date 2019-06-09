package.path = './lua/?.lua;./lua/lib/?.lua;' .. package.path

-- print(_VERSION)

config      = require "config"
local _     = require "exports"
local tbx   = require "tablex"

-- test pb
local pb = require "pb"
-- function make_readonly(tbl)
--     local pxy = {}
--     setmetatable(pxy, {
--         __index = tbl,
--         __newindex = function(_, k, v)
--             error('update a readonly table. k = ' .. tostring(k))
--         end
--     })
--     return pxy
-- end

g.exports.test = function()
    -- log.debug('debug')
    -- log.error('error')
    -- log.critical('cri')
end


local timer = tc.timer(tc.hub)
timer:start(5000, 1000, function(t)
    t:stop()
end, timer)

-- tbx.foreach(tc.redis, function(v, k)
--     print(k, v)
-- end)


-- local rds = tc.redis
-- print(type(rds))
-- rds:exec('keys *')

-- local rds = tc.redis(tc.hub)
-- print(rds:connect('127.0.0.1', 6379))
-- rds:exec('keys *', function(res)
--     print(res:type());
-- end)
-- rds:close()
-- local task = tc.task(tc.hub)

-- local worker = function(t)
--     log.debug('in worker')
--     t:send({p=20, t='aaa'})
-- end

-- local progress = function(data)
--     log.debug(string.format('p=%d, t=%s', data.p, data.t))
-- end

-- local done = function(status)
--     log.debug('done')
-- end
-- task:run(worker, task, progress, done)

-- g.exports.a = 23
-- a = 24
-- print(a)

test()
