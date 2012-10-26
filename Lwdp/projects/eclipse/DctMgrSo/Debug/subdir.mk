################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/DctMgr/Cx_DctMgr.cpp \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/DctMgr/DataCollectTool.cpp \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/DctMgr/DctMgrModule.cpp 

OBJS += \
./Cx_DctMgr.o \
./DataCollectTool.o \
./DctMgrModule.o 

CPP_DEPS += \
./Cx_DctMgr.d \
./DataCollectTool.d \
./DctMgrModule.d 


# Each subdirectory must supply rules for building sources it contributes
Cx_DctMgr.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/DctMgr/Cx_DctMgr.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_PLUGIN_CONTEXT -DLWDP_DEBUG_MACRO -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

DataCollectTool.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/DctMgr/DataCollectTool.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_PLUGIN_CONTEXT -DLWDP_DEBUG_MACRO -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

DctMgrModule.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/DctMgr/DctMgrModule.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_PLUGIN_CONTEXT -DLWDP_DEBUG_MACRO -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


