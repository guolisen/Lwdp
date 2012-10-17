
#include <LwDp.h>
#include <LwApiLib/LwApi/win32/Api4Win32.h>


LWDP_NAMESPACE_BEGIN;
EXTERN_C_BEGIN;

/****************************************************************************
*  Function:       WIN_TaskDelay
*  Description:    
*  Input:          无
*  Output:         // 对输出参数的说明
*  Return:         // 函数返回值的说明	
*  Others:         // 其它说明
*****************************************************************************/
void WIN_IMPL_API(TaskDelay)(ulong_ tick)
{
    ::Sleep(tick);
}

/****************************************************************************
*  Function:       WIN_HaltSystem
*  Description:    
*  Input:          无
*  Output:         // 对输出参数的说明
*  Return:         // 函数返回值的说明	
*  Others:         // 其它说明
*****************************************************************************/
void WIN_IMPL_API(HaltSystem)()
{
    while(1) {::Sleep(1000);}    
}

EXTERN_C_END;
LWDP_NAMESPACE_END;