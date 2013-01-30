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
#include <Interface/CommonUtilMgr/Ix_CommonUtilMgr.h>

#include "DbMgrDef.h"
#include "Cx_DbMgr.h"



LWDP_NAMESPACE_BEGIN;

Cx_DbMgr::Cx_DbMgr()
{

}

Cx_DbMgr::~Cx_DbMgr()
{

}

LWRESULT Cx_DbMgr::Init()
{
	if (mysql_library_init(0, NULL, NULL)) 
	{
		LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					  "Can NOT Initialize MySQL Library", 
					  mysql_error(NULL));

		return LWDP_DB_LIBRARY_INIT_ERROR;
	}
	LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::INFO, 
				   "Initialize MySQL Library OK!");

	return LWDP_OK;
}


DBHandle Cx_DbMgr::Open(const std::string& host, const std::string& user, const std::string& passwd, const std::string& db,
 					      int32_ port, long_ client_flag)
{
	char value = 1;
	char timeOut = 5;
	DBHandleClass* dbclass = new DBHandleClass;
	ASSERT_CHECK(dbclass != 0);
	dbclass->mDb = mysql_init(NULL);
	if(NULL == dbclass->mDb) 
		goto EXT;

	if (NULL == mysql_real_connect(dbclass->mDb, host.c_str(), user.c_str(), passwd.c_str(), db.c_str(), port, NULL, client_flag))
		goto EXT;


	mysql_options(dbclass->mDb, MYSQL_OPT_CONNECT_TIMEOUT, (char*)(&timeOut));
	mysql_options(dbclass->mDb, MYSQL_OPT_RECONNECT, (char *)&value);
	mysql_set_character_set(dbclass->mDb,"gb2312");

	ExecSQL((DBHandle)dbclass, "set interactive_timeout=31536000");
	ExecSQL((DBHandle)dbclass, "set wait_timeout=31536000");
	ExecSQL((DBHandle)dbclass, "set autocommit=1");
	
	if (mysql_select_db(dbclass->mDb, db.c_str()) != 0) 
	{
		mysql_close(dbclass->mDb);
		dbclass->mDb = NULL;
		goto EXT;
	}

	dbclass->mHost = host;
	dbclass->mUser = user;
	dbclass->mPasswd = passwd;
	dbclass->mDbStr  = db;
	dbclass->mPort   = port;
	dbclass->mClientFlag = client_flag;

	return (DBHandle)dbclass;
	
EXT:
	LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					"mysql_real_connect Error(%s)!", mysql_error(dbclass->mDb));

	//��ʼ��mysql�ṹʧ��
	if (dbclass->mDb != NULL )
	{
		mysql_close( dbclass->mDb );
		dbclass->mDb = NULL;
	}
	DELETE_SINGLE(dbclass);

	return NULL;
}

DBHandle Cx_DbMgr::OpenCopy(DBHandle otherHandle)
{
	ASSERT_CHECK(otherHandle != 0);
	DBHandleClass* dbclass = (DBHandleClass*)otherHandle;
	return Open(dbclass->mHost, dbclass->mUser, dbclass->mPasswd, dbclass->mDbStr,
 				dbclass->mPort, dbclass->mClientFlag);
}


void Cx_DbMgr::Close(DBHandle db)
{
	ASSERT_CHECK(db != 0);
	DBHandleClass* dbclass = (DBHandleClass*)db;
	if (dbclass->mDb != NULL)
	{
		mysql_close(dbclass->mDb);
		dbclass->mDb = NULL;
	}
	DELETE_SINGLE(dbclass);
}


DBHandle Cx_DbMgr::GetDbHandle()
{
	return (DBHandle)NULL;
}


