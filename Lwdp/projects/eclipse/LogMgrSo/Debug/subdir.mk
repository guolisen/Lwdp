################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/LogMgr/Cx_LogMgr.cpp \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/LogMgr/LogMgrModule.cpp \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/LogMgr/MyAppender.cpp \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/LogMgr/Plugin.cpp 

OBJS += \
./Cx_LogMgr.o \
./LogMgrModule.o \
./MyAppender.o \
./Plugin.o 

CPP_DEPS += \
./Cx_LogMgr.d \
./LogMgrModule.d \
./MyAppender.d \
./Plugin.d 


# Each subdirectory must supply rules for building sources it contributes
Cx_LogMgr.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/LogMgr/Cx_LogMgr.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_PLUGIN_CONTEXT -DLWDP_DEBUG_MACRO -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -I../../../../code/h/LwApiLib/ComLib/log4cpp -O0 -g3 -Wall -c -fmessage-length=0 -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

LogMgrModule.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/LogMgr/LogMgrModule.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_PLUGIN_CONTEXT -DLWDP_DEBUG_MACRO -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -I../../../../code/h/LwApiLib/ComLib/log4cpp -O0 -g3 -Wall -c -fmessage-length=0 -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

MyAppender.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/LogMgr/MyAppender.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_PLUGIN_CONTEXT -DLWDP_DEBUG_MACRO -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -I../../../../code/h/LwApiLib/ComLib/log4cpp -O0 -g3 -Wall -c -fmessage-length=0 -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Plugin.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/LogMgr/Plugin.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_PLUGIN_CONTEXT -DLWDP_DEBUG_MACRO -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -I../../../../code/h/LwApiLib/ComLib/log4cpp -O0 -g3 -Wall -c -fmessage-length=0 -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


