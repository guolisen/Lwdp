################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/LwApiLib/Framwork/ErrMgr/LwErrMgr.cpp 

OBJS += \
./LwApiLib/Framwork/ErrMgr/LwErrMgr.o 

CPP_DEPS += \
./LwApiLib/Framwork/ErrMgr/LwErrMgr.d 


# Each subdirectory must supply rules for building sources it contributes
LwApiLib/Framwork/ErrMgr/LwErrMgr.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/LwApiLib/Framwork/ErrMgr/LwErrMgr.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_DEBUG_MACRO -D__i386__ -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -I../../../../code/h/LwApiLib/ComLib/log4cpp -I../../../../code/h/LwApiLib/ComLib/tinyxpath -O0 -g3 -Wall -c -fmessage-length=0  -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


