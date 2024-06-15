/*Software License

Copyright(C) 2024[liuyingjie]
License Terms
Usage Rights

Any individual or entity is free to use, copy, and distribute the binary form of this software without modification to the source code, without the need to disclose the source code.
If the source code is modified, the modifications must be open - sourced under the same license.This means that the modifications must be disclosed and accompanied by a copy of this license.
Future Versions Updates
From this version onwards, all future releases will be governed by the terms of the latest version of the license.This license will automatically be nullified and replaced by the new version.
Users must comply with the terms of the new license issued in future releases.
Liability and Disclaimer
This software is provided “as is”, without any express or implied warranties, including but not limited to the warranties of merchantability, fitness for a particular purpose, and non - infringement.In no event shall the author or copyright holder be liable for any claims, damages, or other liabilities, whether in an action of contract, tort, or otherwise, arising from, out of, or in connection with the software or the use or other dealings in the software.
Contact Information
If you have any questions, please contact us: 1585346868@qq.com Or visit our website fwlua.com.
*/
#include "sqlite.h"
#include "util/time.h"
#include "dll_interface.h"

extern "C" {
#ifdef _WIN32
    DLL_EXPORT
#endif
        int fastweb_module_regist(void* sol2, void* lua)
    {
        sol::state* state = static_cast<sol::state*>(sol2);
        module::sqlite_regist(state);
        return 0;
    }
}



void module::sqlite_regist(sol::state* lua)
{
    module::sqlite_result::regist(lua);
    module::sqlite_db::regist(lua);
    module::sqlite_prepare_statement::regist(lua);

}

module::sqlite_result::sqlite_result(ylib::sqlite::result* result) :m_result(result)
{

}

module::sqlite_result::~sqlite_result()
{

}
uint32 module::sqlite_result::field_count()
{
    return m_result->field_count();
}

std::string module::sqlite_result::field_type(sol::object obj)
{
    if (m_result == nullptr)
        throw ylib::exception("result is null");
    if (obj.is<int>() || obj.is<double>()) {
        m_result->field_type((uint32)obj.as<int>());
    }
    else if (obj.is<std::string>()) {
        m_result->field_type((std::string)obj.as<std::string>());
    }
    return "";
}
std::string module::sqlite_result::field_name(uint32 index)
{
    if (m_result == nullptr)
        throw ylib::exception("result is null");
    return m_result->field_name(index);
}
bool module::sqlite_result::next()
{
    if (m_result == nullptr)
        throw ylib::exception("result is null");
    return m_result->next();
}

sol::object module::sqlite_result::get(sol::object obj, sol::this_state s)
{
    if (m_result == nullptr)
        throw ylib::exception("result is null");
    std::string type;
    if (obj.get_type() == sol::type::string)
        type = m_result->field_type(obj.as<std::string>());
    else if (obj.get_type() == sol::type::number)
        type = m_result->field_type(obj.as<int>());

#define GET_VALUE(FUNCTION)                                                                                             \
    if (obj.get_type() == sol::type::string)                                                                                    \
        return sol::make_object(s, m_result->FUNCTION(obj.as<std::string>()));                          \
    else if (obj.get_type() == sol::type::number)                                                                           \
        return sol::make_object(s, m_result->FUNCTION(obj.as<uint32>()))

    if (type == "int" || type == "tinyint")
    {
        GET_VALUE(get_int32);
    }

    else if (type == "text")
    {
        GET_VALUE(get_string);
    }
    return sol::make_object(s, sol::nil);

}

sol::table module::sqlite_result::table(sol::this_state s)
{
    if (m_result == nullptr)
        throw ylib::exception("result is null");
    sol::state_view lua(s);
    sol::table result_table = lua.create_table();
    int row_num = 1;

    while (next()) {
        sol::table row = lua.create_table();
        for (uint32_t i = 0; i < field_count(); ++i) {
            row[field_name(i + 1)] = get(sol::make_object(s, i + 1), s);
        }
        result_table[row_num++] = row;
    }
    return result_table;
}

void module::sqlite_result::regist(sol::state* lua)
{
    lua->new_usertype<module::sqlite_result>("sqlite_result",
        "field_name", &module::sqlite_result::field_name,
        "get", &module::sqlite_result::get,
        "next", &module::sqlite_result::next,
        "table", &module::sqlite_result::table,
        "field_count", &module::sqlite_result::field_count,
        "field_type", &module::sqlite_result::field_type
    );
}

