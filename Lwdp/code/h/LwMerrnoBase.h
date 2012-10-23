/*! \file LwMerrnoBase.h
 *  \brief Base Error No. Define
 *  \author Guolisen, LwDp
 *  \date   2011.08.12
 */

//#ifndef M_submodule
//    #define M_submodule  0
//#endif

ERRNO_DB (OK,                     (0),               "Ok haha~~~")
ERRNO_DB (ERROR,                  (-1),              "Common Error")
ERRNO_DB (MALLOC_MEMORY_ERROR,    (M_errno | 0 | 2), "Memory is NOT Enough")
ERRNO_DB (PARAMETER_ERROR,        (M_errno | 0 | 3), "Function Parameter Error")
ERRNO_DB (POINTER_IS_NULL,        (M_errno | 0 | 4), "Pointer is Null")
ERRNO_DB (OBJECT_POINTER_IS_NULL, (M_errno | 0 | 5), "Get Object Pointer is Null")
ERRNO_DB (GET_OBJECT_ERROR,       (M_errno | 0 | 6), "Get Object Pointer From Cx_Interface<> is Null")



#include <LwApiLib/Framwork/FrameworkErrno.h> 
#include <LwApiLib/LwApi/LwApiErrno.h> 
#include <../PModule/PmoduleErrno.h> 

//#include <LwApiLib/Framwork/PluginMgr/PluginMgrErrno.h> 


 


