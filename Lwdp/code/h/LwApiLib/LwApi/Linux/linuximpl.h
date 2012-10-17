// Included by portimpl.h to implement functions on Linux.

#ifndef X3LINUX_PORTABILITY_IMPL_H
#define X3LINUX_PORTABILITY_IMPL_H

#include "x3unix.h"
#include "../PluginManager/Ix_PluginLoader2.h"
#include "../PluginManager/PluginManager.h"
#include "../UtilFunc/ConvStr.h"
#include <dlfcn.h>
#include <stdio.h>

#define W2A(wstr)   x3::w2a(wstr).c_str()

static tstring  s_err;
static std::map<MODULEID, tstring>   s_plugins;

static inline void seterr(const tchar_* err)
{
    s_err = err ? err : "";
}

bool FreeLibrary(MODULEID hdll)
{
    int32_ ret = hdll ? dlclose(hdll) : 0;

    seterr(dlerror());
    s_plugins.erase(hdll);

    return 0 == ret;
}

MODULEID LoadLibraryW(const tchar_* filename)
{
    tchar_ fullpath[LWDP_MAX_PATH];

    if (Api_PathIsRelative(filename))
    {
        Api_GetModuleFileName(NULL, fullpath, LWDP_MAX_PATH);
        Api_PathRemoveFileSpec(fullpath);
        Api_PathAppend(fullpath, filename);
        filename = fullpath;
    }

    tstring name(x3::w2a(filename));
    MODULEID hdll = dlopen(name.c_str(), RTLD_LAZY);

    seterr(dlerror());
    if (!hdll)
    {
        tchar_* tmpname = PathFindFileNameA(name.c_str());
        if (tmpname && strncasecmp(tmpname, "lib", 3) != 0)
        {
            name.insert(tmpname - name.c_str(), "lib");
            hdll = dlopen(name.c_str(), RTLD_LAZY);
            seterr(dlerror());
        }
    }
    if (hdll)
    {
        s_plugins[hdll] = name;
    }

    return hdll;
}

MODULEID LoadLibraryExW(const tchar_* filename)
{
    return LoadLibraryW(filename);
}

static inline bool cmpdl(const tchar_* dpname, const tchar_* match)
{
    match = PathFindFileNameA(match);
    if (NULL == match)
    {
        return false;
    }
    if (strncasecmp(match, "lib", 3) == 0)
    {
        match += 3;
    }

    int32_ len = strlen(dpname);
    int32_ len2 = strlen(match);

    if (0 == strncasecmp(match, dpname + std::max(0, len - len2), std::min(len, len2)))
    {
        return true;
    }

    return false;
}

Ix_ObjectFactory* x3GetObjectFactory();

MODULEID GetModuleHandleW(const tchar_* filename)
{
    if (CPluginManager::Handle()
        && _wcsicmp(filename,__T"PluginManagerX3" PLNEXT) == 0)
    {
        return CPluginManager::Handle();
    }

    tstring match(x3::w2a(filename));
    std::map<MODULEID, tstring>::const_iterator it;

    for (it = s_plugins.begin(); it != s_plugins.end(); ++it)
    {
        if (cmpdl(it->second.c_str(), match.c_str()))
        {
            return it->first;
        }
    }

    Cx_Interface<Ix_PluginLoader2> factory(x3GetObjectFactory());

    if (factory)
    {
        MODULEID hdll = NULL;
        tstring file;

        for (int32_ i = 0; factory->GetPluginFileName(i, hdll, file); i++)
        {
            if (cmpdl(W2A(file), match.c_str()))
            {
                return hdll;
            }
        }
    }

    return NULL;
}

void* GetProcAddress(MODULEID hdll, const tchar_* name)
{
    void* sym = NULL;

    if (hdll)
    {
        sym = dlsym(hdll, name);
        seterr(dlerror());
    }

    return sym;
}

void GetModuleFileNameW(MODULEID hdll, tchar_* filename, int32_ size)
{
    *filename = 0;

    if (!hdll)
    {
        tchar_ tmp[32];
        tchar_ buf[LWDP_MAX_PATH];

        sprintf(tmp, "/proc/%d/exe", getpid());
        int32_ bytes = readlink(tmp, buf, LWDP_MAX_PATH);
        if (bytes > 0)
        {
            buf[std::min(bytes, LWDP_MAX_PATH - 1)] = '\0';
            wcscpy_s(filename, size, x3::a2w(buf).c_str());
        }
    }
    else
    {
        std::map<MODULEID, tstring>::const_iterator it = s_plugins.find(hdll);
        if (it != s_plugins.end())
        {
            wcscpy_s(filename, size, x3::a2w(it->second).c_str());
        }

        Cx_Interface<Ix_PluginLoader2> factory(x3GetObjectFactory());

        if (factory && 0 == filename[0])
        {
            MODULEID hdll2 = NULL;
            tstring file;

            for (int32_ i = 0; factory->GetPluginFileName(i, hdll2, file); i++)
            {
                if (hdll2 == hdll)
                {
                    wcscpy_s(filename, size, file.c_str());
                }
            }
        }
    }
}

void GetModuleFileNameA(MODULEID hdll, tchar_* filename, int32_ size)
{
    tchar_ wfile[LWDP_MAX_PATH] = { 0 };

    Api_GetModuleFileName(hdll, wfile, LWDP_MAX_PATH);
    strcpy_s(filename, size, x3::w2a(wfile).c_str());
}


bool OpenFileForRead(HANDLE& hfile, const tchar_* filename)
{
    return false;
}

bool OpenFileForWrite(HANDLE& hfile, const tchar_* filename)
{
    return false;
}

bool CloseFile(HANDLE file)
{
    return false;
}

bool Api_ReadFile(HANDLE file, void* buf, uint32_ size, uint32_* readed, void*)
{
    return false;
}

bool WriteFile(HANDLE file, const void* buf, uint32_ size, uint32_* written, void*)
{
    return false;
}

bool PathFileExistsW(const tchar_* path)
{
    return false;
}

bool CreateDirectoryW(const tchar_* path, void*)
{
    return false;
}

bool DeleteFileW(const tchar_* filename)
{
    return false;
}

uint32_ GetFileSize(HANDLE file, uint32_* high)
{
    return 0;
}

void GetTempPathW(uint32_ len, tchar_* buf)
{
}

bool CheckFileAttributes(const tchar_* filename, bool* readonly, bool* folder)
{
    return false;
}

bool SetFileAttributesNormal(const tchar_* filename)
{
    return false;
}

uint32_ GetLastError()
{
    return 0;
}

int32_ WideCharToMultiByte(int32_ /*codepage*/, uint32_ /*flags*/,
                        const tchar_* wstr, int32_ wchars,
                        tchar_* astr, int32_ achars,
                        const tchar_*, void*)
{
    return astr ? wcstombs(astr, wstr, achars)
        : (wchars * sizeof(tchar_));
}

int32_ MultiByteToWideChar(int32_ /*codepage*/, uint32_ /*flags*/,
                        const tchar_* astr, int32_ achars,
                        tchar_* wstr, int32_ wchars)
{
    return wstr ? mbstowcs(wstr, astr, wchars) : achars;
}

long_ InterlockedIncrement(long_* p)
{
    return ++(*p);
}

long_ InterlockedDecrement(long_* p)
{
    return --(*p);
}

long_ InterlockedExchange(long_* p, long_ v)
{
    long_ old = *p;
    *p = v;
    return old;
}

#endif // X3LINUX_PORTABILITY_IMPL_H