LWRESULT Cx_DbMgr::QuerySQL(DBHandle db, const std::string& sql, Cx_Interface<Ix_DbQuery>& query_out)
{
	ASSERT_CHECK(db != 0);
	//lw_mutex_class db_mutex(&q_mutex);
	DBHandleClass* dbclass = (DBHandleClass*)db;
	Cx_Interface<Ix_DbQuery> tmpQuery(CLSID_DbQuery);
	if(!tmpQuery)
	{
		LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					   "Get DbQuery Object Error!");
		return LWDP_GET_QUERY_OBJ_ERROR;
	}

    int32_ ret = Ping(db);
	if(ret != LWDP_OK)
	{
		//ִ�в�ѯʧ��
		LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					   "MySql Connect is Lost");
		return LWDP_ERROR;
	}
	
	if(mysql_real_query(dbclass->mDb, sql.c_str(), sql.size()) != 0)
	{
		LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					   "mysql_real_query Error(%s)!", mysql_error(dbclass->mDb));
		return LWDP_REAL_QUERY_ERROR;
	}
	
	Cx_DbQuery* query = dynamic_cast<Cx_DbQuery*>(tmpQuery.P()); 
	ASSERT_CHECK_RET(LWDP_DB_LOG, LWDP_GET_OBJECT_ERROR, query, 
					 "Cx_DbMgr::QuerySQL Get Cx_DbQuery error, there is not a Cx_DbQuery obj");

	query->mMysqlRes 	= mysql_store_result(dbclass->mDb);
	query->mRow 		= mysql_fetch_row(query->mMysqlRes);
	query->mRow_count 	= mysql_num_rows(query->mMysqlRes); 
	//�õ��ֶ�����
	query->mField_count = mysql_num_fields(query->mMysqlRes);

	query_out = tmpQuery;
	return LWDP_OK;
}

int32_ Cx_DbMgr::ExecSQL(DBHandle db, const std::string& sql)
{
	ASSERT_CHECK(db != 0);
	//lw_mutex_class db_mutex(&q_mutex);
	DBHandleClass* dbclass = (DBHandleClass*)db;
	int ret = 0;
	ret = Ping(db);
	if(ret != LWDP_OK)
	{
		//ִ�в�ѯʧ��
		LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					   "MySql Connect is Lost");
		return LWDP_ERROR;
	}
	if((ret = mysql_real_query(dbclass->mDb, sql.c_str(), sql.size())))
	{
		//ִ�в�ѯʧ��
		LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					   "ExecSQL mysql_real_query(%s) ret Error(%x)(%s)", 
					   sql.c_str(), ret, mysql_error(dbclass->mDb));
		return LWDP_ERROR;
	}

	//�õ���Ӱ�������
	return (int32_)mysql_affected_rows(dbclass->mDb) ;
}


int32_ Cx_DbMgr::Ping(DBHandle db)
{
	ASSERT_CHECK(db != 0);
	DBHandleClass* dbclass = (DBHandleClass*)db;
	//lw_mutex_class mutex_c(&q_mutex);
	//const char* mySqlState = mysql_stat(mDb);
	//if(!mySqlState)
	{
		if((mysql_ping(dbclass->mDb) == 0) && dbclass->mDb)
			return LWDP_OK;
		else 
		{
			if(dbclass->mDb)
			{
				mysql_close(dbclass->mDb);
				dbclass->mDb = NULL;
			}
			LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::WARNING, 
							"Ping Error Reconnect Db!");

			DBHandle reconndb = NULL;
			reconndb = Open(dbclass->mHost, 
							dbclass->mUser, 
							dbclass->mPasswd, 
							dbclass->mDbStr, 
							dbclass->mPort, 
							dbclass->mClientFlag);
			if(!reconndb)
				return LWDP_PING_DB_ERROR;

			DBHandleClass* reconnDbClass = (DBHandleClass*)reconndb;
			dbclass->mDb = reconnDbClass->mDb;
			delete reconndb;
		}
	}
	return LWDP_OK;
}


int32_ Cx_DbMgr::ShutDown(DBHandle db)
{
	ASSERT_CHECK(db != 0);
	DBHandleClass* dbclass = (DBHandleClass*)db;
	if(mysql_shutdown(dbclass->mDb, SHUTDOWN_DEFAULT) == 0)
		return LWDP_OK;
	else 
		return LWDP_SHUTDOWN_DB_ERROR;
}


int32_ Cx_DbMgr::Reboot(DBHandle db)
{
	ASSERT_CHECK(db != 0);
	DBHandleClass* dbclass = (DBHandleClass*)db;
	if(!mysql_reload(dbclass->mDb))
		return LWDP_OK;
	else
		return LWDP_REBOOT_DB_ERROR;
}



