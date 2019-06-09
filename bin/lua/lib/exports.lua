local __g = _G


g = {
    exports = {}
}

-- setup tc.exports
setmetatable(g.exports, {
    __newindex = function(_, k, v)  -- creation reference to __g
        rawset(__g, k, v)
    end,

    __index = function(_, k)        -- access reference to __g
        return rawget(__g, k)
    end
})

-- disable to create new global variables or functions
setmetatable(__g, {
    __newindex = function(_, k, v)
        error('global variable is unallowed')
    end
})