module::sqlite_db::sqlite_db()
{
    m_conn = std::make_unique<ylib::sqlite::conn>();
}

module::sqlite_db::~sqlite_db()
{
        m_conn = nullptr;
}

bool module::sqlite_db::open(const std::string& database_filepath)
{
    ylib::sqlite::sqlite_conn_info info;
    info.database = database_filepath;
    return m_conn->start(info);
}


void module::sqlite_db::clear()
{
    m_conn->clear();
}

std::shared_ptr<module::sqlite_prepare_statement> module::sqlite_db::setsql(const std::string& sql)
{
    auto ppst = m_conn->setsql(sql);
    if (ppst == nullptr)
        return nullptr;
    return std::make_shared<module::sqlite_prepare_statement>(ppst);
}

uint64 module::sqlite_db::insert_id()
{
    return m_conn->insert_id();
}

void module::sqlite_db::begin(bool autocommit)
{
    m_conn->begin(autocommit);
}

void module::sqlite_db::commit()
{
    m_conn->commit();
}

void module::sqlite_db::rollback()
{
    m_conn->rollback();
}

std::string module::sqlite_db::last_error()
{
    return m_conn->last_error();
}

void module::sqlite_db::regist(sol::state* lua)
{
    lua->new_usertype<module::sqlite_db>("sqlite_db",
        "new", sol::constructors<module::sqlite_db()>(),
        "setsql", &module::sqlite_db::setsql,
        "begin", &module::sqlite_db::begin,
        "clear", &module::sqlite_db::clear,
        "commit", &module::sqlite_db::commit,
        "insert_id", &module::sqlite_db::insert_id,
        "rollback", &module::sqlite_db::rollback,
        "open", &module::sqlite_db::open,
        "self", &module::sqlite_db::self,
        "last_error", &module::sqlite_db::last_error
     
    );
}
void module::sqlite_db::regist_global(const char* name, sol::state* lua)
{
    lua->registry()[name] = this;
    (*lua)[name] = this;
}
module::sqlite_prepare_statement::sqlite_prepare_statement(ylib::sqlite::prepare_statement* pstt) :m_ppst(pstt)
{
}

module::sqlite_prepare_statement::~sqlite_prepare_statement()
{
}

void module::sqlite_prepare_statement::set_bigint(uint32 index, const std::string& value)
{
    m_ppst->set_bigint(index, value);
}
void module::sqlite_prepare_statement::set_dob(uint32 index, double value)
{
    m_ppst->set_double(index, value);
}

void module::sqlite_prepare_statement::set_i32(uint32 index, int32 value)
{
    m_ppst->set_int32(index, value);
}

void module::sqlite_prepare_statement::set_i64(uint32 index, int64 value)
{
    m_ppst->set_int64(index, value);
}

void module::sqlite_prepare_statement::set_null(uint32 index)
{
    m_ppst->set_null(index);
}

void module::sqlite_prepare_statement::set_str(uint32 index, const std::string_view& value)
{
    m_ppst->set_string(index, (char*)value.data(), value.size());
}
void module::sqlite_prepare_statement::clear()
{
    m_ppst->clear();
}

uint64 module::sqlite_prepare_statement::update()
{
    return m_ppst->update();
}

std::shared_ptr<module::sqlite_result> module::sqlite_prepare_statement::query()
{
    auto result = m_ppst->query();
    if (result == nullptr)
        return nullptr;
    return std::make_shared<module::sqlite_result>(result);
}

void module::sqlite_prepare_statement::regist(sol::state* lua)
{
    lua->new_usertype<module::sqlite_prepare_statement>("sqlite_prepare_statement",
        "update", &module::sqlite_prepare_statement::update,
        "clear", &module::sqlite_prepare_statement::clear,
        "query", &module::sqlite_prepare_statement::query,
        "set_bigint", &module::sqlite_prepare_statement::set_bigint,
        "set_dob", &module::sqlite_prepare_statement::set_dob,
        "set_i32", &module::sqlite_prepare_statement::set_i32,
        "set_i64", &module::sqlite_prepare_statement::set_i64,
        "set_str", &module::sqlite_prepare_statement::set_str
    );
}
