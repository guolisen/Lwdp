################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/LwApiLib/LwApi/common/LwApiDebug.cpp \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/LwApiLib/LwApi/common/LwApiSys.cpp 

OBJS += \
./LwApiLib/LwApi/common/LwApiDebug.o \
./LwApiLib/LwApi/common/LwApiSys.o 

CPP_DEPS += \
./LwApiLib/LwApi/common/LwApiDebug.d \
./LwApiLib/LwApi/common/LwApiSys.d 


# Each subdirectory must supply rules for building sources it contributes
LwApiLib/LwApi/common/LwApiDebug.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/LwApiLib/LwApi/common/LwApiDebug.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_DEBUG_MACRO -D__i386__ -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -I../../../../code/h/LwApiLib/ComLib/log4cpp -I../../../../code/h/LwApiLib/ComLib/tinyxpath -O0 -g3 -Wall -c -fmessage-length=0  -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

LwApiLib/LwApi/common/LwApiSys.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/LwApiLib/LwApi/common/LwApiSys.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_DEBUG_MACRO -D__i386__ -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -I../../../../code/h/LwApiLib/ComLib/log4cpp -I../../../../code/h/LwApiLib/ComLib/tinyxpath -O0 -g3 -Wall -c -fmessage-length=0  -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


