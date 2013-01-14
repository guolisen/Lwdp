/*! \file   CtDef.h
 *  \brief  CtDef Tag Define
 *  \author Guolisen, Lwdp PluginFramework
 *  \date   2012.12.21  Doom Day!! 
 */
#ifndef LWDP_CHECKTICKET_DEFINE_H_
#define LWDP_CHECKTICKET_DEFINE_H_

#define LW_CT_DB_IP_DEFAULT 	__T("127.0.0.1")
#define LW_CT_DB_USER_DEFAULT   __T("root")
#define LW_CT_DB_PASSWORD       __T("lifesgood")
#define LW_CT_DB_SELECT_DBNAME  __T("test")
#define LW_CT_DB_PORT_DEFAULT  	3306
#define LW_CT_PAGESIZE_DEFAULT  	1000

#define LW_CT_CARD_SET_COUNT_DEFAULT  __T("SELECT count(*) as setcount \
								           FROM sc_swiping \
								           WHERE create_time >= DATE_FORMAT('%s','%%Y-%%m-%%d') AND \
								         	     create_time %s DATE_FORMAT('%s','%%Y-%%m-%%d')")


#define LW_CT_CARD_SET_DEFAULT  __T("SELECT card_no,scenic_id \
							         FROM sc_swiping \
							         WHERE create_time >= DATE_FORMAT('%s','%%Y-%%m-%%d') AND \
							         	   create_time %s DATE_FORMAT('%s','%%Y-%%m-%%d') \
							         LIMIT %d,%d")

#define LW_CT_SCENIC_TABLE_DEFAULT  __T("SELECT scenic_id \
								         FROM sc_card_scenic \
								         WHERE card_no = '%s'")

#define LW_CT_UPDATE_CARD_STATUS_DEFAULT  __T("UPDATE sc_card \
											    SET status = 5 \
											    WHERE card_no = '%s'")

#define LW_CT_SELECT_CARD_STATUS_DEFAULT  __T("SELECT * \
                                               FROM sc_card \
											    WHERE card_no = '%s'")

#define LW_CT_INSERT_CARD_STATUS_DEFAULT  __T("insert into sc_card  \
												(card_no, card_status) \
												values ('%s', 5)")


#define LW_CT_SCENIC_COL_DEFAULT  __T("scenic_id")
#define LW_CT_CARDID_COL_DEFAULT  __T("card_no")

#define LW_CT_MODULE_NAME  		__T("LWDP")

#define LW_CT_DB_IP_NAME  		__T("DbIp")
#define LW_CT_DB_USER_NAME  	__T("DbUser")
#define LW_CT_DB_PASSWORD_NAME  __T("DbPassword")
#define LW_CT_DB_SELECT_DB_NAME  __T("DbSelect")
#define LW_CT_DB_PORT_NAME  	__T("DbPort")


#define LW_CT_CARD_SET_COUNT_NAME  	__T("CardSetCount")
#define LW_CT_CARD_SET_NAME  	__T("CardSet")
#define LW_CT_SCENIC_SET_NAME  	__T("ScenicSet")

#define LW_CT_SCENIC_COL_NAME  __T("ScenicIdCol")
#define LW_CT_CARDID_COL_NAME  __T("CardIdCol")

#define LW_CT_UPDATE_CARD_NAME  __T("UpdateCard")
#define LW_CT_SELECT_CARD_NAME  __T("SelectCard")
#define LW_CT_INSERT_CARD_NAME  __T("InsertCard")

#define LW_CT_PROCESS_PAGE_NAME  __T("ProcessPageSize")


#endif // LWDP_CONFIG_TAG_DEFINE_H_






