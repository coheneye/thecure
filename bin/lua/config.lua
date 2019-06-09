
local config = {
    zk = {
        {'192.168.0.199', 2222, 'password'},    
    },
    redis = {   -- if redis configure shows up here, then the redis configure in zk will be overwritten.
        {'192.168.0.199', 6379, 'password'},
        {'192.168.0.163', 6379, 'password'},
    },
}

return config