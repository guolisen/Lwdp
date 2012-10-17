/*! \file LwCpuArch.h
 *  \brief CPU feature Define
 *  \author Guolisen, LwDp
 *  \date   2011.07.14
 */

#ifndef LW_CPU_ARCH_H_
#define LW_CPU_ARCH_H_

LWDP_NAMESPACE_BEGIN;

#define _LWDP_MC68000	1	/* _LWDP_CPU */
#define _LWDP_MC68010	2	/* _LWDP_CPU */
#define _LWDP_MC68020	3	/* _LWDP_CPU */
#define _LWDP_MC68030	4	/* _LWDP_CPU */
#define _LWDP_MC68040	5	/* _LWDP_CPU */
#define _LWDP_MC68LC040	6	/* _LWDP_CPU */
#define _LWDP_MC68060	7	/* _LWDP_CPU */
#define _LWDP_CPU32	8	/* _LWDP_CPU */
#define _LWDP_MC680X0	9	/* _LWDP_CPU_FAMILY */
#define _LWDP_SPARC	10	/* _LWDP_CPU & _LWDP_CPU_FAMILY */
#define _LWDP_SPARClite	11	/* _LWDP_CPU */
#define _LWDP_I960	20	/* _LWDP_CPU_FAMILY */
#define _LWDP_I960CA	21	/* _LWDP_CPU */
#define _LWDP_I960KA	22	/* _LWDP_CPU */
#define _LWDP_I960KB	23	/* _LWDP_CPU */
#define _LWDP_I960JX	24	/* _LWDP_CPU */
#define _LWDP_I960HX	25	/* _LWDP_CPU */
#define _LWDP_TRON	30	/* _LWDP_CPU_FAMILY */
#define _LWDP_G100	31	/* _LWDP_CPU */
#define _LWDP_G200	32	/* _LWDP_CPU */
#define _LWDP_MIPS	40	/* _LWDP_CPU_FAMILY */
#define _LWDP_MIPS32	41	/* _LWDP_CPU */
#define _LWDP_MIPS64	42	/* _LWDP_CPU */
#define _LWDP_AM29XXX	50	/* _LWDP_CPU_FAMILY */
#define _LWDP_AM29030	51	/* _LWDP_CPU */
#define _LWDP_AM29200	52	/* _LWDP_CPU */
#define _LWDP_AM29035	53	/* _LWDP_CPU */
#define _LWDP_SIMSPARCSUNOS	60	/* _LWDP_CPU & _LWDP_CPU_FAMILY */
#define _LWDP_SIMSPARCSOLARIS	61	/* _LWDP_CPU & _LWDP_CPU_FAMILY */
#define _LWDP_SIMHPPA	70	/* _LWDP_CPU & _LWDP_CPU_FAMILY */
#define _LWDP_I80X86	80	/* _LWDP_CPU_FAMILY */
#define _LWDP_I80386	81	/* _LWDP_CPU */
#define _LWDP_I80486	82	/* _LWDP_CPU */
#define _LWDP_PENTIUM	83	/* _LWDP_CPU */
#define _LWDP_PENTIUM2	84	/* _LWDP_CPU */
#define _LWDP_PENTIUM3	85	/* _LWDP_CPU */
#define _LWDP_PENTIUM4	86	/* _LWDP_CPU */
#define _LWDP_PPC32	90	/* _LWDP_CPU & _LWDP_CPU_FAMILY */
#define _LWDP_PPC	90	/* _LWDP_CPU_FAMILY */
#define _LWDP_PPC601	91	/* _LWDP_CPU */
#define _LWDP_PPC602	92	/* _LWDP_CPU */
#define _LWDP_PPC603	93	/* _LWDP_CPU */
#define _LWDP_PPC604	94	/* _LWDP_CPU */
#define _LWDP_PPC403	95	/* _LWDP_CPU */
#define _LWDP_PPC505	96	/* _LWDP_CPU */
#define _LWDP_PPC860	97	/* _LWDP_CPU */
#define _LWDP_PPCEC603	98	/* _LWDP_CPU */
#define _LWDP_PPC555	99	/* _LWDP_CPU */
#define _LWDP_SIMNT	100	/* _LWDP_CPU & _LWDP_CPU_FAMILY */
#define _LWDP_SPARCV9	110	/* _LWDP_CPU_FAMILY */
#define _LWDP_ULTRASPARC	111	/* _LWDP_CPU */
#define _LWDP_ARM	120	/* _LWDP_CPU_FAMILY */
#define _LWDP_ARM710A	121	/* _LWDP_CPU */
#define _LWDP_ARM7TDMI	122	/* _LWDP_CPU */
#define _LWDP_ARM810	123	/* _LWDP_CPU */
#define _LWDP_ARMSA110	124	/* _LWDP_CPU */
#define _LWDP_ARM7TDMI_T	125	/* _LWDP_CPU */
#define _LWDP_ARMARCH3	126	/* _LWDP_CPU */
#define _LWDP_ARMARCH4	127	/* _LWDP_CPU */
#define _LWDP_ARMARCH4_T	128	/* _LWDP_CPU */
#define _LWDP_STRONGARM	129	/* _LWDP_CPU */
#define _LWDP_SH32	130	/* _LWDP_CPU & _LWDP_CPU_FAMILY */
#define _LWDP_SH	130	/* _LWDP_CPU_FAMILY */
#define _LWDP_SH7000	131	/* _LWDP_CPU */
#define _LWDP_SH7600	132	/* _LWDP_CPU */
#define _LWDP_SH7040	133	/* _LWDP_CPU */
#define _LWDP_SH7700	134	/* _LWDP_CPU */
#define _LWDP_SH7410	135	/* _LWDP_CPU */
#define _LWDP_SH7729	136	/* _LWDP_CPU */
#define _LWDP_SH7750	137	/* _LWDP_CPU */
#define _LWDP_MCORE	150	/* _LWDP_CPU_FAMILY */
#define _LWDP_MCORE10	151	/* _LWDP_CPU */
#define _LWDP_MCORE15	152	/* _LWDP_CPU */
#define _LWDP_MCORE200	153	/* _LWDP_CPU */
#define _LWDP_MCORE300	154	/* _LWDP_CPU */
#define _LWDP_COLDFIRE	160	/* _LWDP_CPU_FAMILY */
#define _LWDP_MCF5200	161	/* _LWDP_CPU */
#define _LWDP_MCF5400	162	/* _LWDP_CPU */
#define _LWDP_FRV	170	/* _LWDP_CPU_FAMILY */
#define _LWDP_FR500	171	/* _LWDP_CPU */
#define _LWDP_MAP1000	180	/* _LWDP_CPU & _LWDP_CPU_FAMILY */
#define _LWDP_MAP	180	/* _LWDP_CPU_FAMILY */
#define _LWDP_MAP1000A	181	/* _LWDP_CPU */
#define _LWDP_MAPCA	182	/* _LWDP_CPU */
#define _LWDP_SIMLINUX	190	/* _LWDP_CPU & _LWDP_CPU_FAMILY */
#define _LWDP_SIMPENTIUM	191	/* _LWDP_CPU & _LWDP_CPU_FAMILY */
#define _LWDP_XSCALE	1120	/* _LWDP_CPU */
#define _LWDP_ARMARCH5	1121	/* _LWDP_CPU */
#define _LWDP_ARMARCH5_T	1122	/* _LWDP_CPU */
#define _LWDP_ARMARCH6	1123	/* _LWDP_CPU */
#define _LWDP_ARMARCH6_T	1124	/* _LWDP_CPU */
#define _LWDP_PPC509	2000	/* _LWDP_CPU */
#define _LWDP_PPC405	2001	/* _LWDP_CPU */
#define _LWDP_PPC405F	2002	/* _LWDP_CPU */
#define _LWDP_PPC440	2003	/* _LWDP_CPU */
#define _LWDP_PPC85XX	2004	/* _LWDP_CPU */
#define _LWDP_PPC970	2005	/* _LWDP_CPU */
#define _LWDP_PPC750	2010	/* _LWDP_CPU */
#define _LWDP_PPC7400	2011	/* _LWDP_CPU */
#define _LWDP_PPC8260	2012	/* _LWDP_CPU */
#define _LWDP_PPC8560	2013	/* _LWDP_CPU */

