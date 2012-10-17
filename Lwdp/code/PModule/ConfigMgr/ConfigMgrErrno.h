/*! \file ConfigMgrErrno.h
 *  \brief ConfigMgr Module Errno Define
 *  \author Guolisen, LwDp
 *  \date   2011.08.12
 */


ERRNO_DB (GET_CONFIG_DATA_ERROR,            (M_configmgr | 1), "Can not Get Config Data From Config Srouce!")
ERRNO_DB (CONFIG_SRC_EXIST_ERROR,           (M_configmgr | 2), "Config Srouce Has Exist!")
ERRNO_DB (PARSE_XML_DATA_ERROR,             (M_configmgr | 3), "Parse Xml Data Error!")
ERRNO_DB (CONFIG_FILE_FORMAT_ERROR, 		(M_configmgr | 4), "Config File Format Error!")
ERRNO_DB (CANNOT_FIND_CONFIG_ERROR,         (M_configmgr | 5), "Can't Find Require Config Data!")


ERRNO_DB (XMLMGR_XML_DOC_ERROR, 			(M_configmgr | 6), "XmlMgr Docment Need Init!")
ERRNO_DB (XMLMGR_ELEMENT_FIND_ERROR, 		(M_configmgr | 7), "XmlMgr Element Tag Find Error!")
ERRNO_DB (XMLMGR_DOCUMENT_EMPTY_ERROR, 		(M_configmgr | 8), "XmlMgr Docment is Empty(Need read file) Error!")









