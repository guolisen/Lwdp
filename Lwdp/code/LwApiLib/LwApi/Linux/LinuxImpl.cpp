
#include <iostream>
#include <string>
#include <algorithm>

#include <LwDp.h>
#include <LwApiLib/LwApi/Linux/Api4Linux.h>
#include <Interface/PluginLoader/Ix_ObjectFactory.h>
#include <Interface/PluginLoader/Ix_PluginLoader2.h>

#include <dlfcn.h>
#include <stdio.h>

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

/****************************************************************************
*  Function:       WIN_TaskDelay
*  Description:    
*  Input:          ��
*  Output:         // ����������˵��
*  Return:         // �����ֵ��˵��	
*  Others:         // ����˵��
*****************************************************************************/
void LINUX_IMPL_API(TaskDelay)(ulong_ tick)
{
    sleep(tick);
}

/****************************************************************************
*  Function:       WIN_HaltSystem
*  Description:    
*  Input:          ��
*  Output:         // ����������˵��
*  Return:         // �����ֵ��˵��	
*  Others:         // ����˵��
*****************************************************************************/
void LINUX_IMPL_API(HaltSystem)()
{
    while(1) {sleep(1000);}    
}


int LINUX_IMPL_API(snprintf)( char *buffer, int num, const char *format, ... )
{
	int tempnum = 0;
	va_list args;
	va_start (args, format);
		tempnum = sprintf(buffer, format, args);
	va_end (args);

  	return tempnum;
}



#if 0
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
#endif

void GetModuleFileNameA(MODULEID hdll, char_* filename, int32_ size);

#define W2A(wstr)   //x3::w2a(wstr).c_str()

static std::string  s_err;
static std::map<MODULEID, std::string>   s_plugins;

static inline void seterr(const char* err)
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

