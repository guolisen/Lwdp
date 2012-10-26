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
./ScriptMgr/Cx_ScriptMgr.o \
./ScriptMgr/ScriptAction.o \
./ScriptMgr/ScriptEngine.o \
./ScriptMgr/ScriptMgrModule.o 

CPP_DEPS += \
./ScriptMgr/Cx_ScriptMgr.d \
./ScriptMgr/ScriptAction.d \
./ScriptMgr/ScriptEngine.d \
./ScriptMgr/ScriptMgrModule.d 


# Each subdirectory must supply rules for building sources it contributes
ScriptMgr/Cx_ScriptMgr.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/ScriptMgr/Cx_ScriptMgr.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DTEST_LWDP_EXTERNAL -DLWDP_DEBUG_MACRO -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib/log4cpp -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib/tinyxml -O0 -g3 -Wall -c -fmessage-length=0 -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

ScriptMgr/ScriptAction.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/ScriptMgr/ScriptAction.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DTEST_LWDP_EXTERNAL -DLWDP_DEBUG_MACRO -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib/log4cpp -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib/tinyxml -O0 -g3 -Wall -c -fmessage-length=0 -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

ScriptMgr/ScriptEngine.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/ScriptMgr/ScriptEngine.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DTEST_LWDP_EXTERNAL -DLWDP_DEBUG_MACRO -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib/log4cpp -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib/tinyxml -O0 -g3 -Wall -c -fmessage-length=0 -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

ScriptMgr/ScriptMgrModule.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/ScriptMgr/ScriptMgrModule.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DTEST_LWDP_EXTERNAL -DLWDP_DEBUG_MACRO -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib/log4cpp -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib/tinyxml -O0 -g3 -Wall -c -fmessage-length=0 -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


