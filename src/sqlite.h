#pragma once
#include"db/sqlite.h"
#include "db/sqler.h"
#include "sol/sol.hpp"
#include "basemodule.h"
namespace module
{
	/// <summary>
	/// 注册
	/// </summary>
	/// <param name="lua"></param>
	void sqlite_regist(sol::state* lua);
	/// <summary>
	/// 结果集
	/// </summary>
	class sqlite_result
	{
	public:
		sqlite_result(ylib::sqlite::result* result);
		~sqlite_result();
		/// <summary>
		/// 字段数
		/// </summary>
		/// <returns></returns>
		uint32 field_count();
		/// <summary>
		/// 字段类型
		/// </summary>
		/// <param name="obj"></param>
		/// <returns></returns>
		std::string field_type(sol::object obj);
		/// <summary>
		/// 字段名称
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		std::string field_name(uint32 index);
		/// <summary>
		/// 下一行
		/// </summary>
		/// <returns></returns>
		bool next();
		/// <summary>
		/// 取数据
		/// </summary>
		/// <param name="name"></param>
		/// <returns></returns>
		sol::object get(sol::object obj, sol::this_state s);
		/// <summary>
		/// 取结果集到table
		/// </summary>
		/// <param name="s"></param>
		/// <returns></returns>
		sol::table table(sol::this_state s);
		/// <summary>
		/// 注册
		/// </summary>
		/// <param name="lua"></param>
		static void regist(sol::state* lua);
	private:
		ylib::sqlite::result* m_result = nullptr;
	};

	
	class sqlite_prepare_statement {
	public:
		sqlite_prepare_statement(ylib::sqlite::prepare_statement* pstt);
		~sqlite_prepare_statement();
		void set_bigint(uint32 index, const std::string& value);
		void set_dob(uint32 index, double value);
		void set_i32(uint32 index, int32 value);
		void set_i64(uint32 index, int64 value);
		void set_null(uint32 index);
		void set_str(uint32 index, const std::string_view& value);
		void clear();
		uint64 update();
		std::shared_ptr<module::sqlite_result> query();

		static void regist(sol::state* lua);
	private:
		ylib::sqlite::prepare_statement* m_ppst = nullptr;
	};

	class sqlite_db :public module::base
	{
	public:
		sqlite_db();
		~sqlite_db();

		bool open(const std::string& database_filepath);
		void clear();
		std::shared_ptr<module::sqlite_prepare_statement> setsql(const std::string& sql);
		uint64 insert_id();
		void begin(bool autocommit = false);
		void commit();
		void rollback();
		std::string last_error();


		static void regist(sol::state* lua);
	private:
		// 通过 imodule 继承
		virtual void regist_global(const char* name, sol::state* lua);
		virtual void delete_global() { delete this; }
	private:
		std::unique_ptr<ylib::sqlite::conn> m_conn;
	};
	
}

