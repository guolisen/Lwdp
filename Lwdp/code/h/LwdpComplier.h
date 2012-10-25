#ifndef LWDP_COMPLIER_DEFINE
#define LWDP_COMPLIER_DEFINE

#if defined(__GCCXML__)
// GCC-XML emulates other compilers, it has to appear first here!
#   define LWDP_COMPILER_GCCXML 

#elif defined __CUDACC__
//  NVIDIA CUDA C++ compiler for GPU
#   define LWDP_COMPILER_NVCC

#elif defined __COMO__
//  Comeau C++
#   define LWDP_COMPILER_COMEAU

#elif defined(__PATHSCALE__) && (__PATHCC__ >= 4)
// PathScale EKOPath compiler (has to come before clang and gcc)
#   define LWDP_COMPILER_PATHSCALE

#elif defined __clang__
//  Clang C++ emulates GCC, so it has to appear early.
#   define LWDP_COMPILER_CLANG 

#elif defined __DMC__
//  Digital Mars C++
#   define LWDP_COMPILER_DIGITALMARS

#elif defined(__INTEL_COMPILER) || defined(__ICL) || defined(__ICC) || defined(__ECC)
//  Intel
#   define LWDP_COMPILER_INTEL 

# elif defined __GNUC__
//  GNU C++:
#   define LWDP_COMPILER_GCC 

#elif defined __KCC
//  Kai C++
#   define LWDP_COMPILER_KCC 

#elif defined __sgi
//  SGI MIPSpro C++
#   define LWDP_COMPILER_SGI 

#elif defined __DECCXX
//  Compaq Tru64 Unix cxx
#   define LWDP_COMPILER_DECCXX 

#elif defined __ghs
//  Greenhills C++
#   define LWDP_COMPILER_GHS 

#elif defined __CODEGEARC__
//  CodeGear - must be checked for before Borland
#   define LWDP_COMPILER_CODEGEAR 

#elif defined __BORLANDC__
//  Borland
#   define LWDP_COMPILER_BORLANDC 

#elif defined  __MWERKS__
//  Metrowerks CodeWarrior
#   define LWDP_COMPILER_METROWERKS 

#elif defined  __SUNPRO_CC
//  Sun Workshop Compiler C++
#   define LWDP_COMPILER_SUNPROC

#elif defined __HP_aCC
//  HP aCC
#   define LWDP_COMPILER_HPACC 

#elif defined(__MRC__) || defined(__SC__)
//  MPW MrCpp or SCpp
#   define LWDP_COMPILER_MPW

#elif defined(__IBMCPP__)
//  IBM Visual Age
#   define LWDP_COMPILER_IBMCPP 

#elif defined(__PGI)
//  Portland Group Inc.
#   define LWDP_COMPILER_PGI

#elif defined _MSC_VER
//  Microsoft Visual C++
//
//  Must remain the last #elif since some other vendors (Metrowerks, for
//  example) also #define _MSC_VER
#   define LWDP_COMPILER_MSC

#else
// this must come last - generate an error if we don't
// recognise the compiler:
#  error "Unknown compiler"

#endif

#endif