#ifndef _LWDP_CPU
#if defined(_M_IX86) || defined(__i386__) 
#define _LWDP_CPU _LWDP_I8X386
#endif

#if defined(_M_ARM) || defined(__ARMEL__) || defined(__ARM__)
#define _LWDP_CPU _LWDP_ARM
#endif

#if defined(__MIPSEL__) || defined(__MIPS__) 
#define _LWDP_CPU _LWDP_MIPS
#endif

#if defined(__PPC32__) || defined(__PPC__) 
#define _LWDP_CPU _LWDP_PPC
#endif
#endif

#ifndef _LWDP_CPU
Not define _LWDP_CPU
#endif


#if	(_LWDP_CPU==_LWDP_MC680X0 || \
     _LWDP_CPU==_LWDP_MC68000 || \
	 _LWDP_CPU==_LWDP_MC68010 || \
	 _LWDP_CPU==_LWDP_MC68020 || \
	 _LWDP_CPU==_LWDP_MC68030 || \
	 _LWDP_CPU==_LWDP_MC68040 || \
	 _LWDP_CPU==_LWDP_MC68LC040 || \
	 _LWDP_CPU==_LWDP_MC68060 || \
	 _LWDP_CPU==_LWDP_CPU32)
#define	_LWDP_CPU_FAMILY	_LWDP_MC680X0
#endif	/* _LWDP_CPU_FAMILY==_LWDP_MC680X0 */