int32_ Cx_DbMgr::StartTransaction(DBHandle db)
{
	ASSERT_CHECK(db != 0);
	int ret = 0;
	DBHandleClass* dbclass = (DBHandleClass*)db;
	if((ret = mysql_real_query(dbclass->mDb, "START TRANSACTION" ,
						     (unsigned long)strlen("START TRANSACTION"))))
	{
		LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					   "mysql_real_query(START TRANSACTION) ret Error(%x: %s)", 
					   ret, mysql_error(dbclass->mDb));
		return ret;
	}

	return LWDP_OK;

}


int32_ Cx_DbMgr::Commit(DBHandle db)
{
	ASSERT_CHECK(db != 0);
	int ret = 0;
	DBHandleClass* dbclass = (DBHandleClass*)db;
	if(ret = mysql_real_query(dbclass->mDb, "COMMIT" ,
						     (unsigned long)strlen("COMMIT")))
	{
		LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					   "mysql_real_query(COMMIT) ret Error(%x: %s)", 
					   ret, mysql_error(dbclass->mDb));
		return ret;
	}

	return LWDP_OK;
}


int32_ Cx_DbMgr::Rollback(DBHandle db)
{
	ASSERT_CHECK(db != 0);
	int ret = 0;
	DBHandleClass* dbclass = (DBHandleClass*)db;
	if(ret = mysql_real_query(dbclass->mDb, "ROLLBACK" ,
						     (unsigned long)strlen("ROLLBACK")))
	{
		LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					   "mysql_real_query(ROLLBACK) ret Error(%x: %s)", 
					   ret, mysql_error(dbclass->mDb));
		return ret;
	}

	return LWDP_OK;
}


const std::string Cx_DbMgr::GetClientInfo()
{
	return static_cast<const std::string>(mysql_get_client_info());
}


const long_ Cx_DbMgr::GetClientVersion()
{
	return static_cast<const long_>(mysql_get_client_version());
}


const std::string Cx_DbMgr::GetHostInfo(DBHandle db)
{
	ASSERT_CHECK(db != 0);
	DBHandleClass* dbclass = (DBHandleClass*)db;
	return static_cast<const std::string>(mysql_get_host_info(dbclass->mDb));
}


const std::string Cx_DbMgr::GetServerInfo(DBHandle db)
{
	ASSERT_CHECK(db != 0);
	DBHandleClass* dbclass = (DBHandleClass*)db;
	return static_cast<const std::string>(mysql_get_server_info(dbclass->mDb));
}


const long_ Cx_DbMgr::GetServerVersion(DBHandle db)
{
	ASSERT_CHECK(db != 0);
	DBHandleClass* dbclass = (DBHandleClass*)db;
	return static_cast<const long_>(mysql_get_server_version(dbclass->mDb));
}


const std::string Cx_DbMgr::GetCharacterSetName(DBHandle db)
{
	ASSERT_CHECK(db != 0);
	DBHandleClass* dbclass = (DBHandleClass*)db;
	return static_cast<const std::string>(mysql_character_set_name(dbclass->mDb));
}


const std::string Cx_DbMgr::GetSysTime(DBHandle db)
{
	ASSERT_CHECK(db != 0);
	DBHandleClass* dbclass = (DBHandleClass*)db;
	//return ExecQueryGetSingValue("select now()");
	return NULL;
}


int32_ Cx_DbMgr::CreateDB(DBHandle db, const std::string& name)
{
	ASSERT_CHECK(db != 0);
	std::string str;
	DBHandleClass* dbclass = (DBHandleClass*)db;
	str = std::string("CREATE DATABASE ") + name;
	int ret = 0;
	if(ret = mysql_real_query(dbclass->mDb, str.c_str(),
						     (unsigned long)str.size()))
	{
		LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					   "mysql_real_query(%s) ret Error(%x: %s)", 
					   str.c_str(), ret, mysql_error(dbclass->mDb));
		return ret;
	}

	return LWDP_OK;
}


int32_ Cx_DbMgr::DropDB(DBHandle db, const std::string& name)
{
	ASSERT_CHECK(db != 0);
	std::string str;
	DBHandleClass* dbclass = (DBHandleClass*)db;
	str = std::string("DROP DATABASE ") + name;
	int ret = 0;
	if(ret = mysql_real_query(dbclass->mDb, str.c_str(),
						     (unsigned long)str.size()))
	{
		LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					   "mysql_real_query(%s) ret Error(%x: %s)", 
					   str.c_str(), ret, mysql_error(dbclass->mDb));
		return ret;
	}

	return LWDP_OK;
}

