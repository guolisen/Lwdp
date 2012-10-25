#ifndef LWDP_PLATFORM_DEFINE
#define LWDP_PLATFORM_DEFINE

#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__GNU__) || defined(__GLIBC__) 
// linux, also other platforms (Hurd etc) that use GLIBC, should these really have their own config headers though?
#  define LWDP_PLATFORM_DEF_LINUX

#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)
// BSD:
#  define LWDP_PLATFORM_DEF_BSD

#elif defined(sun) || defined(__sun)
// solaris:
#  define LWDP_PLATFORM_DEF_SUN

#elif defined(__sgi)
// SGI Irix:
#  define LWDP_PLATFORM_DEF_IRIX

#elif defined(__hpux)
// hp unix:
#  define LWDP_PLATFORM_DEF_HPUX

#elif defined(__CYGWIN__)
// cygwin is not win32:
#  define LWDP_PLATFORM_DEF_CYWIN

#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
// win32:
#  define LWDP_PLATFORM_DEF_WIN32

#elif defined(__BEOS__)
// BeOS
#  define LWDP_PLATFORM_DEF_BEOS

#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
// MacOS
#  define LWDP_PLATFORM_DEF_MACOS

#elif defined(__IBMCPP__) || defined(_AIX)
// IBM
#  define LWDP_PLATFORM_DEF_AIX

#elif defined(__amigaos__)
// AmigaOS
#  define LWDP_PLATFORM_DEF_AMIGAOS

#elif defined(__QNXNTO__)
// QNX:
#  define LWDP_PLATFORM_DEF_QNXNTO

#elif defined(__VXWORKS__)
// vxWorks:
#  define LWDP_PLATFORM_DEF_VXWORKS

#elif defined(__SYMBIAN32__) 
// Symbian: 
#  define LWDP_PLATFORM_DEF_SYMBIAN

#elif defined(__VMS) 
// VMS:
#  define LWDP_PLATFORM_DEF_VMS

#endif

#endif

