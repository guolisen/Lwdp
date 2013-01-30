/*! \file LwApiCommon.h
 *  \brief Common Api
 *  \author Guolisen, LwDp
 *  \date   2011.07.14
 */

#ifndef LW_Api_SYSTEM_H_
#define LW_Api_SYSTEM_H_
#include <LwDp.h>
#include <stdarg.h>

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;


#ifdef UNICODE
#ifdef __GNUC__
#include <wchar.h>
#include <wctype.h>
#endif

inline wchar_ * _co_wcslwr_s(wchar_ *str)
    { for (wchar_* p = str; *p; p++) towlower(*p); return str; }
inline wchar_ * _co_wcsupr_s(wchar_ *str)
    { for (wchar_* p = str; *p; p++) towupper(*p); return str; }


inline int32_ _co_wcscat_s(wchar_ *str, size_t, const wchar_ *src)
    { return wcscat(str, src) ? 0 : -1; }

inline int32_ _co_wcsncpy_s(wchar_ *str, size_t size, const wchar_ *src, size_t len)
    { return wcsncpy(str, src, min(size, len)) ? 0 : -1; }


inline int32_ _co_wcscpy_s(wchar_ *str, size_t size, const wchar_ *src)
    { return wcsncpy(str, src, size) ? 0 : -1; }


inline int32_ _co_wcsncmp_s(wchar_ *str, const wchar_ *src, int32_ size)
    { return wcsncpy(str, src, size) ? 0 : -1; }


inline int32_ _co_wcscmp_s(wchar_ *str, const wchar_ *src)
    { return wcscpy(str, src) ? 0 : -1; }


#if defined(_STDIO_DEFINED)     // stdio.h
inline int32_ _co_swprintf_s(wchar_ *buffer, size_t size, const wchar_ *format, ...)
{
    va_list arglist;
    va_start(arglist, format);
#if !defined(_WIN32)
    return vswprintf(buffer, size, format, arglist);
#else
    size; return vswprintf(buffer, format, arglist);
#endif
}
inline int32_ _co_vswprintf_s(wchar_ *buffer, size_t size, const wchar_ *format, va_list arglist)
#if !defined(_WIN32)
    { return vswprintf(buffer, size, format, arglist); }
#else
    { size; return vswprintf(buffer, format, arglist); }
#endif

#endif // _INC_STDIO


#if defined(_WIN32)

inline int32_ _co_ltow_s(long_ value, wchar_ *str, size_t, int32_ radix)
    { _ltow(value, str, radix); return errno; }
inline int32_ _co_itow_s(int32_ value, wchar_ *str, size_t, int32_ radix)
    { _itow(value, str, radix); return errno; }
inline int32_ _co_ultow_s(unsigned long_ value, wchar_ *str, size_t, int32_ radix)
    { _ultow(value, str, radix); return errno; }

#elif defined(_STDIO_DEFINED)

inline int32_ _co_ltow_s(long_ value, wchar_ *str, size_t size, int32_ radix)
    { swprintf_s(str, size, 16 == radix ?__T("%lx") :__T("%ld"), value); return 0; }
inline int32_ _co_itow_s(int32_ value, wchar_ *str, size_t size, int32_ radix)
    { return _ltow_s(value, str, size, radix); }
inline int32_ _co_ultow_s(unsigned long_ value, wchar_ *str, size_t size, int32_ radix)
    { swprintf_s(str, size, 16 == radix ?__T("%ulx") :__T("%uld"), value); return 0; }

#endif

#if defined(_INC_STDLIB) || defined(_STDLIB_H_)

inline int32_ _co_wsplitpath_s(
    const wchar_ * path, wchar_ * drive, size_t,
    wchar_ * dir, size_t,
    wchar_ * fname, size_t,
    wchar_ * ext,  size_t)
{
    _wsplitpath(path, drive, dir, fname, ext);
    return errno;
}