bool_ Cx_DbMgr::TableExists(DBHandle db, const std::string& table)
{
	ASSERT_CHECK(db != 0);
	return false;
}

uint32_ Cx_DbMgr::LastRowId(DBHandle db)
{
	ASSERT_CHECK(db != 0);
	return LWDP_OK;
}

void Cx_DbMgr::SetBusyTimeout(DBHandle db, int32_ nMillisecs)
{
	ASSERT_CHECK(db != 0);

}



//////////////////////////////////////////////////////
Cx_DbQuery::Cx_DbQuery()
{
	mMysqlRes 	 = NULL;
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
	if(mMysqlRes != NULL)
	{
		mysql_free_result(mMysqlRes);
		mMysqlRes = NULL;
	}
}

uint32_ Cx_DbQuery::NumRow()//������
{
	return mRow_count;
}

int32_  Cx_DbQuery::NumFields()//������
{
	return mField_count;
}

int32_  Cx_DbQuery::FieldIndex(const std::string& szField)
{
	if(NULL == mMysqlRes || szField.empty())
		return -1;

	mysql_field_seek(mMysqlRes, 0);//��λ����0��
	uint32_ i = 0;
	while(i < mField_count)
	{
		mField = mysql_fetch_field( mMysqlRes );
		if(mField != NULL && (std::string(mField->name) == szField))//�ҵ�
			return i;
		i++;
	}
	return -1;
}

//0...n-1��
const std::string Cx_DbQuery::FieldName(int32_ nCol)
{
	if(mMysqlRes == NULL)
		return static_cast<const std::string>("");
	mysql_field_seek(mMysqlRes, nCol);
	mField = mysql_fetch_field(mMysqlRes);
	if(mField != NULL)
		return static_cast<const std::string>(mField->name);
	else
		return  static_cast<const std::string>("");
}

uint32_ Cx_DbQuery::SeekRow(uint32_ offerset)
{
	if(offerset < 0)
		offerset = 0;
	if(offerset >= mRow_count)
		offerset = mRow_count -1;
	mysql_data_seek(mMysqlRes, offerset);

	mRow = mysql_fetch_row(mMysqlRes);
	return offerset;
}

int32_  Cx_DbQuery::GetIntField(int32_ nField, int32_ nNullValue)
{
	if(NULL == mMysqlRes)
		return nNullValue;

	if(nField + 1 > (int)mField_count)
		return nNullValue;

	if(NULL == mRow)
		return nNullValue;

	return atoi(mRow[nField]);
}

int32_  Cx_DbQuery::GetIntField(const std::string& szField, int32_ nNullValue)
{
	if(NULL == mMysqlRes || szField.empty())
		return nNullValue;

	if(NULL == mRow)
		return nNullValue;
	const std::string filed = GetStringField(szField);
	if(filed.empty())
		return nNullValue;
	
	return atoi(filed.c_str());
}

double_ Cx_DbQuery::GetFloatField(int32_ nField, double_ fNullValue)
{
	const std::string field = GetStringField(nField);
	if(field.empty())
		return fNullValue;
	
	return atol(field.c_str());

}

double_ Cx_DbQuery::GetFloatField(const std::string& szField, double_ fNullValue)
{
	const std::string field = GetStringField(szField);
	if (field.empty())
		return fNullValue;
	
	return atol(field.c_str());
}

//0...n-1��
const std::string Cx_DbQuery::GetStringField(int32_ nField, const std::string& szNullValue)
{
	if(NULL == mMysqlRes)
		return szNullValue;
	if(nField + 1 > (int)mField_count)
		return szNullValue;
	if(NULL == mRow)
		return szNullValue;
	
	return mRow[nField];
}

const std::string Cx_DbQuery::GetStringField(const std::string& szField, const std::string& szNullValue)
{
	if(NULL == mMysqlRes)
		return szNullValue;
	int nField = FieldIndex(szField);
	if(nField == -1)
		return szNullValue;

	return GetStringField(nField);
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
	if ( NULL == mMysqlRes )
		return;
	mRow = mysql_fetch_row(mMysqlRes);
}

void Cx_DbQuery::Finalize()
{


}





LWDP_NAMESPACE_END;
