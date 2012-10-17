// Copyright (C), 2006гн2011, Potevio Institute of Technology.
/************************************************************
*  File name:   LwApiCommon.cpp  	
*  Author:      guolisen
*  Version:   	v1.0
*  Create Date: 2011-08-11
***********************************************************/
// 2011.08.11: Create By Guolisen

#include <LwDp.h>
#include <Platform/LwApi/LwApi.h>

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

static inline bool IsPathSlash(tchar_ c)
{
    return L'\\' == c || L'/' == c;
}

tchar_* Co_PathFindFileNameA(const tchar_* path)
{
    const tchar_* p1 = path ? strrchr(path, '\\') : NULL;
    const tchar_* p2 = path ? strrchr(path, '/') : NULL;

    p1 = !p1 || (p2 && p2 > p1) ? p2 : p1;

    return const_cast<tchar_*>(p1 ? p1 + 1 : p1);
}

#ifdef _UNICODE
tchar_* Co_PathFindFileNameW(const tchar_* path)
{
    const tchar_* p1 = path ? wcsrchr(path, L'\\') : NULL;
    const tchar_* p2 = path ? wcsrchr(path, L'/') : NULL;

    p1 = !p1 || (p2 && p2 > p1) ? p2 : p1;

    return const_cast<tchar_*>(p1 ? p1 + 1 : path);
}

bool Co_PathIsRelativeW(const tchar_* path)
{
    return wcsrchr(path, L':') == NULL && path[0] != L'\\' && path[0] != L'/';
}

void Co_PathStripPathW(tchar_* path)
{
    Co_PathRemoveBackslashW(path);
    tchar_* filename = Co_PathFindFileNameW(path);

    if (filename && filename > path)
    {
        while (*filename)
        {
            *path++ = *filename++;
        }
        *path = *filename;
    }
}

void Co_PathRemoveFileSpecW(tchar_* path)
{
    Co_PathRemoveBackslashW(path);
    tchar_* filename = Co_PathFindFileNameW(path);

    if (filename && filename > path)
    {
        *(filename - 1) = 0;
    }
}

void Co_PathRemoveExtensionW(tchar_* path)
{
    tchar_* dot = wcsrchr(path, L'.');

    if (dot)
    {
        *dot = 0;
    }
}

void Co_PathRemoveBackslashW(tchar_* path)
{
    int32_ len = wcslen(path);

    if (len > 0 && Co_IsPathSlash(path[len - 1]))
    {
        path[len - 1] = 0;
    }
}

void Co_PathAppendW(tchar_* path, const tchar_* more)
{
    ASSERT(path && more);
    int32_ len = wcslen(path);

    if (len > 0 && !Co_IsPathSlash(path[len - 1]))
    {
        wcscat_s(path, LWDP_MAX_PATH,__T"/");
    }
    wcscat_s(path, LWDP_MAX_PATH, Co_IsPathSlash(*more) ? more + 1 : more);
}

tchar_* Co_PathAddBackslashW(tchar_* path)
{
    int32_ len = wcslen(path);

    if (len > 0 && !Co_IsPathSlash(path[len - 1]))
    {
        wcscat_s(path, LWDP_MAX_PATH,__T"/");
    }

    return path;
}

void Co_PathRenameExtensionW(tchar_* path, const tchar_* more)
{
    tchar_* dot = wcsrchr(path, L'.');

    if (!dot)
    {
        dot = path + wcslen(path);
    }
    wcscpy_s(dot, LWDP_MAX_PATH - (dot - path), more);
}
#endif


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

inline char_ * _co_strlwr_s(char_ *str)
    { for (char_* p = str; *p; p++) tolower(*p); return str; }
inline char_ * _co_strupr_s(char_ *str)
    { for (char_* p = str; *p; p++) toupper(*p); return str; }

inline int32_ _co_strcat_s(char_ *str, size_t, const char_ *src)
    { return strcat(str, src) ? 0 : -1; }

inline int32_ _co_strncpy_s(char_ *str, size_t size, const char_ *src, size_t len)
    { return strncpy(str, src, min(size, len)) ? 0 : -1; }


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
    { sprintf_s(str, size, 16 == radix ? "%lx" : "%ld", value); return 0; }
inline int32_ _co_itoa_s(int32_ value, char_ *str, size_t size, int32_ radix)
    { return _ltoa_s(value, str, size, radix); }

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
inline void localtime_s(struct tm *tmOut, const time_t *timer)
{
    struct tm * p = localtime(timer);
    if (tmOut != NULL && p != NULL)
        *tmOut = *p;
}
#endif // _INC_TIME

#define _sopen_s(fileHandler, filename, oflag, pmode, rw)   \
    (*fileHandler = _open(filename, oflag, pmode), errno)

EXTERN_C_END;
LWDP_NAMESPACE_END;
    
