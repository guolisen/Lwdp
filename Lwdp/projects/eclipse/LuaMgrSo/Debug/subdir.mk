################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/LuaMgr/Cx_LuaMgr.cpp \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/LuaMgr/LuaMgrModule.cpp 

OBJS += \
./Cx_LuaMgr.o \
./LuaMgrModule.o 

CPP_DEPS += \
./Cx_LuaMgr.d \
./LuaMgrModule.d 


# Each subdirectory must supply rules for building sources it contributes
Cx_LuaMgr.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/LuaMgr/Cx_LuaMgr.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_PLUGIN_CONTEXT -DLWDP_DEBUG_MACRO -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

LuaMgrModule.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/LuaMgr/LuaMgrModule.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_PLUGIN_CONTEXT -DLWDP_DEBUG_MACRO -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


