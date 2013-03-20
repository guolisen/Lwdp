//#pragma once
#ifndef CX_ACDEVICE_H
#define CX_ACDEVICE_H

using namespace NLwdp;
#include <Interface/ConfigMgr/Ix_ConfigMgr.h>
#include "../Interface/ZmqBackend/Ix_ZmqBackend.h"
#include "../Interface/ACDevice/Ix_ACDevice.h"


class Cx_ACDevice: public Ix_ACDevice
{
    X3BEGIN_CLASS_DECLARE(Cx_ACDevice)
        X3DEFINE_INTERFACE_ENTRY(Ix_ACDevice)
    X3END_CLASS_DECLARE()
protected:
	Cx_ACDevice();
	virtual ~Cx_ACDevice();
public:
	virtual LWRESULT DeviceInitMsgProcess(DBHandle db_handle, const uint8_* ret_msg, uint32_ ret_msg_len, 
									      Data_Ptr& send_msg, uint32_& send_msg_len);

	virtual LWRESULT DeviceConfigMsgProcess(DBHandle db_handle,const uint8_* ret_msg, uint32_ ret_msg_len, 
									      Data_Ptr& send_msg, uint32_& send_msg_len);

	virtual LWRESULT DeviceHBMsgProcess(DBHandle db_handle,const uint8_* ret_msg, uint32_ ret_msg_len, 
									      Data_Ptr& send_msg, uint32_& send_msg_len);

	virtual LWRESULT DeviceCardDataMsgProcess(DBHandle db_handle,const uint8_* ret_msg, uint32_ ret_msg_len, 
									      Data_Ptr& send_msg, uint32_& send_msg_len);

	virtual LWRESULT DeviceBulkDataMsgProcess(DBHandle db_handle,const uint8_* ret_msg, uint32_ ret_msg_len, 
									      Data_Ptr& send_msg, uint32_& send_msg_len);

public:
	static std::string mInitSql;
	static std::string mConfigSql;
	static std::string mCardSql;
	static XPropertyTable gateInfoTable;

protected:
	virtual LWRESULT getCardStatus(DBHandle db_handle,
								    const std::string& carIdStr, 
	                                const std::string& sceneryIdStr,
							        int32_& statusCode,
							        char_** retMsg);
	virtual LWRESULT cardCheckIn(DBHandle db_handle,
								    const std::string& carIdStr, 
	                                const std::string& sceneryIdStr,
							        int32_& statusCode,
							        char_** retMsg);

	virtual LWRESULT cardCheckOut(DBHandle db_handle,
								      const std::string& carIdStr, 
	                                  const std::string& sceneryIdStr,
							          int32_& statusCode,
							          char_** retMsg);
	
	virtual LWRESULT intArrayToStr(uint32_ int_array[], uint32_ size, std::string& ret_str);


	
protected:
    // From Ix_ACDevice
    virtual LWRESULT Init();

};

#endif
