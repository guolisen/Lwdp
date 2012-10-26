################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/LuaMgr/Cx_LuaMgr.cpp \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/LuaMgr/LuaMgrModule.cpp 

OBJS += \
./LuaMgr/Cx_LuaMgr.o \
./LuaMgr/LuaMgrModule.o 

CPP_DEPS += \
./LuaMgr/Cx_LuaMgr.d \
./LuaMgr/LuaMgrModule.d 


# Each subdirectory must supply rules for building sources it contributes
LuaMgr/Cx_LuaMgr.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/LuaMgr/Cx_LuaMgr.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DTEST_LWDP_EXTERNAL -DLWDP_DEBUG_MACRO -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib/log4cpp -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib/tinyxml -O0 -g3 -Wall -c -fmessage-length=0 -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

LuaMgr/LuaMgrModule.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/PModule/LuaMgr/LuaMgrModule.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DTEST_LWDP_EXTERNAL -DLWDP_DEBUG_MACRO -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib/log4cpp -I/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/h/LwApiLib/ComLib/tinyxml -O0 -g3 -Wall -c -fmessage-length=0 -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


