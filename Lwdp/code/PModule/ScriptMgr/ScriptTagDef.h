/*! \file ScriptTagDef.h
 *  \brief  Script Tag Define
 *  \author Guolisen, Lwdp PluginFramework
 *  \date   2010.11.8
 */
#ifndef LWDP_SCRIPT_TAG_DEFINE_H_
#define LWDP_SCRIPT_TAG_DEFINE_H_

#define LW_DOMAIN_TOP_TAG 			__T("LWDP")
#define LW_DOMAIN_IF_TAG 			__T("IF")
#define LW_DOMAIN_ELSE_TAG 			__T("ELSE")
#define LW_DOMAIN_WHILE_TAG 		__T("WHILE")

#define LW_SCRIPT_RET_TAG 			__T("Ret")
#define LW_SCRIPT_VAR_TAG 			__T('@')
#define LW_SCRIPT_STR_TAG 			__T('\'')
#define LW_SCRIPT_FIGURE_TAG 		__T(':')
#define LW_SCRIPT_EXPR_TAG 			__T('$')

#define LWDP_ACTION_BREAK_RET 	    (LWRESULT)(-2)
#define LWDP_ACTION_CONTINUE_RET 	(LWRESULT)(-3)

#define LW_ACTION_BREAK_TAG 		__T("break")
#define LW_ACTION_BREAK_RET_TAG 	__T("break")

#define LW_ACTION_CONTINUE_TAG 		__T("continue")
#define LW_ACTION_CONTINUE_RET_TAG 	__T("continue")

#define LW_ACTION_WHILE_TAG 		__T("while")
#define LW_ACTION_WHILE_EXPR_TAG 	__T("Expr")

#define LW_ACTION_COMPUT_TAG 		__T("comput")
#define LW_ACTION_COMPUT_PARA_STEP 	__T("Step")

#define LW_ACTION_DEC_TAG 			__T("dec")
#define LW_ACTION_INC_TAG 			__T("inc")


#define LW_ACTION_IFELSE_TAG 		__T("if")
#define LW_ACTION_ELSE_TAG 			__T("else")
#define LW_ACTION_IFELSE_PARA_EXPR 	__T("Expr")

#define LW_ACTION_VARS_TAG 			__T("vars")
#define LW_ACTION_VARS_PARA_NAME 	__T("Name")

#define LW_ACTION_PRINT_TAG 		__T("printf")
#define LW_ACTION_PRINT_PARA_LEVEL 	__T("Level")

#define LW_ACTION_PRINT_LEVEL_TAG 	__T("set_print_level")
#define LW_ACTION_PRINT_LEVEL_PARA_LEVEL 	__T("Level")


#endif // LWDP_CONFIG_TAG_DEFINE_H_