inline int32_ _co_wmakepath_s(wchar_ *path, size_t,
    const wchar_ *drive, const wchar_ *dir, const wchar_ *fname, const tchar_ *ext)
    { _wmakepath(path, drive, dir, fname, ext); return errno; }

#endif // _INC_STDLIB

/////////////////////////////////////////////////////////////
#else  //UNICODE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

inline char_* _co_strlwr_s(char_ *str)
{
	//for (char_* p = str; *p; p++)
	//	tolower(*p);
	return str;
}

inline char_ * _co_strupr_s(char_ *str)
{
	//for (char_* p = str; *p; p++)
	//	toupper(*p);
	return str;
}

inline int32_ _co_strcat_s(char_ *str, size_t, const char_ *src)
    { return strcat(str, src) ? 0 : -1; }

inline int32_ _co_strncpy_s(char_ *str, size_t size, const char_ *src, size_t len)
{ return strncpy(str, src, size<len?size:len) ? 0 : -1; }


inline int32_ _co_strcpy_s(char_ *str, size_t size, const char_ *src)
    { return strncpy(str, src, size) ? 0 : -1; }

inline int32_ _co_strcnmp_s(char_ *str, const char_ *src, int32_ size)
    { return strncmp(str, src, size) ? 0 : -1; }


inline int32_ _co_strcmp_s(char_ *str, const char_ *src)
    { return strcmp(str, src) ? 0 : -1; }


#if defined(_STDIO_DEFINED)     // stdio.h

inline int32_ _co_sprintf_s(char_ *buffer, size_t, const char_ *format, ...)
{
    va_list arglist;
    va_start(arglist, format);
    return vsprintf(buffer, format, arglist);
}
inline int32_ _co_vsprintf_s(char_ *buffer, size_t, const char_ *format, va_list arglist)
    { return vsprintf(buffer, format, arglist); }

#endif // _INC_STDIO


#if defined(_WIN32)
inline int32_ _co_ltoa_s(long_ value, char_ *str, size_t, int32_ radix)
    { _ltoa(value, str, radix); return errno; }
inline int32_ _co_itoa_s(int32_ value, char_ *str, size_t, int32_ radix)
    { _itoa(value, str, radix); return errno; }

#elif defined(_STDIO_DEFINED)
inline int32_ _co_ltoa_s(long_ value, char_ *str, size_t size, int32_ radix)
    { _co_sprintf_s(str, size, 16 == radix ? "%lx" : "%ld", value); return 0; }
inline int32_ _co_itoa_s(int32_ value, char_ *str, size_t size, int32_ radix)
    { return _co_ltoa_s(value, str, size, radix); }

#endif

#if defined(_INC_STDLIB) || defined(_STDLIB_H_)

inline int32_ _co_splitpath_s(
    const char_ * path, char_ * drive, size_t,
    char_ * dir, size_t,
    char_ * fname, size_t,
    char_ * ext,  size_t)
{
    _splitpath(path, drive, dir, fname, ext);
    return errno;
}


inline int32_ _co_makepath_s(char_ *path, size_t,
    const char_ *drive, const char_ *dir, const char_ *fname, const char_ *ext)
    { _makepath(path, drive, dir, fname, ext); return errno; }

#endif // _INC_STDLIB
#endif // UNICODE



#ifdef _INC_TIME        // time.h
#if 0
inline void localtime_s(struct tm *tmOut, const time_t *timer)
{
    struct tm * p = localtime(timer);
    if (tmOut != NULL && p != NULL)
        *tmOut = *p;
}
#endif
#endif // _INC_TIME

#define _sopen_s(fileHandler, filename, oflag, pmode, rw)   \
    (*fileHandler = _open(filename, oflag, pmode), errno)








#ifdef UNICODE

