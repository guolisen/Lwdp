/*! \file Api4Linux.h
 *  \brief Included by LwApi.h to implement Linux Api.
 *  \author Guolisen, LwDp
 *  \date   2011.9.4
 */
#ifndef API_FOR_LINUX_H
#define API_FOR_LINUX_H



//#define PTW32_STATIC_LIB
//#include <LwApiLib/LwApi/win32/posix/pthread/pthread.h>
//#include <LwApiLib/LwApi/win32/posix/pthread/implement.h>
//#include <LwApiLib/LwApi/win32/posix/pthread/sched.h>
//#include <LwApiLib/LwApi/win32/posix/pthread/semaphore.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <LwDp.h>

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

#define LINUX_IMPL_API(api) LINUX_##api

#define __cdecl  
#define __stdcall 
#define SOCKET int
#define DECLEXPORT
#define OUTAPI      extern "C" DECLEXPORT
#define PLNEXT     __T".so"
#define PLUGINCALL  OUTAPI

#ifndef LWDP_MAX_PATH
#define LWDP_MAX_PATH 256
#endif

typedef void* MODULEID;
typedef void* HANDLE;
typedef void* HWND;

typedef int32_ DWORD;

#ifdef __cplusplus

typedef int         (*FUNCPTR) (...);  /* ptr to function returning int */

#else

typedef int         (*FUNCPTR) ();       /* ptr to function returning int */

#endif/* _cplusplus */

//typedef HMODULE MODULEID;

void LINUX_IMPL_API(TaskDelay)(ulong_ tick);
void LINUX_IMPL_API(HaltSystem)();
int  LINUX_IMPL_API(snprintf)( char *buffer, int num, const char *format, ... );

int portable_snprintf(char *str, size_t str_m, const char *fmt, /*args*/ ...);
long InterlockedIncrement(long* p);
long InterlockedDecrement(long* p);
bool FreeLibrary(MODULEID hdll);
void* GetProcAddress(MODULEID hdll, const char* name);
MODULEID LoadLibraryA(const char_* filename);


#define Lwdp_InterlockedIncrement    InterlockedIncrement
#define Lwdp_InterlockedDecrement    InterlockedDecrement
#define Lwdp_FreeLibrary             FreeLibrary
#define Lwdp_GetProcAddress          GetProcAddress
#define Lwdp_LoadLibrary             LoadLibraryA
//#define Lwdp_PathFindFileName        PathFindFileNameA

#define Lwdp_TaskDelay               LINUX_IMPL_API(TaskDelay)
#define Lwdp_HaltSystem              LINUX_IMPL_API(HaltSystem)

//#define Lwdp_sprintf
#if 1
#ifdef UNICODE
#define Lwdp_strlwr	      _wcslwr_s
#define Lwdp_strupr	      _wcsupr_s
#define Lwdp_strlen		  wcslen

#define Lwdp_isalnum      iswalnum 
#define Lwdp_isdigit 	  iswdigit
#define Lwdp_isalpha 	  iswalpha

#define Lwdp_fprintf      fwprintf
#define Lwdp_sprintf      swprintf_s
#define Lwdp_snprintf     
#define Lwdp_vstprintf    vswprintf

#define Lwdp_tcscpy       wcscpy_s
#define Lwdp_tcsncpy      wcsncpy_s
#define Lwdp_tcscat       wcscat_s
#define Lwdp_ltot         _ltow_s
#define Lwdp_itot         _itow_s
#define Lwdp_tcscmp 	  wcscmp
#define Lwdp_tcsncmp	  wcsncmp
#else
#define Lwdp_strlwr	  	  strlwr
#define Lwdp_strupr	  	  strupr
#define Lwdp_strlen		  strlen

#define Lwdp_isalnum      isalnum 
#define Lwdp_isdigit 	  isdigit
#define Lwdp_isalpha 	  isalpha

#define Lwdp_fprintf      fprintf
#define Lwdp_sprintf      sprintf
#define Lwdp_snprintf     portable_snprintf   //LINUX_IMPL_API(snprintf)
#define Lwdp_vstprintf    vsprintf

#define Lwdp_tcscpy       strcpy
//#define Lwdp_tcsncpy      strncpy
#define Lwdp_tcscat       strcat
#define Lwdp_ltot         ltoa
#define Lwdp_itot         itoa
#define Lwdp_tcscmp 	  strcmp
#define Lwdp_tcsncmp	  strcnmp
#define Lwdp_tprintf	  printf
#endif // UNICODE
#endif

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


