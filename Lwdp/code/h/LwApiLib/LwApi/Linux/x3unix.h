// Included by X3Portability.h to declare functions on linux or macosx.

#ifndef X3UNIX_PORTABILITY_H
#define X3UNIX_PORTABILITY_H

#include <string.h>
#include <stdlib.h>
#include <wchar.h>

#define DECLEXPORT
#define OUTAPI      extern "C" DECLEXPORT
#define PLNEXT     __T".so"

#ifndef LWDP_MAX_PATH
#define LWDP_MAX_PATH 256
#endif

typedef void* MODULEID;
typedef void* HANDLE;
typedef void* HWND;
typedef unsigned tchar_ BYTE;
typedef unsigned short WORD;
typedef unsigned long_ uint32_;
typedef uint32_ ULONG;

long_ InterlockedIncrement(long_* p);
long_ InterlockedDecrement(long_* p);
long_ InterlockedExchange(long_* p, long_ v);

bool FreeLibrary(MODULEID hdll);
MODULEID LoadLibraryW(const tchar_* filename);
MODULEID LoadLibraryExW(const tchar_* filename);
MODULEID GetModuleHandleW(const tchar_* filename);
void* GetProcAddress(MODULEID hdll, const tchar_* name);
void GetModuleFileNameW(MODULEID hdll, tchar_* filename, int32_ size);
void GetModuleFileNameA(MODULEID hdll, tchar_* filename, int32_ size);

bool OpenFileForRead(HANDLE& hfile, const tchar_* filename);
bool OpenFileForWrite(HANDLE& hfile, const tchar_* filename);
bool CloseFile(HANDLE file);
bool ReadFile(HANDLE file, void* buf, uint32_ size, uint32_* readed, void*);
bool WriteFile(HANDLE file, const void* buf, uint32_ size, uint32_* written, void*);
bool PathFileExistsW(const tchar_* path);
bool CreateDirectoryW(const tchar_* path, void*);
bool DeleteFileW(const tchar_* filename);
uint32_ GetFileSize(HANDLE file, uint32_* high);

tchar_* PathFindFileNameA(const tchar_* path);
tchar_* PathFindFileNameW(const tchar_* path);
bool PathIsRelativeW(const tchar_* path);
void PathStripPathW(tchar_* path);
void PathRemoveFileSpecW(tchar_* path);
void PathRemoveExtensionW(tchar_* path);
void PathRemoveBackslashW(tchar_* path);
void PathAppendW(tchar_* path, const tchar_* more);
tchar_* PathAddBackslashW(tchar_* path);
void PathRenameExtensionW(tchar_* path, const tchar_* more);

void GetTempPathW(uint32_ len, tchar_* buf);
bool CheckFileAttributes(const tchar_* filename, bool* readonly, bool* folder);
bool SetFileAttributesNormal(const tchar_* filename);
uint32_ GetLastError();

int32_ WideCharToMultiByte(int32_ codepage, uint32_ flags,
                        const tchar_* wstr, int32_ wchars,
                        tchar_* astr, int32_ achars,
                        const tchar_* defaultChar, void*);
int32_ MultiByteToWideChar(int32_ codepage, uint32_ flags,
                        const tchar_* astr, int32_ achars,
                        tchar_* wstr, int32_ wchars);

inline int32_ _stricmp(const tchar_* s1, const tchar_* s2)
    { return strncasecmp(s1, s2, strlen(s1)); }
inline int32_ _wcsicmp(const tchar_* s1, const tchar_* s2)
    { return wcsncasecmp(s1, s2, wcslen(s1)); }
inline int32_ _wcsnicmp(const tchar_* s1, const tchar_* s2, int32_ count)
    { return wcsncasecmp(s1, s2, count); }

#endif // X3UNIX_PORTABILITY_H
