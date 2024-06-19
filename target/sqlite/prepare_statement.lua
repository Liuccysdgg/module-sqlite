local result = require("sqlite.result")
local sqlite_prepare_statement = {}
sqlite_prepare_statement.__index = sqlite_prepare_statement

--[[
    创建一个新的 fw_sqlite_prepare_statement 对象
    @return 返回一个新的 fw_sqlite_prepare_statement 对象
]]
function sqlite_prepare_statement.new(__module)
    local instance = setmetatable({}, sqlite_prepare_statement)
    instance.module = __module
    return instance
end

--[[
    设置 bigint 类型的参数
    @param index 参数索引
    @param value 参数值
]]
function sqlite_prepare_statement:set_bigint(index, value)
    self.module:set_bigint(index, value)
end

--[[
    设置 double 类型的参数
    @param index 参数索引
    @param value 参数值
]]
function sqlite_prepare_statement:set_dob(index, value)
    self.module:set_dob(index, value)
end

--[[
    设置 int32 类型的参数
    @param index 参数索引
    @param value 参数值
]]
function sqlite_prepare_statement:set_i32(index, value)
    self.module:set_i32(index, value)
end

--[[
    设置 int64 类型的参数
    @param index 参数索引
    @param value 参数值
]]
function sqlite_prepare_statement:set_i64(index, value)
    self.module:set_i64(index, value)
end

--[[
    设置 null 类型的参数
    @param index 参数索引
]]
function sqlite_prepare_statement:set_null(index)
    self.module:set_null(index)
end

--[[
    设置字符串类型的参数
    @param index 参数索引
    @param value 参数值
]]
function sqlite_prepare_statement:set_str(index, value)
    self.module:set_str(index, value)
end

--[[
    清空所有参数
]]
function sqlite_prepare_statement:clear()
    self.module:clear()
end

--[[
    执行 update 操作
    @return 返回受影响的行数
]]
function sqlite_prepare_statement:update()
    return self.module:update()
end

--[[
    执行 query 操作
    @return 返回 sqlite_result 对象
]]
function sqlite_prepare_statement:query()
    return result.new(self.module:query())
end

return sqlite_prepare_statement
