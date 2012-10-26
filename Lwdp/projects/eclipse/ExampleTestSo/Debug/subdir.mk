################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/UModule/Win32DllTempl/Cx_Example.cpp \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/UModule/Win32DllTempl/Module.cpp \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/UModule/Win32DllTempl/Plugin.cpp 

OBJS += \
./Cx_Example.o \
./Module.o \
./Plugin.o 

CPP_DEPS += \
./Cx_Example.d \
./Module.d \
./Plugin.d 


# Each subdirectory must supply rules for building sources it contributes
Cx_Example.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/UModule/Win32DllTempl/Cx_Example.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_PLUGIN_CONTEXT -DLWDP_DEBUG_MACRO -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Module.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/UModule/Win32DllTempl/Module.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_PLUGIN_CONTEXT -DLWDP_DEBUG_MACRO -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Plugin.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/UModule/Win32DllTempl/Plugin.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_PLUGIN_CONTEXT -DLWDP_DEBUG_MACRO -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


