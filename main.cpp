#include <iostream>
#include "TMySQL.h"
#include "TDB.h"
#include "TDBResult.hpp"
using namespace std;
using namespace TModel;


static int UpdateAccountFromMysql(char* errmsg, int errlen, const std::vector<std::string> &m_Brokers)
{
	//LOG(INFO) << "UpdateAccountFromMysql";
	// 读取一些基础信息判断是否有效
	string sql_ipaddress = "47.100.**.***";
	string sql_port = "3306";
	string sql_username = "***";
	string sql_password = "***";
	string sql_dbname = "Operation";
	if (sql_ipaddress.empty() || sql_port.empty() || sql_username.empty() || sql_password.empty() || sql_dbname.empty())
	{
		if (errmsg != NULL && errlen > 0)
		{
			snprintf(errmsg, errlen, "getenv Error");
		//	LOG(ERROR) << "getenv Error";
		}
		return -1;
	}

	// 连接数据库获取账户信息
	TMySQL mysqldb;
	TDBError dberror = mysqldb.Open(sql_ipaddress, (short)strtol(sql_port.c_str(), NULL, 10), sql_username, sql_password);
	if (dberror.error != TDB_OK)
	{
		if (errmsg != NULL && errlen > 0)
		{
			snprintf(errmsg, errlen, "Open MySql Error:%s", dberror.strError.c_str());
		//	LOG(ERROR) << "Open MySql Error: " << dberror.strError.c_str();
		}
		return -1;
	}

	dberror = mysqldb.SelectDB(sql_dbname);
	if (dberror.error != TDB_OK)
	{
		mysqldb.Close();

		if (errmsg != NULL && errlen > 0)
		{
			snprintf(errmsg, errlen, "%s SelectDB MySql Error:%s", sql_dbname.c_str(), dberror.strError.c_str());
		//	LOG(ERROR) << sql_dbname.c_str() << " SelectDB MySql Error: " << dberror.strError.c_str();
		}
		return -1;
	}

	int count = 0;
	// 轮寻每个柜台通道到数据库中获取账户信息
	for (std::string broker : m_Brokers)
	{
		// 到数据库中查询柜台通道对应的账户数据集
		char szqueryinfo[512];
		//snprintf(szqueryinfo, sizeof(szqueryinfo), "SELECT ClientID FROM AG_CLIENT where Broker = \'%s\'", broker.c_str());
		snprintf(szqueryinfo, sizeof(szqueryinfo), "select * from master WHERE customerName=\'%s\'", broker.c_str());

	//	LOG(INFO) << broker << " Broker MySql QuerySql=" << szqueryinfo;

		TDBResult dbresult;
		dberror = mysqldb.Query(&dbresult, szqueryinfo);
		if (dberror.error != TDB_OK)
		{
			mysqldb.Close();

			if (errmsg != NULL && errlen > 0)
			{
				snprintf(errmsg, errlen, "%s Query MySql Error:%s", broker.c_str(), dberror.strError.c_str());
		//		LOG(ERROR) << broker.c_str() << " Query MySql Error: " << dberror.strError.c_str();
			}
			return -1;
		}

		// 判断如果没有记录则报出警告，然后找下一个柜台进行处理
		int recordcount = (int)dbresult.size();
		if (recordcount <= 0)
		{
		//	LOG(WARNING) << broker << " :MySql Table AG_CLIENT Have No Result";
			continue;
		}

		// 保存柜台通道对应的资金账户信息
		unordered_map<string, TDBRowValue>::iterator iter;
		std::cout << endl;
		for (int i = 0; i < recordcount; i++)
		{		
			for (auto it = dbresult[i].begin(); it != dbresult[i].end(); ++it)
			{
				std::cout << it->second << '\t';
			}
			std::cout << endl;
		}
	}
	mysqldb.Close();

	return count;
}


int main()
{
	std::vector<std::string> aa;
	aa.emplace_back("gumu");
	aa.emplace_back("yinnuo");
	aa.emplace_back("tiansuan");

	cout << "hello" << endl;

	UpdateAccountFromMysql(NULL,0,aa);
	getchar();
	return 0;
}
