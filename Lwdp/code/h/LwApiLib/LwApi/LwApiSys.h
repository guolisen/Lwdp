/*! \file LwApiCommon.h
 *  \brief Common Api
 *  \author Guolisen, LwDp
 *  \date   2011.07.14
 */

#ifndef LW_Api_SYSTEM_H_
#define LW_Api_SYSTEM_H_
 
LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

#ifdef UNICODE
#ifdef __GNUC__
#include <wchar.h>
#include <wctype.h>
#endif

inline wchar_ * _co_wcslwr_s(wchar_ *str);
inline wchar_ * _co_wcsupr_s(wchar_ *str);
inline int32_ 		_co_wcscat_s(wchar_ *str, size_t, const wchar_ *src);
inline int32_ 		_co_wcsncpy_s(wchar_ *str, size_t size, const wchar_ *src, size_t len);
inline int32_ 		_co_wcscpy_s(wchar_ *str, size_t size, const wchar_ *src);
inline int32_ 		_co_wcsncmp_s(wchar_ *str, const wchar_ *src, int32_ size);
inline int32_ 		_co_wcscmp_s(wchar_ *str, const wchar_ *src);

#if defined(_STDIO_DEFINED)     // stdio.h
inline int32_ _co_swprintf_s(wchar_ *buffer, size_t size, const wchar_ *format, ...);
inline int32_ _co_vswprintf_s(wchar_ *buffer, size_t size, const wchar_ *format, va_list arglist);
#endif // _INC_STDIO


#if defined(_WIN32)
inline int32_ _co_ltow_s(long_ value, wchar_ *str, size_t, int32_ radix);
inline int32_ _co_itow_s(int32_ value, wchar_ *str, size_t, int32_ radix);
inline int32_ _co_ultow_s(unsigned long_ value, wchar_ *str, size_t, int32_ radix);

#elif defined(_STDIO_DEFINED)
;
inline int32_ _co_ltow_s(long_ value, wchar_ *str, size_t size, int32_ radix);
inline int32_ _co_itow_s(int32_ value, wchar_ *str, size_t size, int32_ radix);
inline int32_ _co_ultow_s(unsigned long_ value, wchar_ *str, size_t size, int32_ radix);

#endif

#if defined(_INC_STDLIB) || defined(_STDLIB_H_)

inline int32_ _co_wsplitpath_s(
    const wchar_ * path, wchar_ * drive, size_t,
    wchar_ * dir, size_t,
    wchar_ * fname, size_t,
    wchar_ * ext,  size_t);

inline int32_ _co_wmakepath_s(wchar_ *path, size_t,
    const wchar_ *drive, const wchar_ *dir, const wchar_ *fname, const wchar_ *ext);

#endif // _INC_STDLIB

/////////////////////////////////////////////////////////////
#else  //UNICODE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

inline char_ * 	_co_strlwr_s(char_ *str);
inline char_ * 	_co_strupr_s(char_ *str);
inline int32_ 		_co_strcat_s(char_ *str, size_t, const char_ *src);
inline int32_ 		_co_strncpy_s(char_ *str, size_t size, const char_ *src, size_t len);
inline int32_ 		_co_strcpy_s(char_ *str, size_t size, const char_ *src);
inline int32_ 		_co_strcnmp_s(char_ *str, const char_ *src, int32_ size);
inline int32_ 		_co_strcmp_s(char_ *str, const char_ *src);

#if defined(_STDIO_DEFINED)     // stdio.h
inline int32_ 		_co_sprintf_s(char_ *buffer, size_t, const char_ *format, ...);
inline int32_ 		_co_vsprintf_s(char_ *buffer, size_t, const char_ *format, va_list arglist);
#endif // _INC_STDIO

#if defined(_WIN32)
inline int32_ 		_co_ltoa_s(long_ value, char_ *str, size_t, int32_ radix);
inline int32_ 		_co_itoa_s(int32_ value, char_ *str, size_t, int32_ radix);

#elif defined(_STDIO_DEFINED)
inline int32_ 		_co_ltoa_s(long_ value, char_ *str, size_t size, int32_ radix);
inline int32_ 		_co_itoa_s(int32_ value, char_ *str, size_t size, int32_ radix);
#endif

#if defined(_INC_STDLIB) || defined(_STDLIB_H_)
inline int32_ _co_splitpath_s(
    const char_ * path, char_ * drive, size_t,
    char_ * dir, size_t,
    char_ * fname, size_t,
    char_ * ext,  size_t);

inline int32_ _co_makepath_s(char_ *path, size_t,
    const char_ *drive, const char_ *dir, const char_ *fname, const char_ *ext);

#endif // _INC_STDLIB
#endif // UNICODE


#ifdef _INC_TIME        // time.h
inline void localtime_s(struct tm *tmOut, const time_t *timer);
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
wchar_*	Co_PathFindFileNameA(const wchar_* path);
bool	Co_PathIsRelativeA(const wchar_* path);
void	Co_PathStripPathA(wchar_* path);
void	Co_PathRemoveFileSpecA(wchar_* path);
void	Co_PathRemoveExtensionA(wchar_* path);
void	Co_PathRemoveBackslashA(wchar_* path);
void	Co_PathAppendA(wchar_* path, const wchar_* more);
wchar_*	Co_PathAddBackslashA(wchar_* path);
void	Co_PathRenameExtensionA(wchar_* path, const wchar_* more);


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



EXTERN_C_END;
LWDP_NAMESPACE_END;
#endif




 


