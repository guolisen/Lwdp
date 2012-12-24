/*! \file Api4Win32.h
 *  \brief Included by LwApi.h to implement Win32 Api.
 *  \author Guolisen, LwDp
 *  \date   2011.9.4
 */
#ifndef API_FOR_WIN32_H
#define API_FOR_WIN32_H

#ifdef _AFXDLL
#define VC_EXTRALEAN
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#define _AFX_ALL_WARNINGS
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdisp.h>        // MFC Automation classes
#endif
#else
#define  FD_SETSIZE 1024
#include <winsock2.h>
#define _WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#include <windows.h>        // Windows master include file
#endif // _AFXDLL

#include <shlwapi.h>        // Windows light-weight utility APIs
#ifdef _MSC_VER
#pragma comment(lib, "shlwapi.lib")
#endif

#define PTW32_STATIC_LIB
#include <LwApiLib/LwApi/win32/posix/pthread/pthread.h>
#include <LwApiLib/LwApi/win32/posix/pthread/implement.h>
#include <LwApiLib/LwApi/win32/posix/pthread/sched.h>
#include <LwApiLib/LwApi/win32/posix/pthread/semaphore.h>


LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

#define WIN_IMPL_API(api) WIN_##api

#define DECLEXPORT  __declspec(dllexport)
#define PLNEXT      __T(".dll")
#define PLUGINCALL  extern "C" DECLEXPORT

typedef int socklen_t;
typedef HMODULE MODULEID;

void WIN_IMPL_API(TaskDelay)(ulong_ tick);
void WIN_IMPL_API(HaltSystem)();


#define Lwdp_InterlockedIncrement    InterlockedIncrement
#define Lwdp_InterlockedDecrement    InterlockedDecrement
#define Lwdp_FreeLibrary             FreeLibrary
#define Lwdp_GetProcAddress          GetProcAddress
#define Lwdp_LoadLibrary             LoadLibrary
#define Lwdp_PathFindFileName        PathFindFileName

#define Lwdp_TaskDelay               WIN_IMPL_API(TaskDelay)
#define Lwdp_HaltSystem              WIN_IMPL_API(HaltSystem)

#ifdef UNICODE
#define Lwdp_strlwr	      _wcslwr_s
#define Lwdp_strupr	      _wcsupr_s
#define Lwdp_strlen		  wcslen

#define Lwdp_isalnum      iswalnum 
#define Lwdp_isdigit 	  iswdigit
#define Lwdp_isalpha 	  iswalpha

#define Lwdp_fprintf      fwprintf
#define Lwdp_sprintf     swprintf_s
#define Lwdp_snprintf     _snprintf
#define Lwdp_vstprintf    vswprintf

#define Lwdp_tcscpy       wcscpy_s
#define Lwdp_tcsncpy      wcsncpy_s
#define Lwdp_tcscat       wcscat_s
#define Lwdp_ltot         _ltow_s
#define Lwdp_itot         _itow_s
#define Lwdp_tcscmp 	  wcscmp
#define Lwdp_tcsncmp	  wcsncmp
#else
#define Lwdp_strlwr	  	  _strlwr_s
#define Lwdp_strupr	  	  _strupr_s
#define Lwdp_strlen		  strlen

#define Lwdp_isalnum      isalnum 
#define Lwdp_isdigit 	  isdigit
#define Lwdp_isalpha 	  isalpha

#define Lwdp_fprintf      fprintf
#define Lwdp_sprintf      sprintf
#define Lwdp_snprintf     _snprintf
#define Lwdp_vsnprintf    _vsnprintf
#define Lwdp_vstprintf    vsprintf

#define Lwdp_tcscpy       strcpy_s
#define Lwdp_tcsncpy      strncpy_s
#define Lwdp_tcscat       strcat_s
#define Lwdp_ltot         _ltoa_s
#define Lwdp_itot         _itoa_s
#define Lwdp_tcscmp 	  strcmp
#define Lwdp_tcsncmp	  strcnmp
#define Lwdp_tprintf	  printf
#endif // UNICODE

#define XEND_DEFINE_CLASS_SYS XEND_DEFINE_CLASS_DLL

//! End group of class factory registry and implement entry function of Win32DLL(USRDLL).
/*! Using this macro need include this file and XModuleImpl.h file.
    \ingroup _GROUP_PLUGIN_CORE_
    \see XEND_DEFINE_MODULE_MFCEXTDLL, XEND_DEFINE_MODULE_MFCDLL
*/
#if defined(_USRDLL) && defined(APIENTRY)
#define XEND_DEFINE_CLASS_DLL() \
        XEND_DEFINE_CLASS()    \
BOOL APIENTRY DllMain(HANDLE hModule, uint32_ dwReason, LPVOID)   \
{   \
	int result = 0; \
    if (dwReason == DLL_PROCESS_ATTACH) \
    {   \
	    result = pthread_win32_process_attach_np (); \
    }   \
    else if (dwReason == DLL_PROCESS_DETACH)    \
    {   \
		result = pthread_win32_process_detach_np (); \
    }   \
    return result;    \
}
#else
#define XEND_DEFINE_CLASS_DLL() \
        XEND_DEFINE_CLASS()
#endif // XEND_DEFINE_MODULE_DLL

//! End group of class factory registry and implement entry function of MFC Extension DLL.
/*! Using this macro need include this file and XModuleImpl.h file.
    \ingroup _GROUP_PLUGIN_CORE_
    \see XEND_DEFINE_MODULE_DLL, XEND_DEFINE_MODULE_MFCDLL
*/
#define XEND_DEFINE_MODULE_MFCEXTDLL() \
        XEND_DEFINE_MODULE()    \
static AFX_EXTENSION_MODULE MFCExtDLL = { NULL, NULL }; \
extern "C" int32_ APIENTRY \
DllMain(HINSTANCE hInstance, uint32_ dwReason, LPVOID)    \
{   \
    if (dwReason == DLL_PROCESS_ATTACH) \
    {   \
        if (!AfxInitExtensionModule(MFCExtDLL, hInstance))  \
            return 0;   \
        new CDynLinkLibrary(MFCExtDLL); \
        x3GetModuleInterface(NULL, MFCExtDLL.hModule);  \
    }   \
    else if (dwReason == DLL_PROCESS_DETACH)    \
    {   \
        x3GetCurrentModule()->ClearModuleItems();        \
        AfxTermExtensionModule(MFCExtDLL);  \
    }   \
    return 1;   \
}

//! End group of class factory registry and implement entry function of MFC regular DLL.
/*! This macro implements CPluginApp class.\n
    Using this macro need include this file and XModuleImpl.h file.
    \note Call the following statemant before call any MFC function: \n
            AFX_MANAGE_STATE(AfxGetStaticModuleState());
    \ingroup _GROUP_PLUGIN_CORE_
    \see XEND_DEFINE_MODULE_DLL, XEND_DEFINE_MODULE_MFCEXTDLL
*/
#define XEND_DEFINE_MODULE_MFCDLL() \
        XEND_DEFINE_MODULE()    \
class CPluginApp : public CWinApp   \
{   \
public: \
    CPluginApp()    \
    {   \
    }   \
    virtual BOOL InitInstance() \
    {   \
        CWinApp::InitInstance();    \
        x3GetModuleInterface(NULL, m_hInstance);    \
        return TRUE;    \
    }   \
    virtual int32_ ExitInstance()  \
    {   \
        x3GetCurrentModule()->ClearModuleItems();    \
        return CWinApp::ExitInstance(); \
    }   \
};



EXTERN_C_END;
LWDP_NAMESPACE_END;

#endif // API_FOR_WIN32_H

