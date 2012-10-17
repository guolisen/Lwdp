/*! \file LwLogErrno.h
 *  \brief Log Module Errno Define
 *  \author Guolisen, LwDp
 *  \date   2011.08.12
 */


ERRNO_DB (PLUGINMGR_CLSID_ERROR,            	(M_pluginloader | 1), "CLSID is invaild!")
ERRNO_DB (PLUGINMGR_LOADMODULE_ERROR,       	(M_pluginloader | 2), "Load Module is Error!")
ERRNO_DB (PLUGINMGR_MODULE_ALREADY_LOAD,    	(M_pluginloader | 3), "Plugin Has Already Load!")
ERRNO_DB (PLUGINMGR_MODULE_INDEX_ERROR,     	(M_pluginloader | 4), "Plugin Index Error!")
ERRNO_DB (PLUGINMGR_NOT_FIND_MODULE_ENTRY,  	(M_pluginloader | 5), "Can not Find the Module Entry in Global Module Table!")
ERRNO_DB (PLUGINMGR_NOT_FIND_MODULE, 			(M_pluginloader | 6), "Can not Find the Module Entry in PluginLoader!")
ERRNO_DB (PLUGINMGR_MODULE_USING, 				(M_pluginloader | 7), "MODULE is Using By Other!")
ERRNO_DB (PLUGINMGR_PLUGINVERSION_IS_HIGH,  	(M_pluginloader | 8), "Plugin Version is so High!")
ERRNO_DB (PLUGINMGR_PLUGIN_UNICODE_IS_DIFF, 	(M_pluginloader | 9), "Plugin's Unicode is different from Framwork!")
ERRNO_DB (PLUGINMGR_PLUGIN_TARGET_LEVEL_HIGH, 	(M_pluginloader | 10), "Target Object's Level is Higher than Local Object, When Create Object!")
ERRNO_DB (PLUGINMGR_PLUGIN_TARGET_DOMAIN_ERROR, (M_pluginloader | 11), "Target Object's Domain is different from Local Domain, When Create Object!")





