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

pthread_mutex_t db_mutex = PTHREAD_MUTEX_INITIALIZER;
class db_mutex_class
{
public:
	db_mutex_class()
	{
		pthread_mutex_lock(&db_mutex);
	};
	virtual ~db_mutex_class()
	{
		pthread_mutex_unlock(&db_mutex);
	};
};

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
	if (mysql_library_init(0, NULL, NULL)) 
	{
		LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					  "Can NOT Initialize MySQL Library(%s)", 
					  mysql_error(mDb));

		return LWDP_DB_LIBRARY_INIT_ERROR;
	}
	LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::INFO, 
				   "Initialize MySQL Library OK!");

	return LWDP_OK;
}


LWRESULT Cx_DbMgr::Open(const std::string& host, const std::string& user, const std::string& passwd, const std::string& db,
 					    int32_ port, long_ client_flag)
{
	char value = 1;
	mDb = mysql_init(NULL);
	if(NULL == mDb) 
		goto EXT;


	if (NULL == mysql_real_connect(mDb, host.c_str(), user.c_str(), passwd.c_str(), db.c_str(), port, NULL, client_flag))
		goto EXT;

	mysql_options(mDb, MYSQL_OPT_RECONNECT, (char *)&value);
	mysql_set_character_set(mDb,"gb2312");
	
	if (mysql_select_db(mDb, db.c_str()) != 0) 
	{
		mysql_close(mDb);
		mDb = NULL;
		goto EXT;
	}

	mHost = host;
	mUser = user;
	mPasswd = passwd;
	mDbStr  = db;
	mPort   = port;
	mClientFlag = client_flag;

	return LWDP_OK;
EXT:
	LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					"mysql_real_connect Error(%s)!", mysql_error(mDb));

	//��ʼ��mysql�ṹʧ��
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


DBHandle Cx_DbMgr::GetDbHandle()
{
	return (DBHandle)mDb;
}


LWRESULT Cx_DbMgr::QuerySQL(const std::string& sql, Cx_Interface<Ix_DbQuery>& query_out)
{
	db_mutex_class db_mutex;
	Cx_Interface<Ix_DbQuery> tmpQuery(CLSID_DbQuery);
	if(!tmpQuery)
	{
		LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					   "Get DbQuery Object Error!");
		return LWDP_GET_QUERY_OBJ_ERROR;
	}
	
	if(mysql_real_query(mDb, sql.c_str(), sql.size()) != 0)
	{
		LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					   "mysql_real_query Error(%s)!", mysql_error(mDb));
		return LWDP_REAL_QUERY_ERROR;
	}
	
	Cx_DbQuery* query = dynamic_cast<Cx_DbQuery*>(tmpQuery.P()); 
	ASSERT_CHECK_RET(LWDP_DB_LOG, LWDP_GET_OBJECT_ERROR, query, 
					 "Cx_DbMgr::QuerySQL Get Cx_DbQuery error, there is not a Cx_DbQuery obj");

	query->mMysqlRes 	= mysql_store_result(mDb);
	query->mRow 		= mysql_fetch_row(query->mMysqlRes);
	query->mRow_count 	= mysql_num_rows(query->mMysqlRes); 
	//�õ��ֶ�����
	query->mField_count = mysql_num_fields(query->mMysqlRes);

	query_out = tmpQuery;
	return LWDP_OK;
}

int32_ Cx_DbMgr::ExecSQL(const std::string& sql)
{
	db_mutex_class db_mutex;
	int ret = 0;
	if((ret = mysql_real_query(mDb, sql.c_str(), sql.size())))
	{
		//ִ�в�ѯʧ��
		LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					   "ExecSQL mysql_real_query(%s) ret Error(%x)(%s)", 
					   sql.c_str(), ret, mysql_error(mDb));
		return LWDP_ERROR;
	}

	//�õ���Ӱ�������
	return (int32_)mysql_affected_rows(mDb) ;
}


