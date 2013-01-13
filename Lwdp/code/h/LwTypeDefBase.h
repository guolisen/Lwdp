/*! \file LwTypeDefBase.h
 *  \brief Define base type
 *  \author Guolisen, LwDp
 *  \date   2011.07.14
 */

#ifndef LW_TYPE_DEF_BASE_H_
#define LW_TYPE_DEF_BASE_H_



#ifndef EXTERN_C_BEGIN
#ifdef __cplusplus
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif
#endif

#ifndef NO_LWDP_NAMESPACE_BEGIN
#ifdef __cplusplus
#define LWDP_NAMESPACE_BEGIN namespace NLwdp { 
#define LWDP_NAMESPACE_END  }
#else
#define LWDP_NAMESPACE_BEGIN  
#define LWDP_NAMESPACE_END
#endif
#endif

LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;


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
/////////////////////////////////////////////////////////
#ifdef _SZ_NO_INT_64

    /* define _SZ_NO_INT_64, if your compiler doesn't support 64-bit integers.
       NOTES: Some code will work incorrectly in that case! */

    typedef long_ int64_;
    typedef unsigned long_ uint64_;

#else

#if defined(_MSC_VER) || defined(__BORLANDC__)
#define UINT64_CONST(n) n
#else
#define UINT64_CONST(n) n ## ULL
#endif

#if defined(_MSC_VER)
	//
	// Windows/Visual C++
	//
	typedef signed __int64         int64_;
	typedef unsigned __int64       uint64_;
	#if defined(_WIN64)
		#define LWDP_PTR_IS_64_BIT 1
		typedef signed __int64     intPtr_;
		typedef unsigned __int64   uintPtr_;
	#else
		typedef signed long        intPtr_;
		typedef unsigned long      uintPtr_;
	#endif
	#define LWDP_HAVE_INT64 1
#elif defined(__GNUC__) || defined(__clang__)
	//
	// Unix/GCC
	//
	typedef signed long            intPtr_;
	typedef unsigned long          uintPtr_;
	#if defined(__LP64__)
		#define LWDP_PTR_IS_64_BIT 1
		#define LWDP_LONG_IS_64_BIT 1
		typedef signed long        int64_;
		typedef unsigned long      uint64_;
	#else
		typedef signed long long   int64_;
		typedef unsigned long long uint64_;
	#endif
	#define LWDP_HAVE_INT64 1
#elif defined(__DECCXX)
	//
	// Compaq C++
	//
	typedef signed __int64         int64_;
	typedef unsigned __int64       uint64_;
	#if defined(__VMS)
		#if defined(__32BITS)
			typedef signed long    intPtr_;
			typedef unsigned long  uintPtr_;
		#else
			typedef int64_          intPtr_;
			typedef uint64_         uintPtr_;
			#define LWDP_PTR_IS_64_BIT 1
		#endif
	#else
		typedef signed long        intPtr_;
		typedef unsigned long      uintPtr_;
		#define LWDP_PTR_IS_64_BIT 1
		#define LWDP_LONG_IS_64_BIT 1
	#endif
	#define LWDP_HAVE_INT64 1
#elif defined(__HP_aCC)
	//
	// HP Ansi C++
	//
	typedef signed long            intPtr_;
	typedef unsigned long          uintPtr_;
	#if defined(__LP64__)
		#define LWDP_PTR_IS_64_BIT 1
		#define LWDP_LONG_IS_64_BIT 1
		typedef signed long        int64_;
		typedef unsigned long      uint64_;
	#else
		typedef signed long long   int64_;
		typedef unsigned long long uint64_;
	#endif
	#define LWDP_HAVE_INT64 1
#elif defined(__SUNPRO_CC)
	//
	// SUN Forte C++
	//
	typedef signed long            intPtr_;
	typedef unsigned long          uintPtr_;
	#if defined(__sparcv9)
		#define LWDP_PTR_IS_64_BIT 1
		#define LWDP_LONG_IS_64_BIT 1
		typedef signed long        int64_;
		typedef unsigned long      uint64_;
	#else
		typedef signed long long   int64_;
		typedef unsigned long long uint64_;
	#endif
	#define LWDP_HAVE_INT64 1
#elif defined(__IBMCPP__) 
	//
	// IBM XL C++
	//
	typedef signed long            intPtr_;
	typedef unsigned long          uintPtr_;
	#if defined(__64BIT__)
		#define LWDP_PTR_IS_64_BIT 1
		#define LWDP_LONG_IS_64_BIT 1
		typedef signed long        int64_;
		typedef unsigned long      uint64_;
	#else
		typedef signed long long   int64_;
		typedef unsigned long long uint64_;
	#endif
	#define LWDP_HAVE_INT64 1
#elif defined(__sgi) 
	//
	// MIPSpro C++
	//
	typedef signed long            intPtr_;
	typedef unsigned long          uintPtr_;
	#if _MIPS_SZLONG == 64
		#define LWDP_PTR_IS_64_BIT 1
		#define LWDP_LONG_IS_64_BIT 1
		typedef signed long        int64_;
		typedef unsigned long      uint64_;
	#else
		typedef signed long long   int64_;
		typedef unsigned long long uint64_;
	#endif
	#define LWDP_HAVE_INT64 1
#elif defined(_DIAB_TOOL)
	typedef signed long        intPtr_;
	typedef unsigned long      uintPtr_;
	typedef signed long long   int64_;
	typedef unsigned long long uint64_;
	#define LWDP_HAVE_INT64 1
#endif


#endif
//////////////////////////////////////////

////////////////////////////////////////////////////////


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



EXTERN_C_END;
LWDP_NAMESPACE_END;
#endif




 