#if	(_LWDP_CPU==_LWDP_SPARC || \
	 _LWDP_CPU==_LWDP_SPARClite)
#define	_LWDP_CPU_FAMILY	_LWDP_SPARC
#endif	/* _LWDP_CPU_FAMILY==_LWDP_SPARC */

#if	(_LWDP_CPU==_LWDP_I960 || \
     _LWDP_CPU==_LWDP_I960CA || \
	 _LWDP_CPU==_LWDP_I960KA || \
	 _LWDP_CPU==_LWDP_I960KB || \
	 _LWDP_CPU==_LWDP_I960JX || \
	 _LWDP_CPU==_LWDP_I960HX)
#define	_LWDP_CPU_FAMILY	_LWDP_I960
#endif	/* _LWDP_CPU_FAMILY==_LWDP_I960 */

#if	(_LWDP_CPU==_LWDP_TRON || \
     _LWDP_CPU==_LWDP_G100 || \
	 _LWDP_CPU==_LWDP_G200)
#define	_LWDP_CPU_FAMILY	_LWDP_TRON
#endif	/* _LWDP_CPU_FAMILY==_LWDP_TRON */

#if	(_LWDP_CPU==_LWDP_MIPS || \
     _LWDP_CPU==_LWDP_MIPS32 || \
	 _LWDP_CPU==_LWDP_MIPS64)
#define	_LWDP_CPU_FAMILY	_LWDP_MIPS
#endif	/* _LWDP_CPU_FAMILY==_LWDP_MIPS */

#if	(_LWDP_CPU==_LWDP_AM29XXX || \
     _LWDP_CPU==_LWDP_AM29030 || \
	 _LWDP_CPU==_LWDP_AM29200 || \
	 _LWDP_CPU==_LWDP_AM29035)
#define	_LWDP_CPU_FAMILY	_LWDP_AM29XXX
#endif	/* _LWDP_CPU_FAMILY==_LWDP_AM29XXX */

#if	(_LWDP_CPU==_LWDP_SIMSPARCSUNOS)
#define	_LWDP_CPU_FAMILY	_LWDP_SIMSPARCSUNOS
#endif	/* _LWDP_CPU_FAMILY==_LWDP_SIMSPARCSUNOS */

