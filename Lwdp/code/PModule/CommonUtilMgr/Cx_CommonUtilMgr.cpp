/*! \file Cx_CommonUtilMgr.cpp
 *  \brief CommonUtilMgr Module Impl
 *  \author Guolisen, LwDp
 *  \date   2012.10.17
 */
 
#include <LwDp.h>
#include <PluginInc.h>
#include <iostream>
#include <iconv.h>

#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include <Interface/LogMgr/Ix_LogMgr.h>

#include "CommonUtilMgrDef.h"
#include "Cx_CommonUtilMgr.h"



LWDP_NAMESPACE_BEGIN;


Cx_CommonUtilMgr::Cx_CommonUtilMgr()
{
}

Cx_CommonUtilMgr::~Cx_CommonUtilMgr()
{
}

LWRESULT Cx_CommonUtilMgr::Init()
{
	lw_log_info(LWDP_LUA_LOG, __T("Cx_CommonUtilMgr::Init OK!"));

	srand(time(NULL));

	return LWDP_OK;
}

LWRESULT Cx_CommonUtilMgr::StrConvert(char_* from_charset,
					                      char_* to_charset,
					                      char * inbuf,
					                      int    inlen,
					                      char * outbuf,
					                      int    outlen)
{
	if(!from_charset || !to_charset || !inbuf || !outbuf || !inlen || !outlen)
		return LWDP_ERROR;

	iconv_t cd;
	int rc = 0;
	const char **pin  = (const char **)&inbuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset, from_charset);
	if(!cd) 
		return LWDP_ERROR;
	
	memset(outbuf, 0, outlen);
	if((rc=iconv(cd, pin, (size_t *)&inlen, pout, (size_t *)&outlen)) == -1)
	{
		iconv_close(cd);
		return LWDP_ERROR;
	}
	iconv_close(cd);

	return LWDP_OK;
}

LWDP_NAMESPACE_END;

