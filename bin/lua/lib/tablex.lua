-- @file = tablex.lua 
-- v0.1
-- date: 2019-05-21

-- 过滤表 @t, 
-- bool @fn(value, index)
-- @return: new table
-- 使每一个 t[i] 都满足 fn(t[i]) == true
local function filter(t, fn)
    local res = {}
    for k, v in pairs(t) do
        if fn(v, k) then
            table.insert(res, t[k])
        end
    end
    return res
end


local function swap(t, x, y)
    -- 不检查 nil, 交换为 nil 合理的
    local temp = t[x]
    t[x] = t[y]
    t[y] = temp
end


local function values(tbl)
    if not tbl then return nil end
    if not next(tbl) then return {} end
    local rtv = {}
    for _, v in pairs(tbl) do table.insert(rtv, v) end
    return rtv
end


-- @return 由 tbl 的 key 组成的一维数组
local function keys(tbl)
    if not tbl then
        return nil
    end
    if not next(tbl) then
        return {}
    end
    local rtv = {}      -- return value
    for k, _ in pairs(tbl) do
        table.insert(rtv, k)
    end
    return rtv
end


-- 对一维 number list 求和 
local function sum(tbl)
    local sum = 0
    for _, v in ipairs(tbl) do
        sum = sum + v
    end
    return sum
end


-- 打乱table
local function shuffle(tbl, rand)
    rand = rand or math.random
    if #tbl <= 1 then
        return
    end
    for i = #tbl, 2, -1 do
        local j = rand(1, i - 1)
        print(i, j)
        tbl[i], tbl[j] = tbl[j], tbl[i]
    end
end


-- @return 由 tbl 的 key 组成，每个 key 满足 func(key) 的数组
-- 如果 func 为 nil, 返回所有 key，等同于 tablex:keys(tbl)
local function keys_if(tbl, func)
    if not tbl then return nil end
    if not next(tbl) then return {} end
    local rtv = {}
    local function __nope(k) return true end
    func = func or __nope
    for k, _ in pairs(tbl) do
        if func(k) then table.insert(rtv, k) end
    end
    return rtv
end


local function foreach(tbl, fn)
    for k, v in pairs(tbl) do
        fn(k, v)
    end
end


return {
    filter = filter,
    foreach = foreach,
}