#if	(_LWDP_CPU==_LWDP_SIMSPARCSOLARIS)
#define	_LWDP_CPU_FAMILY	_LWDP_SIMSPARCSOLARIS
#endif	/* _LWDP_CPU_FAMILY==_LWDP_SIMSPARCSOLARIS */

#if	(_LWDP_CPU==_LWDP_SIMHPPA)
#define	_LWDP_CPU_FAMILY	_LWDP_SIMHPPA
#endif	/* _LWDP_CPU_FAMILY==_LWDP_SIMHPPA */

#if	(_LWDP_CPU==_LWDP_I80X86   || \
     _LWDP_CPU==_LWDP_I80386   || \
	 _LWDP_CPU==_LWDP_I80486   || \
	 _LWDP_CPU==_LWDP_PENTIUM  || \
	 _LWDP_CPU==_LWDP_PENTIUM2 || \
	 _LWDP_CPU==_LWDP_PENTIUM3 || \
	 _LWDP_CPU==_LWDP_PENTIUM4)
#define	_LWDP_CPU_FAMILY	_LWDP_I80X86
#endif	/* _LWDP_CPU_FAMILY==_LWDP_I80X86 */

#if	(_LWDP_CPU==_LWDP_PPC    || \
     _LWDP_CPU==_LWDP_PPC32  || \
	 _LWDP_CPU==_LWDP_PPC601 || \
	 _LWDP_CPU==_LWDP_PPC602 || \
	 _LWDP_CPU==_LWDP_PPC603 || \
	 _LWDP_CPU==_LWDP_PPC604 || \
	 _LWDP_CPU==_LWDP_PPC403 || \
	 _LWDP_CPU==_LWDP_PPC505 || \
	 _LWDP_CPU==_LWDP_PPC860 || \
	 _LWDP_CPU==_LWDP_PPCEC603 || \
	 _LWDP_CPU==_LWDP_PPC555 || \
	 _LWDP_CPU==_LWDP_PPC509 || \
	 _LWDP_CPU==_LWDP_PPC405 || \
	 _LWDP_CPU==_LWDP_PPC405F || \
	 _LWDP_CPU==_LWDP_PPC440 || \
	 _LWDP_CPU==_LWDP_PPC85XX || \
	 _LWDP_CPU==_LWDP_PPC970 || \
	 _LWDP_CPU==_LWDP_PPC750 || \
	 _LWDP_CPU==_LWDP_PPC7400 || \
	 _LWDP_CPU==_LWDP_PPC8260 || \
	 _LWDP_CPU==_LWDP_PPC8560)
#define	_LWDP_CPU_FAMILY	_LWDP_PPC
#endif	/* _LWDP_CPU_FAMILY==_LWDP_PPC */

#if	(_LWDP_CPU==_LWDP_SIMNT)
#define	_LWDP_CPU_FAMILY	_LWDP_SIMNT
#endif	/* _LWDP_CPU_FAMILY==_LWDP_SIMNT */

#if	(_LWDP_CPU==_LWDP_ULTRASPARC)
#define	_LWDP_CPU_FAMILY	_LWDP_SPARCV9
#endif	/* _LWDP_CPU_FAMILY==_LWDP_SPARCV9 */

#if	(_LWDP_CPU==_LWDP_ARM      || \
     _LWDP_CPU==_LWDP_ARM710A  || \
	 _LWDP_CPU==_LWDP_ARM7TDMI || \
	 _LWDP_CPU==_LWDP_ARM810   || \
	 _LWDP_CPU==_LWDP_ARMSA110 || \
	 _LWDP_CPU==_LWDP_ARM7TDMI_T || \
	 _LWDP_CPU==_LWDP_ARMARCH3 || \
	 _LWDP_CPU==_LWDP_ARMARCH4 || \
	 _LWDP_CPU==_LWDP_ARMARCH4_T || \
	 _LWDP_CPU==_LWDP_STRONGARM || \
	 _LWDP_CPU==_LWDP_XSCALE   || \
	 _LWDP_CPU==_LWDP_ARMARCH5 || \
	 _LWDP_CPU==_LWDP_ARMARCH5_T || \
	 _LWDP_CPU==_LWDP_ARMARCH6 || \
	 _LWDP_CPU==_LWDP_ARMARCH6_T)
