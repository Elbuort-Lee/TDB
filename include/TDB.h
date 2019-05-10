#pragma once
#include <string>
#include <mutex>
#include "TDBResult.hpp"

#ifdef _WIN32
#	pragma warning(disable: 4251)
#	pragma warning(disable: 4996)
#ifndef TMODEL_API_IMPORT
#	define TMODEL_API __declspec(dllexport)
#else
#	define TMODEL_API __declspec(dllimport)
#endif
#else
#	define TMODEL_API
#endif

#define TDB_OK 0
#define TDB_NO_OPEN -0xFFF0
#define TDB_NO_OPEN_STR "DB not opened."
#define TDB_NOT_EXIST -0xFFF1
#define TDB_NOT_EXIST_STR "DB not exist"
#define TDB_INVALID_TABLE -0xFFF2
#define TDB_INVALID_TABLE_STR "Invalid table or table not exist"
#define TDB_INVALID_DATA -0XFFF3
#define TDB_INVALID_DATA_STR "Invalid data"

#define TDB_CHECK_OPENED \
if (!_bOpened) return { TDB_NO_OPEN_STR, TDB_NO_OPEN };

#define __RET_TD_OK  return TDBError("", TDB_OK)

namespace TModel
{
	typedef struct ts_tdb_error
	{
		ts_tdb_error(const std::string str, const int error)
			:strError(str), error(error) {}
		std::string strError;
		int error;
	}TDBError;

	class TMODEL_API TDB
	{
	protected:
		void* _Tsql = nullptr;
		short _port = 0;
		bool _bOpened = false, _bConnected = false;
		std::string _dbUS, _dbPW;
		std::string _dbAddress, _dbName;
		std::mutex _queryMutex;
	public:
		TDB(void* sql) { _Tsql = sql; }
		virtual TDBError Open(
			const std::string address,
			const short port,
			const std::string username,
			const std::string password)
		{
			_dbAddress = address;
			_dbUS = username;
			_dbPW = password;
			_port = port;
			__RET_TD_OK;
		}

		virtual TDBError Query(
			TDBResult* result,
			const char* cmd,
			...) = 0;

		virtual TDBError SelectDB(const std::string dbName) = 0;
		virtual void Close() = 0;
	};
}