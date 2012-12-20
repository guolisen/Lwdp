/*! \file Cx_DbMgr.cpp
 *  \brief DbMgr Module Impl
 *  \author Guolisen, LwDp
 *  \date   2012.10.17
 */
 
#include <LwDp.h>
#include <PluginInc.h>
#include <iostream>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>

#include "DbMgrDef.h"
#include "Cx_DbMgr.h"



LWDP_NAMESPACE_BEGIN;


Cx_DbMgr::Cx_DbMgr()
{
	mDb = NULL;
}

Cx_DbMgr::~Cx_DbMgr()
{
	if ( mDb != NULL )
	{
		Close();
	}
}

LWRESULT Cx_DbMgr::Init()
{
	lw_log_info(LWDP_LUA_LOG, __T("Cx_DbMgr::Init OK!"));



	return LWDP_OK;
}


LWRESULT Cx_DbMgr::Open(const std::string& host, const std::string& user, const std::string& passwd, const std::string& db,
 					    int32_ port, long_ client_flag)
{
	mDb = mysql_init(NULL);
	if(NULL == mDb) 
		goto EXT;

	//如果连接失败，返回NULL。对于成功的连接，返回值与第1个参数的值相同。
	if (NULL == mysql_real_connect(mDb, host.c_str(), user.c_str(), passwd.c_str(), db.c_str(), port, NULL, client_flag))
		goto EXT;
	//选择制定的数据库失败
	//0表示成功，非0值表示出现错误。
	if (mysql_select_db(mDb, db.c_str()) != 0) 
	{
		mysql_close(mDb);
		mDb = NULL;
		goto EXT;
	}

	return LWDP_OK;
EXT:
	//初始化mysql结构失败
	if (mDb != NULL )
	{
		mysql_close( mDb );
		mDb = NULL;
	}
	
	return LWDP_OPEN_DB_ERROR;
}
void Cx_DbMgr::Close()
{
	if (mDb != NULL)
	{
		mysql_close(mDb);
		mDb = NULL;
	}
}

/* 返回句柄 */
DBHandle Cx_DbMgr::GetDbHandle()
{
	return (DBHandle)mDb;
}

/* 处理返回多行的查询，返回影响的行数 */
//返回引用是因为在CppMySQLQuery的赋值构造函数中要把成员变量_mysql_res置为空
//CppMySQLQuery& Cx_DbMgr::QuerySQL(const std::string& sql)
//{


//}


/* 执行非返回结果查询 */
int32_ Cx_DbMgr::ExecSQL(const std::string& sql)
{
	int ret = 0;
	if((ret = mysql_real_query(mDb, sql.c_str(), sql.size())))
	{
		//执行查询失败
		LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					   "ExecSQL mysql_real_query(%s) ret Error(%x)", 
					   sql.c_str(), ret);
		return LWDP_ERROR;
	}

	//得到受影响的行数
	return (int32_)mysql_affected_rows(mDb) ;
}

/* 测试mysql服务器是否存活 */
int32_ Cx_DbMgr::Ping()
{
	if(mysql_ping(mDb) == 0)
		return LWDP_OK;
	else 
		return LWDP_PING_DB_ERROR; 
}

/* 关闭mysql 服务器 */
int32_ Cx_DbMgr::ShutDown()
{
	if(mysql_shutdown(mDb, SHUTDOWN_DEFAULT) == 0)
		return LWDP_OK;
	else 
		return LWDP_SHUTDOWN_DB_ERROR;
}

/* 主要功能:重新启动mysql 服务器 */
int32_ Cx_DbMgr::Reboot()
{
	if(!mysql_reload(mDb))
		return LWDP_OK;
	else
		return LWDP_REBOOT_DB_ERROR;
}