#define	_LWDP_CPU_FAMILY	_LWDP_ARM
#endif	/* _LWDP_CPU_FAMILY==_LWDP_ARM */

#if	(_LWDP_CPU==_LWDP_SH     || \
     _LWDP_CPU==_LWDP_SH32   || \
	 _LWDP_CPU==_LWDP_SH7000 || \
	 _LWDP_CPU==_LWDP_SH7600 || \
	 _LWDP_CPU==_LWDP_SH7040 || \
	 _LWDP_CPU==_LWDP_SH7700 || \
	 _LWDP_CPU==_LWDP_SH7410 || \
	 _LWDP_CPU==_LWDP_SH7729 || \
	 _LWDP_CPU==_LWDP_SH7750)
#define	_LWDP_CPU_FAMILY	_LWDP_SH
#endif	/* _LWDP_CPU_FAMILY==_LWDP_SH */

#if	(_LWDP_CPU==_LWDP_MCORE    || \
     _LWDP_CPU==_LWDP_MCORE10  || \
	 _LWDP_CPU==_LWDP_MCORE15  || \
	 _LWDP_CPU==_LWDP_MCORE200 || \
	 _LWDP_CPU==_LWDP_MCORE300)
#define	_LWDP_CPU_FAMILY	_LWDP_MCORE
#endif	/* _LWDP_CPU_FAMILY==_LWDP_MCORE */

#if	(_LWDP_CPU==_LWDP_COLDFIRE || \
     _LWDP_CPU==_LWDP_MCF5200  || \
	 _LWDP_CPU==_LWDP_MCF5400)
#define	_LWDP_CPU_FAMILY	_LWDP_COLDFIRE
#endif	/* _LWDP_CPU_FAMILY==_LWDP_COLDFIRE */

#if	(_LWDP_CPU==_LWDP_FRV || \
     _LWDP_CPU==_LWDP_FR500)
#define	_LWDP_CPU_FAMILY	_LWDP_FRV
#endif	/* _LWDP_CPU_FAMILY==_LWDP_FRV */

#if	(_LWDP_CPU==_LWDP_MAP || \
     _LWDP_CPU==_LWDP_MAP1000 || \
	 _LWDP_CPU==_LWDP_MAP1000A || \
	 _LWDP_CPU==_LWDP_MAPCA)
#define	_LWDP_CPU_FAMILY	_LWDP_MAP
#endif	/* _LWDP_CPU_FAMILY==_LWDP_MAP */

#if	(_LWDP_CPU==_LWDP_SIMLINUX)
#define	_LWDP_CPU_FAMILY	_LWDP_SIMLINUX
#endif	/* _LWDP_CPU_FAMILY==_LWDP_SIMLINUX */

#if	(_LWDP_CPU==_LWDP_SIMPENTIUM)
#define	_LWDP_CPU_FAMILY	_LWDP_SIMPENTIUM
#endif	/* _LWDP_CPU_FAMILY==_LWDP_SIMPENTIUM */


/*
_LWDP_CPU_LE means that CPU is LITTLE ENDIAN.
If _LWDP_CPU_LE is not defined, we don't know about that property of platform (it can be LITTLE ENDIAN).

_LWDP_CPU_LE_UNALIGN means that CPU is LITTLE ENDIAN and CPU supports unaligned memory accesses.
If _LWDP_CPU_LE_UNALIGN is not defined, we don't know about these properties of platform.
*/

#if defined(_M_X64) || defined(_M_AMD64) || defined(__x86_64__)
#define _LWDP_CPU_AMD64
#endif

#if defined(_LWDP_CPU_AMD64) || defined(_M_IA64)
#define _LWDP_CPU_64BIT
#endif

#if (_LWDP_CPU_FAMILY==_LWDP_I80X86) || defined(_LWDP_CPU_AMD64)
#define _LWDP_CPU_X86_OR_AMD64
#endif

#if (_LWDP_CPU_FAMILY==_LWDP_I80X86) || (_LWDP_CPU_FAMILY==_LWDP_ARM)
#define _LWDP_CPU_32BIT
#endif