#ifndef Lwdp_strlwr
#define Lwdp_strlwr		 _co_wcslwr_s
#endif
#ifndef Lwdp_strupr
#define Lwdp_strupr		 _co_wcsupr_s
#endif
#ifndef Lwdp_stprintf
#define Lwdp_stprintf     _co_swprintf_s
#endif
#ifndef Lwdp_vstprintf
#define Lwdp_vstprintf    _co_vswprintf_s
#endif
#ifndef Lwdp_tcscpy
#define Lwdp_tcscpy       _co_wcscpy_s
#endif
#ifndef Lwdp_tcsncpy
#define Lwdp_tcsncpy      _co_wcsncpy_s
#endif
#ifndef Lwdp_tcscat
#define Lwdp_tcscat       _co_wcscat_s
#endif
#ifndef Lwdp_tsplitpath
#define Lwdp_tsplitpath   _co_wsplitpath_s
#endif
#ifndef Lwdp_tmakepath
#define Lwdp_tmakepath    _co_wmakepath_s
#endif
#ifndef Lwdp_ltot
#define Lwdp_ltot         _co_ltow_s
#endif
#ifndef Lwdp_itot
#define Lwdp_itot         _co_itow_s
#endif
#ifndef Lwdp_tcscmp
#define Lwdp_tcscmp 		 _co_wcscmp_s
#endif
#ifndef Lwdp_tcsncmp
#define Lwdp_tcsncmp		 _co_wcsncmp_s
#endif

#else

#ifndef Lwdp_strlwr
#define Lwdp_strlwr		_co_strlwr_s
#endif
#ifndef Lwdp_strupr
#define Lwdp_strupr		_co_strupr_s
#endif
#ifndef Lwdp_stprintf
#define Lwdp_stprintf     _co_sprintf_s
#endif
#ifndef Lwdp_vstprintf
#define Lwdp_vstprintf    _co_vsprintf_s
#endif
#ifndef Lwdp_tcscpy
#define Lwdp_tcscpy       _co_strcpy_s
#endif
#ifndef Lwdp_tcsncpy
#define Lwdp_tcsncpy      _co_strncpy_s
#endif
#ifndef Lwdp_tcscat
#define Lwdp_tcscat       _co_strcat_s
#endif
#ifndef Lwdp_tsplitpath
#define Lwdp_tsplitpath   _co_splitpath_s
#endif
#ifndef Lwdp_tmakepath
#define Lwdp_tmakepath    _co_makepath_s
#endif
#ifndef Lwdp_ltot
#define Lwdp_ltot         _co_ltoa_s
#endif
#ifndef Lwdp_itot
#define Lwdp_itot         _co_itoa_s
#endif
#ifndef Lwdp_tcscmp
#define Lwdp_tcscmp 		_co_strcmp_s
#endif
#ifndef Lwdp_tcsncmp
#define Lwdp_tcsncmp		_co_strcnmp_s
#endif


#endif // UNICODE



#ifndef GET_WHEEL_DELTA_WPARAM
#define GET_WHEEL_DELTA_WPARAM(wParam)  ((short)HIWORD(wParam))
#endif

#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof(array[0]))
#endif



//==================================================
//                Path Function Api 
//==================================================
char_*	Co_PathFindFileNameA(const char_* path);
char_*	Co_PathFindFileNameA(const char_* path);
bool	Co_PathIsRelativeA(const char_* path);
void	Co_PathStripPathA(char_* path);
void	Co_PathRemoveFileSpecA(char_* path);
void	Co_PathRemoveExtensionA(char_* path);
void	Co_PathRemoveBackslashA(char_* path);
void	Co_PathAppendA(char_* path, const char_* more);
char_*	Co_PathAddBackslashA(char_* path);
void	Co_PathRenameExtensionA(char_* path, const char_* more);