/*
* 说明:事务支持InnoDB or BDB表类型
*/
/* 主要功能:开始事务 */
int32_ Cx_DbMgr::StartTransaction()
{
	int ret = 0;
	if(ret = mysql_real_query(mDb, "START TRANSACTION" ,
						     (unsigned long)strlen("START TRANSACTION")))
	{
		LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					   "mysql_real_query(START TRANSACTION) ret Error(%x)", 
					   ret);
		return ret;
	}

	return LWDP_OK;

}

/* 主要功能:提交事务 */
int32_ Cx_DbMgr::Commit()
{
	int ret = 0;
	if(ret = mysql_real_query(mDb, "COMMIT" ,
						     (unsigned long)strlen("COMMIT")))
	{
		LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					   "mysql_real_query(COMMIT) ret Error(%x)", 
					   ret);
		return ret;
	}

	return LWDP_OK;
}

/* 主要功能:回滚事务 */
int32_ Cx_DbMgr::Rollback()
{
	int ret = 0;
	if(ret = mysql_real_query(mDb, "ROLLBACK" ,
						     (unsigned long)strlen("ROLLBACK")))
	{
		LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					   "mysql_real_query(ROLLBACK) ret Error(%x)", 
					   ret);
		return ret;
	}

	return LWDP_OK;
}

/* 得到客户信息 */
const std::string Cx_DbMgr::GetClientInfo()
{
	return static_cast<const std::string>(mysql_get_client_info());
}

/* 主要功能:得到客户版本信息 */
const long_ Cx_DbMgr::GetClientVersion()
{
	return static_cast<const long_>(mysql_get_client_version());
}

/* 主要功能:得到主机信息 */
const std::string Cx_DbMgr::GetHostInfo()
{
	return static_cast<const std::string>(mysql_get_host_info(mDb));
}

/* 主要功能:得到服务器信息 */
const std::string Cx_DbMgr::GetServerInfo()
{
	return static_cast<const std::string>(mysql_get_server_info(mDb));
}

/*主要功能:得到服务器版本信息*/
const long_ Cx_DbMgr::GetServerVersion()
{
	return static_cast<const long_>(mysql_get_server_version(mDb));
}

/*主要功能:得到 当前连接的默认字符集*/
const std::string Cx_DbMgr::GetCharacterSetName()
{
	return static_cast<const std::string>(mysql_character_set_name(mDb));
}

/* 得到系统时间 */
const std::string Cx_DbMgr::GetSysTime()
{
	//return ExecQueryGetSingValue("select now()");
	return NULL;
}

/* 建立新数据库 */
int32_ Cx_DbMgr::CreateDB(const std::string& name)
{
	std::string str;
	str = std::string("CREATE DATABASE ") + name;
	int ret = 0;
	if(ret = mysql_real_query(mDb, str.c_str(),
						     (unsigned long)str.size()))
	{
		LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					   "mysql_real_query(%s) ret Error(%x)", 
					   str.c_str(), ret);
		return ret;
	}

	return LWDP_OK;
}

/* 删除制定的数据库*/
int32_ Cx_DbMgr::DropDB(const std::string& name)
{
	std::string str;
	str = std::string("DROP DATABASE ") + name;
	int ret = 0;
	if(ret = mysql_real_query(mDb, str.c_str(),
						     (unsigned long)str.size()))
	{
		LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					   "mysql_real_query(%s) ret Error(%x)", 
					   str.c_str(), ret);
		return ret;
	}

	return LWDP_OK;
}

bool_ Cx_DbMgr::TableExists(const std::string& table)
{
	return false;
}

uint32_ Cx_DbMgr::LastRowId()
{
	return LWDP_OK;
}

void Cx_DbMgr::SetBusyTimeout(int32_ nMillisecs)
{


}



//////////////////////////////////////////////////////
Cx_DbQuery::Cx_DbQuery()
{
	mMysql_res 	 = NULL;
	mField 		 = NULL;
	mRow 		 = NULL;
	mRow_count 	 = 0;
	mField_count = 0;
}

Cx_DbQuery::~Cx_DbQuery()
{
	freeRes();
}

