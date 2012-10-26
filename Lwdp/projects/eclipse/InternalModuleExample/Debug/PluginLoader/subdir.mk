################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/PluginLoader/Cx_ObjectFactory.cpp \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/PluginLoader/Cx_PluginLoader.cpp \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/PluginLoader/DelayLoad.cpp \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/PluginLoader/PluginModule.cpp 

OBJS += \
./PluginLoader/Cx_ObjectFactory.o \
./PluginLoader/Cx_PluginLoader.o \
./PluginLoader/DelayLoad.o \
./PluginLoader/PluginModule.o 

CPP_DEPS += \
./PluginLoader/Cx_ObjectFactory.d \
./PluginLoader/Cx_PluginLoader.d \
./PluginLoader/DelayLoad.d \
./PluginLoader/PluginModule.d 


# Each subdirectory must supply rules for building sources it contributes
PluginLoader/Cx_ObjectFactory.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/PluginLoader/Cx_ObjectFactory.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DTEST_LWDP_EXTERNAL -DLWDP_DEBUG_MACRO -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib/log4cpp -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib/tinyxml -O0 -g3 -Wall -c -fmessage-length=0 -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

PluginLoader/Cx_PluginLoader.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/PluginLoader/Cx_PluginLoader.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DTEST_LWDP_EXTERNAL -DLWDP_DEBUG_MACRO -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib/log4cpp -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib/tinyxml -O0 -g3 -Wall -c -fmessage-length=0 -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

PluginLoader/DelayLoad.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/PluginLoader/DelayLoad.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DTEST_LWDP_EXTERNAL -DLWDP_DEBUG_MACRO -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib/log4cpp -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib/tinyxml -O0 -g3 -Wall -c -fmessage-length=0 -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

PluginLoader/PluginModule.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/PluginLoader/PluginModule.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DTEST_LWDP_EXTERNAL -DLWDP_DEBUG_MACRO -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib/log4cpp -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib/tinyxml -O0 -g3 -Wall -c -fmessage-length=0 -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


