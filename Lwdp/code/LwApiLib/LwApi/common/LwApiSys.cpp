// Copyright (C), 2006��2011, Potevio Institute of Technology.
/************************************************************
*  File name:   LwApiCommon.cpp  	
*  Author:      guolisen
*  Version:   	v1.0
*  Create Date: 2011-08-11
***********************************************************/
// 2011.08.11: Create By Guolisen

#include <LwDp.h>

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

#ifndef _UNICODE

static inline bool IsPathSlashA(char_ c)
{
    return '\\' == c || '/' == c;
}

char_* Co_PathFindFileNameA(const char_* path)
{
    const char_* p1 = path ? strrchr(path, '\\') : NULL;
    const char_* p2 = path ? strrchr(path, '/') : NULL;

    p1 = !p1 || (p2 && p2 > p1) ? p2 : p1;

    return const_cast<char_*>(p1 ? p1 + 1 : p1);
}

bool Co_PathIsRelativeA(const char_* path)
{
    return strrchr(path, ':') == NULL && path[0] != '\\' && path[0] != '/';
}

void Co_PathStripPathA(char_* path)
{
    Co_PathRemoveBackslashA(path);
    char_* filename = Co_PathFindFileNameA(path);

    if (filename && filename > path)
    {
        while (*filename)
        {
            *path++ = *filename++;
        }
        *path = *filename;
    }
}

void Co_PathRemoveFileSpecA(char_* path)
{
    Co_PathRemoveBackslashA(path);
    char_* filename = Co_PathFindFileNameA(path);

    if (filename && filename > path)
    {
        *(filename - 1) = 0;
    }
}

void Co_PathRemoveExtensionA(char_* path)
{
    char_* dot = strrchr(path, '.');

    if (dot)
    {
        *dot = 0;
    }
}

void Co_PathRemoveBackslashA(char_* path)
{
    int32_ len = strlen(path);

    if (len > 0 && IsPathSlashA(path[len - 1]))
    {
        path[len - 1] = 0;
    }
}

void Co_PathAppendA(char_* path, const char_* more)
{
	//ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PARAMETER_ERROR, (path && more), "Co_PathAppendA Para Pointor is NULL");
    int32_ len = strlen(path);

    if (len > 0 && !IsPathSlashA(path[len - 1]))
    {
        //wcscat_s(path, LWDP_MAX_PATH,__T"/");
        strncat(path, "/", LWDP_MAX_PATH);
    }
	strncat(path, IsPathSlashA(*more) ? more + 1 : more, LWDP_MAX_PATH);
}

tchar_* Co_PathAddBackslashA(char_* path)
{
    int32_ len = strlen(path);

    if (len > 0 && !IsPathSlashA(path[len - 1]))
    {
        //wcscat_s(path, LWDP_MAX_PATH,__T"/");
		strncat(path, "/", LWDP_MAX_PATH);
    }

    return path;
}

void Co_PathRenameExtensionA(char_* path, const char_* more)
{
    char_* dot = strrchr(path, '.');

    if (!dot)
    {
        dot = path + strlen(path);
    }
    strncpy(dot, more, LWDP_MAX_PATH - (dot - path));
}



#else
static inline bool IsPathSlashW(tchar_ c)
{
    return L'\\' == c || L'/' == c;
}

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

    if (len > 0 && IsPathSlashW(path[len - 1]))
    {
        path[len - 1] = 0;
    }
}

void Co_PathAppendW(tchar_* path, const tchar_* more)
{
	ASSERT_CHECK_RET(LWDP_MODULE_LOG, LWDP_PARAMETER_ERROR, (path && more), "Co_PathAppendW Para Pointor is NULL");
    int32_ len = wcslen(path);

    if (len > 0 && !IsPathSlashW(path[len - 1]))
    {
        wcscat_s(path, LWDP_MAX_PATH,__T"/");
    }
    wcscat_s(path, LWDP_MAX_PATH, Co_IsPathSlash(*more) ? more + 1 : more);
}

tchar_* Co_PathAddBackslashW(tchar_* path)
{
    int32_ len = wcslen(path);

    if (len > 0 && !IsPathSlashW(path[len - 1]))
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


EXTERN_C_END;
LWDP_NAMESPACE_END;
    
