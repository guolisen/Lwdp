/*! \file LwTypeDef.h
 *  \brief Platform Api
 *  \author Guolisen, LwDp
 *  \date   2011.07.14
 */

#ifndef LW_API_H_
#define LW_API_H_

#define Api_InterlockedIncrement    Lwdp_InterlockedIncrement
#define Api_InterlockedDecrement    Lwdp_InterlockedDecrement
#define Api_FreeLibrary             Lwdp_FreeLibrary
#define Api_GetProcAddress          Lwdp_GetProcAddress

#define Api_TaskDelay               Lwdp_TaskDelay
#define Api_HaltSystem              Lwdp_HaltSystem

#define Api_LoadLibrary             Lwdp_LoadLibrary
#define Api_PathFindFileName        Lwdp_PathFindFileName

#define Api_strlwr		Lwdp_strlwr	      
#define Api_strupr		Lwdp_strupr	 
#define Api_strlen		Lwdp_strlen  
#define Api_tcscmp		Lwdp_tcscmp 	
#define Api_tcsncmp		Lwdp_tcsncmp	  
#define Api_tcscpy		Lwdp_tcscpy       
#define Api_tcsncpy		Lwdp_tcsncpy     
#define Api_tcscat		Lwdp_tcscat       

#define Api_isalnum		Lwdp_isalnum       
#define Api_isdigit 	Lwdp_isdigit       
#define Api_isalpha 	Lwdp_isalpha       

#define Api_fprintf		Lwdp_fprintf
#define Api_sprintf		Lwdp_sprintf  
#define Api_vsprintf	Lwdp_vstprintf    
#define Api_ltot		Lwdp_ltot        
#define Api_itot		Lwdp_itot         


#ifdef __VXWORKS__
#include <LwApiLib/LwApi/VxWorks/Api4Vxworks.h>
#elif (WIN32 || _WIN32)
#include <LwApiLib/LwApi/Win32/Api4Win32.h>
#include <LwApiLib/LwApi/Win32/CsOsApi.h>  //Vxworks Api
#elif __linux__
#include <LwApiLib/LwApi/Linux/Api4Linux.h>
#elif defined (MACOS) || defined (_MAC)
#include <LwApiLib/LwApi/Mac/NacImpl.h>
#endif

#include <LwApiLib/LwApi/LwApiCommon.h>

#endif




 

