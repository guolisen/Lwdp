#ifndef __DEF_INTERFACE_H
#define __DEF_INTERFACE_H

#ifdef _LWDP_UINT32_IS_ULONG
typedef long              int32_;
typedef unsigned long     uint32_;
#else
typedef int                int32_;
typedef unsigned int       uint32_;
#endif

typedef unsigned char       uint8_;
typedef unsigned short      uint16_;
typedef unsigned short      ushort_;
typedef unsigned long       ulong_;

#if defined(__STDC__) || defined(__cplusplus)
typedef signed char int8_;
#else
typedef char        int8_;
#endif

typedef short               int16_;
typedef short               short_;
typedef long                long_;

typedef char                char_;
typedef wchar_t             wchar_;

typedef float               float_;
typedef double              double_;

typedef unsigned char       byte_;
typedef unsigned short      word_;
typedef uint32_             dword_;

typedef int32_              bool_;



#undef TRUE
#define TRUE   1
#undef FALSE
#define FALSE  0

#ifdef _SZ_NO_INT_64

    /* define _SZ_NO_INT_64, if your compiler doesn't support 64-bit integers.
       NOTES: Some code will work incorrectly in that case! */

    typedef long_ int64_;
    typedef unsigned long_ uint64_;

#else

#if defined(_MSC_VER) || defined(__BORLANDC__)
    typedef __int64 int64_;
    typedef unsigned __int64 uint64_;
#define UINT64_CONST(n) n
#else
    //typedef long_ long_ int32_ int64_;
    //typedef unsigned long_ long_ int32_ uint64_;
#define UINT64_CONST(n) n ## ULL
#endif

#endif

#ifdef _LWDP_NO_SYSTEM_SIZE_T
typedef L_UINT32    size_t_;
#else
typedef size_t      size_t_;
#endif

#ifdef _UNICODE
typedef std::wstring tstring;
typedef wchar_ tchar_;
#else
typedef std::string  tstring;
typedef char_  tchar_;
#endif

typedef  long_  LWRESULT;


#endif