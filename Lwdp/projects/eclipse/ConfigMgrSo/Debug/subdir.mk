################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/ConfigMgr/ConfigModule.cpp \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/ConfigMgr/Cx_ConfigMgr.cpp \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/ConfigMgr/Cx_XmlMgr.cpp 

OBJS += \
./ConfigModule.o \
./Cx_ConfigMgr.o \
./Cx_XmlMgr.o 

CPP_DEPS += \
./ConfigModule.d \
./Cx_ConfigMgr.d \
./Cx_XmlMgr.d 


# Each subdirectory must supply rules for building sources it contributes
ConfigModule.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/ConfigMgr/ConfigModule.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_PLUGIN_CONTEXT -DLWDP_DEBUG_MACRO -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Cx_ConfigMgr.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/ConfigMgr/Cx_ConfigMgr.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_PLUGIN_CONTEXT -DLWDP_DEBUG_MACRO -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Cx_XmlMgr.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/ConfigMgr/Cx_XmlMgr.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_PLUGIN_CONTEXT -DLWDP_DEBUG_MACRO -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


