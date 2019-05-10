#pragma once
#include "TDB.h"

#define __MYSQL_ERROR TDBError(mysql_error((MYSQL*)_Tsql), (int)mysql_errno((MYSQL*)_Tsql))
#define __MYSQL_OK TDBError("", TDB_OK)

namespace TModel
{
	class TMODEL_API TMySQL :
		public TDB
	{
		friend class TDBResult;
		friend class TDBRow;
	public:
		TMySQL(const TMySQL&) = delete;
		TMySQL& operator= (const TMySQL&) = delete;

		TMySQL();
		virtual ~TMySQL() {}

		virtual TDBError Open(
			const std::string address,
			const short port,
			const std::string username,
			const std::string password);

		virtual TDBError Query(
			TDBResult* result,
			const char* cmd,
			...);

		virtual TDBError Query(
			TDBResult* result,
			const char* cmd,
			va_list& ap);

		virtual TDBError SelectDB(const std::string dbName);
		virtual void Close();
	};
}