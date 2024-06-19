local ppst = require("sqlite.prepare_statement")
local sqlite_db = {}
sqlite_db.__index = sqlite_db

--[[
    创建一个新的 fw_sqlite_db 对象
    @return 返回一个新的 fw_sqlite_db 对象
]]
function sqlite_db.new()
    local instance = setmetatable({}, sqlite_db)
    instance.module = fw_sqlite_db.new()
    return instance
end

--[[
    打开 SQLite 数据库文件
    @param database_filepath 数据库文件路径
    @return 是否成功打开
]]
function sqlite_db:open(database_filepath)
    return self.module:open(database_filepath)
end

--[[
    清空数据库连接
]]
function sqlite_db:clear()
    self.module:clear()
end

--[[
    设置 SQL 语句
    @param sql SQL 语句
    @return 返回 sqlite_prepare_statement 对象
]]
function sqlite_db:setsql(sql)
    return ppst.new(self.module:setsql(sql))
end

--[[
    获取上一次插入操作的 ID
    @return 返回上一次插入操作的 ID
]]
function sqlite_db:insert_id()
    return self.module:insert_id()
end

--[[
    开始事务
    @param autocommit 是否自动提交
]]
function sqlite_db:begin(autocommit)
    self.module:begin(autocommit or false)
end

--[[
    提交事务
]]
function sqlite_db:commit()
    self.module:commit()
end

--[[
    回滚事务
]]
function sqlite_db:rollback()
    self.module:rollback()
end

--[[
    获取最后的错误信息
    @return 返回错误信息字符串
]]
function sqlite_db:last_error()
    return self.module:last_error()
end

function sqlite_db:self()
    return self.module:self()
end

return sqlite_db