void Cx_DbQuery::freeRes()
{
	if(mMysql_res != NULL)
	{
		mysql_free_result(mMysql_res);
		mMysql_res = NULL;
	}
}

uint32_ Cx_DbQuery::NumRow()//多少行
{
	return mRow_count;
}

int32_  Cx_DbQuery::NumFields()//多少列
{
	return mField_count;
}

int32_  Cx_DbQuery::FieldIndex(const std::string& szField)
{
	if(NULL == mMysql_res || szField.empty())
		return -1;

	mysql_field_seek(mMysql_res, 0);//定位到第0列
	int32_ i = 0;
	while(i < mField_count)
	{
		mField = mysql_fetch_field( mMysql_res );
		if(mField != NULL && strcmp(mField->name, szField) == 0)//找到
			return i;
		i++;
	}
	return -1;
}

//0...n-1列
const std::string Cx_DbQuery::FieldName(int32_ nCol)
{
	if(mMysql_res == NULL)
		return static_cast<const std::string>("");
	mysql_field_seek(mMysql_res, nCol);
	mField = mysql_fetch_field(mMysql_res);
	if(mField != NULL)
		return static_cast<const std::string>(mField->name);
	else
		return  static_cast<const std::string>("");
}

uint32_ Cx_DbQuery::SeekRow(u_long offerset)
{
	if(offerset < 0)
		offerset = 0;
	if(offerset >= mRow_count)
		offerset = mRow_count -1;
	mysql_data_seek(mMysql_res, offerset);

	mRow = mysql_fetch_row(mMysql_res);
	return offerset;
}

int32_  Cx_DbQuery::GetIntField(int32_ nField, int32_ nNullValue)
{
	if(NULL == mMysql_res)
		return nNullValue;

	if(nField + 1 > (int)mField_count)
		return nNullValue;

	if(NULL == mRow)
		return nNullValue;

	return atoi(mRow[nField]);
}

int32_  Cx_DbQuery::GetIntField(const std::string& szField, int32_ nNullValue)
{
	if(NULL == mMysql_res || NULL == szField)
		return nNullValue;

	if(NULL == mRow)
		return nNullValue;
	const char* filed = getStringField(szField);
	if(NULL == filed)
		return nNullValue;
	
	return atoi(filed);
}

double_ Cx_DbQuery::GetFloatField(int32_ nField, double_ fNullValue)
{
	const char* field = getStringField(nField);
	if(NULL == field)
		return fNullValue;
	
	return atol(field);

}

double_ Cx_DbQuery::GetFloatField(const std::string& szField, double_ fNullValue)
{
	const char* field = getStringField(szField);
	if ( NULL == field )
		return fNullValue;
	
	return atol(field);
}

//0...n-1列
const std::string Cx_DbQuery::GetStringField(int32_ nField, const std::string& szNullValue)
{
	if(NULL == mMysql_res)
		return szNullValue;
	if(nField + 1 > (int)mField_count)
		return szNullValue;
	if(NULL == mRow)
		return szNullValue;
	
	return mRow[nField];
}

const std::string Cx_DbQuery::GetStringField(const std::string& szField, const std::string& szNullValue)
{
	if(NULL == mMysql_res)
		return szNullValue;
	int nField = fieldIndex(szField);
	if(nField == -1)
		return szNullValue;

	return getStringField(nField);
}

bool Cx_DbQuery::FieldIsNull(int32_ nField)
{
	return true;
}

bool Cx_DbQuery::GieldIsNull(const std::string& szField)
{
	return true;
}

bool Cx_DbQuery::Eof()
{
	if ( mRow == NULL )
		return true;
	
	return false;
}

void Cx_DbQuery::NextRow()
{
	if ( NULL == mMysql_res )
		return;
	mRow = mysql_fetch_row(mMysql_res);
}

void Cx_DbQuery::Finalize()
{


}





LWDP_NAMESPACE_END;

