################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/ScriptMgr/Cx_ScriptMgr.cpp \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/ScriptMgr/ScriptAction.cpp \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/ScriptMgr/ScriptEngine.cpp \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/ScriptMgr/ScriptMgrModule.cpp 

OBJS += \
./Cx_ScriptMgr.o \
./ScriptAction.o \
./ScriptEngine.o \
./ScriptMgrModule.o 

CPP_DEPS += \
./Cx_ScriptMgr.d \
./ScriptAction.d \
./ScriptEngine.d \
./ScriptMgrModule.d 


# Each subdirectory must supply rules for building sources it contributes
Cx_ScriptMgr.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/ScriptMgr/Cx_ScriptMgr.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_PLUGIN_CONTEXT -DLWDP_DEBUG_MACRO -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

ScriptAction.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/ScriptMgr/ScriptAction.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_PLUGIN_CONTEXT -DLWDP_DEBUG_MACRO -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

ScriptEngine.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/ScriptMgr/ScriptEngine.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_PLUGIN_CONTEXT -DLWDP_DEBUG_MACRO -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

ScriptMgrModule.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/ScriptMgr/ScriptMgrModule.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_PLUGIN_CONTEXT -DLWDP_DEBUG_MACRO -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