MODULEID LoadLibraryA(const char_* filename)
{
    char_ fullpath[LWDP_MAX_PATH];

    if (Co_PathIsRelativeA(filename))
    {
        GetModuleFileNameA(NULL, fullpath, LWDP_MAX_PATH);
        Co_PathRemoveFileSpecA(fullpath);
        Co_PathAppendA(fullpath, filename);
        filename = fullpath;
    }

    std::string name(filename);
    MODULEID hdll = dlopen(name.c_str(), RTLD_LAZY);

    seterr(dlerror());
    if (!hdll)
    {
        char* tmpname = Co_PathFindFileNameA(name.c_str());
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

MODULEID LoadLibraryW(const wchar_t* filename)
{
#if 0
    wchar_t fullpath[LWDP_MAX_PATH];

    if (PathIsRelativeW(filename))
    {
        GetModuleFileNameW(NULL, fullpath, LWDP_MAX_PATH);
        PathRemoveFileSpecW(fullpath);
        PathAppendW(fullpath, filename);
        filename = fullpath;
    }

    std::string name(x3::w2a(filename));
    MODULEID hdll = dlopen(name.c_str(), RTLD_LAZY);

    seterr(dlerror());
    if (!hdll)
    {
        char* tmpname = PathFindFileNameA(name.c_str());
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
#endif
    return 0;
}

MODULEID LoadLibraryExW(const wchar_t* filename)
{
    return LoadLibraryW(filename);
}

static inline bool cmpdl(const char* dpname, const char* match)
{
    match = Co_PathFindFileNameA(match);
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

    int tempNum = std::min(len, len2);
    if (0 == strncasecmp((const char*)match, (const char*)(dpname + std::max(0, len - len2)), tempNum))
    {
        return true;
    }

    return false;
}

Ix_ObjectFactory* GetObjectFactory();
//static long s_objFactoryLocker = 0;

MODULEID GetModuleHandleW(const wchar_t* filename)
{
#if 0
    std::string match(x3::w2a(filename));
    std::map<MODULEID, std::string>::const_iterator it;

    for (it = s_plugins.begin(); it != s_plugins.end(); ++it)
    {
        if (cmpdl(it->second.c_str(), match.c_str()))
        {
            return it->first;
        }
    }

    CLockCount locker(&s_objFactoryLocker);
    Ix_PluginLoader2* factory = (s_objFactoryLocker > 1) ? NULL
        : dynamic_cast<Ix_PluginLoader2*>(xGetObjectFactory());

    if (factory)
    {
        MODULEID hdll = NULL;
        std::wstring file;

        for (int32_ i = 0; factory->GetPluginFileName(i, hdll, file); i++)
        {
            if (cmpdl(W2A(file), match.c_str()))
            {
                return hdll;
            }
        }
    }
#endif
    return NULL;
}

void* GetProcAddress(MODULEID hdll, const char* name)
{
    void* sym = NULL;

    if (hdll)
    {
        sym = dlsym(hdll, name);
        seterr(dlerror());
    }

    return sym;
}

void GetModuleFileNameW(MODULEID hdll, wchar_t* filename, int size)
{
#if 0
    *filename = 0;

    if (!hdll)
    {
        char tmp[32];
        char buf[MAX_PATH];

        sprintf(tmp, "/proc/%d/exe", getpid());
        int bytes = readlink(tmp, buf, LWDP_MAX_PATH);
        if (bytes > 0)
        {
            buf[std::min(bytes, LWDP_MAX_PATH - 1)] = '\0';
            wcscpy_s(filename, size, x3::a2w(buf).c_str());
        }
    }
    else
    {
        std::map<MODULEID, std::string>::const_iterator it = s_plugins.find(hdll);
        if (it != s_plugins.end())
        {
            wcscpy_s(filename, size, x3::a2w(it->second).c_str());
        }

        CLockCount locker(&s_objFactoryLocker);
        Ix_PluginLoader2* factory = (s_objFactoryLocker > 1) ? NULL
            : dynamic_cast<Ix_PluginLoader2*>(xGetObjectFactory());

        if (factory && 0 == filename[0])
        {
            MODULEID hdll2 = NULL;
            std::wstring file;

            for (int i = 0; factory->GetPluginFileName(i, hdll2, file); i++)
            {
                if (hdll2 == hdll)
                {
                    wcscpy_s(filename, size, file.c_str());
                }
            }
        }
    }
#endif
}

void GetModuleFileNameA(MODULEID hdll, char_* filename, int32_ size)
{
    *filename = 0;

    if (!hdll)
    {
        char_ tmp[32];
        char_ buf[LWDP_MAX_PATH];

        sprintf(tmp, "/proc/%d/exe", getpid());
        int32_ bytes = readlink(tmp, buf, LWDP_MAX_PATH);
        if (bytes > 0)
        {
            buf[std::min(bytes, LWDP_MAX_PATH - 1)] = '\0';
            strncpy(filename, buf, size);
        }
    }
    else
    {
        std::map<MODULEID, std::string>::const_iterator it = s_plugins.find(hdll);
        if (it != s_plugins.end())
        {
            strncpy(filename, it->second.c_str(), size);
        }

        Cx_Interface<Ix_PluginLoader2> factory(GetObjectFactory());

        if (factory && 0 == filename[0])
        {
            MODULEID hdll2 = NULL;
            std::string file;

            for (int32_ i = 0; factory->GetPluginFileName(i, hdll2, file); i++)
            {
                if (hdll2 == hdll)
                {
                    strncpy(filename, file.c_str(), size);
                }
            }
        }
    }
}



bool OpenFileForRead(HANDLE& hfile, const wchar_t* filename)
{
    return false;
}

bool OpenFileForWrite(HANDLE& hfile, const wchar_t* filename)
{
    return false;
}

bool CloseFile(HANDLE file)
{
    return false;
}

bool ReadFile(HANDLE file, void* buf, DWORD size, DWORD* readed, void*)
{
    return false;
}

bool WriteFile(HANDLE file, const void* buf, DWORD size, DWORD* written, void*)
{
    return false;
}

bool PathFileExistsW(const wchar_t* path)
{
    return false;
}

bool CreateDirectoryW(const wchar_t* path, void*)
{
    return false;
}

bool DeleteFileW(const wchar_t* filename)
{
    return false;
}

DWORD GetFileSize(HANDLE file, DWORD* high)
{
    return 0;
}

void GetTempPathW(DWORD len, wchar_t* buf)
{
}

bool CheckFileAttributes(const wchar_t* filename, bool* readonly, bool* folder)
{
    return false;
}

bool SetFileAttributesNormal(const wchar_t* filename)
{
    return false;
}

DWORD GetLastError()
{
    return 0;
}

int WideCharToMultiByte(int /*codepage*/, DWORD /*flags*/,
                        const wchar_t* wstr, int wchars,
                        char* astr, int achars,
                        const char*, void*)
{
    return astr ? wcstombs(astr, wstr, achars)
        : (wchars * sizeof(wchar_t));
}

int MultiByteToWideChar(int /*codepage*/, DWORD /*flags*/,
                        const char* astr, int achars,
                        wchar_t* wstr, int wchars)
{
    return wstr ? mbstowcs(wstr, astr, wchars) : achars;
}

long InterlockedIncrement(long* p)
{
    return ++(*p);
}

long InterlockedDecrement(long* p)
{
    return --(*p);
}

long InterlockedExchange(long* p, long v)
{
    long old = *p;
    *p = v;
    return old;
}
EXTERN_C_END;
LWDP_NAMESPACE_END;