wchar_*	Co_PathFindFileNameW(const wchar_* path);
bool	Co_PathIsRelativeW(const wchar_* path);
void	Co_PathStripPathW(wchar_* path);
void	Co_PathRemoveFileSpecW(wchar_* path);
void	Co_PathRemoveExtensionW(wchar_* path);
void	Co_PathRemoveBackslashW(wchar_* path);
void	Co_PathAppendW(wchar_* path, const wchar_* more);
wchar_*	Co_PathAddBackslashW(wchar_* path);
void	Co_PathRenameExtensionW(wchar_* path, const wchar_* more);



#ifdef _UNICODE

#ifndef Lwdp_PathFindFileName
#define Lwdp_PathFindFileName        Co_PathFindFileNameW
#endif

#ifndef Lwdp_PathIsRelative
#define Lwdp_PathIsRelative          Co_PathIsRelativeW
#endif

#ifndef Lwdp_PathStripPath
#define Lwdp_PathStripPath           Co_PathStripPathW
#endif

#ifndef Lwdp_PathRemoveFileSpec
#define Lwdp_PathRemoveFileSpec      Co_PathRemoveFileSpecW
#endif

#ifndef Lwdp_PathRemoveExtension
#define Lwdp_PathRemoveExtension     Co_PathRemoveExtensionW
#endif

#ifndef Lwdp_PathRemoveBackslash
#define Lwdp_PathRemoveBackslash     Co_PathRemoveBackslashW
#endif

#ifndef Lwdp_PathAppend
#define Lwdp_PathAppend              Co_PathAppendW
#endif

#ifndef Lwdp_PathAddBackslash
#define Lwdp_PathAddBackslash        Co_PathAddBackslashW
#endif

#ifndef Lwdp_PathRenameExtension
#define Lwdp_PathRenameExtension     Co_PathRenameExtensionW
#endif

#else
#ifndef Lwdp_PathFindFileName
#define Lwdp_PathFindFileName        Co_PathFindFileNameA
#endif

#ifndef Lwdp_PathIsRelative
#define Lwdp_PathIsRelative          Co_PathIsRelativeA
#endif

#ifndef Lwdp_PathStripPath
#define Lwdp_PathStripPath           Co_PathStripPathA
#endif

#ifndef Lwdp_PathRemoveFileSpec
#define Lwdp_PathRemoveFileSpec      Co_PathRemoveFileSpecA
#endif

#ifndef Lwdp_PathRemoveExtension
#define Lwdp_PathRemoveExtension     Co_PathRemoveExtensionA
#endif

#ifndef Lwdp_PathRemoveBackslash
#define Lwdp_PathRemoveBackslash     Co_PathRemoveBackslashA
#endif

#ifndef Lwdp_PathAppend
#define Lwdp_PathAppend              Co_PathAppendA
#endif

#ifndef Lwdp_PathAddBackslash
#define Lwdp_PathAddBackslash        Co_PathAddBackslashA
#endif

#ifndef Lwdp_PathRenameExtension
#define Lwdp_PathRenameExtension     Co_PathRenameExtensionA
#endif
#endif

#ifndef Lwdp_TaskDelay
We Need Implyment the Function Api_TaskDelay() In Concrete System;
#endif

#ifndef Lwdp_HaltSystem
We Need Implyment the Function Api_HaltSystem() In Concrete System;
#endif

#ifndef Lwdp_InterlockedIncrement
We Need Implyment the Function Lwdp_InterlockedIncrement() In Concrete System;
#endif

#ifndef Lwdp_InterlockedDecrement
We Need Implyment the Function Lwdp_InterlockedDecrement() In Concrete System;
#endif
#ifndef Lwdp_FreeLibrary
We Need Implyment the Function Lwdp_FreeLibrary() In Concrete System;
#endif

#ifndef Lwdp_GetProcAddress
We Need Implyment the Function Lwdp_GetProcAddress() In Concrete System;
#endif
#ifndef Lwdp_LoadLibrary
We Need Implyment the Function Lwdp_LoadLibrary() In Concrete System;
#endif


EXTERN_C_END;
LWDP_NAMESPACE_END;
#endif




 


