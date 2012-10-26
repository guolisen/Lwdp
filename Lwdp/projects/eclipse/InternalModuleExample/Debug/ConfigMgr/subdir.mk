################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/ConfigMgr/ConfigModule.cpp \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/ConfigMgr/Cx_ConfigMgr.cpp \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/ConfigMgr/Cx_XmlMgr.cpp 

OBJS += \
./ConfigMgr/ConfigModule.o \
./ConfigMgr/Cx_ConfigMgr.o \
./ConfigMgr/Cx_XmlMgr.o 

CPP_DEPS += \
./ConfigMgr/ConfigModule.d \
./ConfigMgr/Cx_ConfigMgr.d \
./ConfigMgr/Cx_XmlMgr.d 


# Each subdirectory must supply rules for building sources it contributes
ConfigMgr/ConfigModule.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/ConfigMgr/ConfigModule.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DTEST_LWDP_EXTERNAL -DLWDP_DEBUG_MACRO -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib/log4cpp -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib/tinyxml -O0 -g3 -Wall -c -fmessage-length=0 -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

ConfigMgr/Cx_ConfigMgr.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/ConfigMgr/Cx_ConfigMgr.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DTEST_LWDP_EXTERNAL -DLWDP_DEBUG_MACRO -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib/log4cpp -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib/tinyxml -O0 -g3 -Wall -c -fmessage-length=0 -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

ConfigMgr/Cx_XmlMgr.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/ConfigMgr/Cx_XmlMgr.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DTEST_LWDP_EXTERNAL -DLWDP_DEBUG_MACRO -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib/log4cpp -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib/tinyxml -O0 -g3 -Wall -c -fmessage-length=0 -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


