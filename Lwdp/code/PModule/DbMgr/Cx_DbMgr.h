/*! \file Cx_DbMgr.h
 *  \brief DbMgr Module Impl
 *  \author Guolisen, LwDp
 *  \date   2012.12.15
 */
 
#ifndef CX_DB_MANAGER_H
#define CX_DB_MANAGER_H

#include <boost/shared_ptr.hpp>
#include <list>
#include <Interface/DbMgr/Ix_DbMgr.h>
//#include <Interface/ScriptMgr/Ix_ScriptMgr.h>
#include "DbMgrDef.h"

#include <iostream>


LWDP_NAMESPACE_BEGIN;
#if 0
class Cx_DbMgr
    : public Ix_DbMgr
{
    X3BEGIN_CLASS_DECLARE(Cx_DbQuery)
        X3DEFINE_INTERFACE_ENTRY(Ix_DbQuery)
    X3END_CLASS_DECLARE()
protected:
	Cx_DbQuery();
	virtual ~Cx_DbQuery();

protected:
 	virtual uint32_ NumRow();//多少行
    virtual int32_  NumFields();//多少列
    virtual int32_  FieldIndex(const std::string& szField);
 //0...n-1列
    virtual const std::string FieldName(int32_ nCol);

 	virtual u_long  SeekRow(u_long offerset);
    virtual int32_  GetIntField(int32_ nField, int32_ nNullValue);
    virtual int32_  GetIntField(const std::string& szField, int32_ nNullValue);
    virtual double_ GetFloatField(int32_ nField, double_ fNullValue);
    virtual double_ GetFloatField(const std::string& szField, double_ fNullValue);
 //0...n-1列
    virtual const std::string GetStringField(int32_ nField, const std::string& szNullValue);
    virtual const std::string GetStringField(const std::string& szField, const std::string& szNullValue);
    virtual bool FieldIsNull(int32_ nField);
    virtual bool GieldIsNull(const std::string& szField);
    virtual bool Eof();
    virtual void NextRow();
    virtual void Finalize();
};

#endif

class Cx_DbMgr
    : public Ix_DbMgr
{
    X3BEGIN_CLASS_DECLARE(Cx_DbMgr)
        X3DEFINE_INTERFACE_ENTRY(Ix_DbMgr)
    X3END_CLASS_DECLARE()
protected:
	Cx_DbMgr();
	virtual ~Cx_DbMgr();

protected:
	virtual LWRESULT Init();
	virtual int32_ 	Open(const std::string& host, const std::string& user, const std::string& passwd, const std::string& db,
	 					   unsigned int32_ port, unsigned long_ client_flag);
	virtual void 	Close();
	/* 返回句柄 */
	virtual DBHandle 	GetDbHandle();
	/* 处理返回多行的查询，返回影响的行数 */
	//返回引用是因为在CppMySQLQuery的赋值构造函数中要把成员变量_mysql_res置为空
	//virtual CppMySQLQuery& QuerySQL(const std::string& sql);
	
	/* 执行非返回结果查询 */
	virtual int32_ 		ExecSQL(const std::string& sql);
	/* 测试mysql服务器是否存活 */
	virtual int32_ 		Ping();
	/* 关闭mysql 服务器 */
	virtual int32_ 		ShutDown();
	/* 主要功能:重新启动mysql 服务器 */
	virtual int32_ 		Reboot();
	/*
	* 说明:事务支持InnoDB or BDB表类型
	*/
	/* 主要功能:开始事务 */
	virtual int32_ 		StartTransaction();
	/* 主要功能:提交事务 */
	virtual int32_ 		Commit();
	/* 主要功能:回滚事务 */
	virtual int32_ 		Rollback();
	/* 得到客户信息 */
	virtual const std::string 	GetClientInfo();
	/* 主要功能:得到客户版本信息 */
	virtual const unsigned long_  GetClientVersion();
	/* 主要功能:得到主机信息 */
	virtual const std::string 	GetHostInfo();
	/* 主要功能:得到服务器信息 */
	virtual const std::string 	GetServerInfo();
	/*主要功能:得到服务器版本信息*/
	virtual const unsigned long_  GetServerVersion();
	/*主要功能:得到 当前连接的默认字符集*/
	virtual const std::string  	GetCharacterSetName();
	/* 得到系统时间 */
	virtual const std::string 	GetSysTime();
	/* 建立新数据库 */
	virtual int32_ 	CreateDB(const std::string& name);
	/* 删除制定的数据库*/
	virtual int32_ 	DropDB(const std::string& name);
	virtual bool_ 	TableExists(const std::string& table);
	virtual uint32_ LastRowId();
	virtual void 	SetBusyTimeout(int32_ nMillisecs);

protected:
	/* msyql 连接句柄 */
	MYSQL* mDb;
	//CppMySQLQuery _db_query;

	
};


LWDP_NAMESPACE_END;

#endif