int32_ Cx_DbMgr::Ping()
{
	db_mutex_class db_mutex;
	//const char* mySqlState = mysql_stat(mDb);
	//if(!mySqlState)
	{
		if(mysql_ping(mDb) == 0)
			return LWDP_OK;
		else 
		{
			mysql_close(mDb);
			LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::WARNING, 
							"Ping Error Reconnect Db!");

			if(Open(mHost, mUser, mPasswd, mDbStr, mPort, mClientFlag) != LWDP_OK)
				return LWDP_PING_DB_ERROR;
		}
	}
	return LWDP_OK;
}


int32_ Cx_DbMgr::ShutDown()
{
	if(mysql_shutdown(mDb, SHUTDOWN_DEFAULT) == 0)
		return LWDP_OK;
	else 
		return LWDP_SHUTDOWN_DB_ERROR;
}


int32_ Cx_DbMgr::Reboot()
{
	if(!mysql_reload(mDb))
		return LWDP_OK;
	else
		return LWDP_REBOOT_DB_ERROR;
}



int32_ Cx_DbMgr::StartTransaction()
{
	int ret = 0;
	if((ret = mysql_real_query(mDb, "START TRANSACTION" ,
						     (unsigned long)strlen("START TRANSACTION"))))
	{
		LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					   "mysql_real_query(START TRANSACTION) ret Error(%x: %s)", 
					   ret, mysql_error(mDb));
		return ret;
	}

	return LWDP_OK;

}


int32_ Cx_DbMgr::Commit()
{
	int ret = 0;
	if(ret = mysql_real_query(mDb, "COMMIT" ,
						     (unsigned long)strlen("COMMIT")))
	{
		LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					   "mysql_real_query(COMMIT) ret Error(%x: %s)", 
					   ret, mysql_error(mDb));
		return ret;
	}

	return LWDP_OK;
}


int32_ Cx_DbMgr::Rollback()
{
	int ret = 0;
	if(ret = mysql_real_query(mDb, "ROLLBACK" ,
						     (unsigned long)strlen("ROLLBACK")))
	{
		LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					   "mysql_real_query(ROLLBACK) ret Error(%x: %s)", 
					   ret, mysql_error(mDb));
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


const std::string Cx_DbMgr::GetHostInfo()
{
	return static_cast<const std::string>(mysql_get_host_info(mDb));
}


const std::string Cx_DbMgr::GetServerInfo()
{
	return static_cast<const std::string>(mysql_get_server_info(mDb));
}


const long_ Cx_DbMgr::GetServerVersion()
{
	return static_cast<const long_>(mysql_get_server_version(mDb));
}


const std::string Cx_DbMgr::GetCharacterSetName()
{
	return static_cast<const std::string>(mysql_character_set_name(mDb));
}


const std::string Cx_DbMgr::GetSysTime()
{
	//return ExecQueryGetSingValue("select now()");
	return NULL;
}


int32_ Cx_DbMgr::CreateDB(const std::string& name)
{
	std::string str;
	str = std::string("CREATE DATABASE ") + name;
	int ret = 0;
	if(ret = mysql_real_query(mDb, str.c_str(),
						     (unsigned long)str.size()))
	{
		LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					   "mysql_real_query(%s) ret Error(%x: %s)", 
					   str.c_str(), ret, mysql_error(mDb));
		return ret;
	}

	return LWDP_OK;
}


int32_ Cx_DbMgr::DropDB(const std::string& name)
{
	std::string str;
	str = std::string("DROP DATABASE ") + name;
	int ret = 0;
	if(ret = mysql_real_query(mDb, str.c_str(),
						     (unsigned long)str.size()))
	{
		LWDP_LOG_PRINT("DbMgr", LWDP_LOG_MGR::ERR, 
					   "mysql_real_query(%s) ret Error(%x: %s)", 
					   str.c_str(), ret, mysql_error(mDb));
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
