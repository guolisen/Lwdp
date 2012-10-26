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
./Cx_ObjectFactory.o \
./Cx_PluginLoader.o \
./DelayLoad.o \
./PluginModule.o 

CPP_DEPS += \
./Cx_ObjectFactory.d \
./Cx_PluginLoader.d \
./DelayLoad.d \
./PluginModule.d 


# Each subdirectory must supply rules for building sources it contributes
Cx_ObjectFactory.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/PluginLoader/Cx_ObjectFactory.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_PLUGIN_CONTEXT -DLWDP_DEBUG_MACRO -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Cx_PluginLoader.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/PluginLoader/Cx_PluginLoader.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_PLUGIN_CONTEXT -DLWDP_DEBUG_MACRO -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

DelayLoad.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/PluginLoader/DelayLoad.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_PLUGIN_CONTEXT -DLWDP_DEBUG_MACRO -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

PluginModule.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/PluginLoader/PluginModule.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_PLUGIN_CONTEXT -DLWDP_DEBUG_MACRO -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