#if defined(_WIN32) && (_LWDP_CPU_FAMILY==_LWDP_ARM)
#define _LWDP_CPU_ARM_LE
#endif

#if defined(_WIN32) && defined(_M_IA64)
#define _LWDP_CPU_IA64_LE
#endif

#if defined(_LWDP_CPU_X86_OR_AMD64)
#define _LWDP_CPU_LE_UNALIGN
#endif

#if defined(_LWDP_CPU_X86_OR_AMD64) || defined(_LWDP_CPU_ARM_LE)  || defined(_LWDP_CPU_IA64_LE) || defined(__ARMEL__) || defined(__MIPSEL__) || defined(__LITTLE_ENDIAN__)
#define _LWDP_CPU_LE
#endif

#if defined(__BIG_ENDIAN__)
#define _LWDP_CPU_BE
#endif

#if defined(_LWDP_CPU_LE) && defined(_LWDP_CPU_BE)
Stop_Compiling_Bad_Endian
#endif


#ifdef _LWDP_CPU_LE_UNALIGN

#define GetUi16(p) (*(const uint16_ *)(p))
#define GetUi32(p) (*(const uint32_ *)(p))
#define GetUi64(p) (*(const uint64_ *)(p))
#define SetUi16(p, d) *(uint16_ *)(p) = (d);
#define SetUi32(p, d) *(uint32_ *)(p) = (d);
#define SetUi64(p, d) *(uint64_ *)(p) = (d);

#else

#define GetUi16(p) (((const byte_ *)(p))[0] | ((uint16_)((const byte_ *)(p))[1] << 8))

#define GetUi32(p) ( \
             ((const byte_ *)(p))[0]        | \
    ((uint32_)((const byte_ *)(p))[1] <<  8) | \
    ((uint32_)((const byte_ *)(p))[2] << 16) | \
    ((uint32_)((const byte_ *)(p))[3] << 24))

#define GetUi64(p) (GetUi32(p) | ((uint64_)GetUi32(((const byte_ *)(p)) + 4) << 32))

#define SetUi16(p, d) { uint32_ _x_ = (d); \
    ((byte_ *)(p))[0] = (byte_)_x_; \
    ((byte_ *)(p))[1] = (byte_)(_x_ >> 8); }

#define SetUi32(p, d) { uint32_ _x_ = (d); \
    ((byte_ *)(p))[0] = (byte_)_x_; \
    ((byte_ *)(p))[1] = (byte_)(_x_ >> 8); \
    ((byte_ *)(p))[2] = (byte_)(_x_ >> 16); \
    ((byte_ *)(p))[3] = (byte_)(_x_ >> 24); }

#define SetUi64(p, d) { uint64_ _x64_ = (d); \
    SetUi32(p, (uint32_)_x64_); \
    SetUi32(((byte_ *)(p)) + 4, (uint32_)(_x64_ >> 32)); }

#endif

#if defined(_LWDP_CPU_LE_UNALIGN) && defined(_WIN64) && (_MSC_VER >= 1300)

#pragma intrinsic(_byteswap_ulong)
#pragma intrinsic(_byteswap_uint64)
#define GetBe32(p) _byteswap_ulong(*(const uint32_ *)(const byte_ *)(p))
#define GetBe64(p) _byteswap_uint64(*(const uint64_ *)(const byte_ *)(p))

#else

#define GetBe32(p) ( \
    ((uint32_)((const byte_ *)(p))[0] << 24) | \
    ((uint32_)((const byte_ *)(p))[1] << 16) | \
    ((uint32_)((const byte_ *)(p))[2] <<  8) | \
             ((const byte_ *)(p))[3] )

#define GetBe64(p) (((uint64_)GetBe32(p) << 32) | GetBe32(((const byte_ *)(p)) + 4))

#endif

#define GetBe16(p) (((uint16_)((const byte_ *)(p))[0] << 8) | ((const byte_ *)(p))[1])


LWDP_NAMESPACE_END;

#endif

