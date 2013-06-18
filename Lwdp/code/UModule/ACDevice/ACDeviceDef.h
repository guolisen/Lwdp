
#ifndef _AC_DEVICE_DEF_H
#define _AC_DEVICE_DEF_H


#define LW_ACDEVICE_BULK_PAGESIZE_DEFAULT    100
#define LW_ACDEVICE_THREAD_NUM_DEFAULT       5
#define LW_ACDEVICE_SELECT_DEVICE_DEFAULT  __T("select * from gate_info where gate_id =")

#define LW_ACDEVICE_CONFIG_DEVICE_DEFAULT  __T("select * from gate_info where gate_id =")
#define LW_ACDEVICE_INSERT_CARD_DATA_DEFAULT  __T("insert into card_swipecard(card_id,  \
													                          scenery_id,  \
													                          gate_id,  \
													                          card_type,  \
													                          action,  \
													                          checkin_time) VALUES")


#define LW_ACDEVICE_MODULE_NAME         __T("ACDevice")
#define LW_ACDEVICE_SELECT_FOR_INIT     __T("InitSql")
#define LW_ACDEVICE_CONFIG_SQL_STR      __T("ConfigSql")
#define LW_ACDEVICE_CARD_SQL_STR        __T("CardSql")

#define LW_ACDEVICE_GATE_INFO_TABLE     __T("GateInfoCol")

//#define LW_ACDEVICE_GATE_INFO_GATE_ID_COL         __T("GateIdCol")
#define LW_ACDEVICE_GATE_INFO_SCENERY_ID_COL	  __T("SceneryIdCol")	
//#define LW_ACDEVICE_GATE_INFO_MANUFACTURER_COL    __T("ManufacturerCol")
#define LW_ACDEVICE_GATE_INFO_POSITION_COL        __T("PositionCol")
#define LW_ACDEVICE_GATE_INFO_TYPE_COL            __T("typeCol")

#define LW_ACDEVICE_BULK_THREAD_NUM_NAME          __T("BulkThreadNum")
#define LW_ACDEVICE_BULK_PAGESIZE_NAME            __T("BulkPageSize")


#define LW_ACDEVICE_CARD_STATUS_NOTUSE     0
#define LW_ACDEVICE_CARD_STATUS_USE        1
#define LW_ACDEVICE_CARD_STATUS_ABANDON    3
#define LW_ACDEVICE_CARD_STATUS_FREEZE     4

#define LW_ACDEVICE_CARD_STATUS_NOTRELEASE      0
#define LW_ACDEVICE_CARD_STATUS_RELEASE        	1
#define LW_ACDEVICE_CARD_STATUS_LOST   		    2
#define LW_ACDEVICE_CARD_STATUS_BROKEN     		3
#define LW_ACDEVICE_CARD_STATUS_BAN     		4
#define LW_ACDEVICE_CARD_STATUS_DISUSE     		5
#define LW_ACDEVICE_CARD_STATUS_AD_BAN     		6

#define LW_ACDEVICE_CARD_TYPE_M1            1
#define LW_ACDEVICE_CARD_TYPE_2D        	2
#define LW_ACDEVICE_CARD_TYPE_ID   		    3

#define LW_ACDEVICE_TICKET_TYPE_POS         6
#define LW_ACDEVICE_TICKET_TYPE_LENGTH      2
#define LW_ACDEVICE_TICKET_TYPE_ONE         1
#define LW_ACDEVICE_TICKET_TYPE_SUIT        2
#define LW_ACDEVICE_TICKET_TYPE_YEAR   		3

#define GET_CARD_TAB(card_type, tab) \
	switch(card_type) \
	{ \
		case LW_ACDEVICE_CARD_TYPE_M1: \
			tab = "card_no"; \
			break; \
		case LW_ACDEVICE_CARD_TYPE_2D: \
			tab = "two_code"; \
			break; \
		case LW_ACDEVICE_CARD_TYPE_ID: \
			tab = "identity_id"; \
			break; \
		default: \
			tab = "card_no"; \
	};
	
#define GET_TICKET_TAB(ticket_type, tab) \
	switch(ticket_type) \
	{ \
		case LW_ACDEVICE_TICKET_TYPE_ONE: \
			tab = "sc_sale_scenic"; \
			break; \
		case LW_ACDEVICE_TICKET_TYPE_SUIT: \
			tab = "sc_package_ticket_map"; \
			break; \
		case LW_ACDEVICE_TICKET_TYPE_YEAR: \
			tab = "sc_annual_ticket_scenery_map"; \
			break; \
		default: \
			tab = "sc_sale_scenic"; \
	};
	
#define GET_SWIPING_TAB(ticket_type, tab) \
	switch(ticket_type) \
	{ \
		case LW_ACDEVICE_TICKET_TYPE_ONE: \
			tab = "sc_swiping"; \
			break; \
		case LW_ACDEVICE_TICKET_TYPE_SUIT: \
			tab = "sc_annual_ticket_swiping"; \
			break; \
		case LW_ACDEVICE_TICKET_TYPE_YEAR: \
			tab = "sc_package_ticket_swiping"; \
			break; \
		default: \
			tab = "sc_swiping"; \
	};
	
#endif

