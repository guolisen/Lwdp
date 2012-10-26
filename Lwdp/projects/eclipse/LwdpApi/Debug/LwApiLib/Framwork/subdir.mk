################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/LwApiLib/Framwork/Cx_FwIntf.cpp \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/LwApiLib/Framwork/FwEntryModule.cpp 

OBJS += \
./LwApiLib/Framwork/Cx_FwIntf.o \
./LwApiLib/Framwork/FwEntryModule.o 

CPP_DEPS += \
./LwApiLib/Framwork/Cx_FwIntf.d \
./LwApiLib/Framwork/FwEntryModule.d 


# Each subdirectory must supply rules for building sources it contributes
LwApiLib/Framwork/Cx_FwIntf.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/LwApiLib/Framwork/Cx_FwIntf.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_DEBUG_MACRO -D__i386__ -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -I../../../../code/h/LwApiLib/ComLib/log4cpp -I../../../../code/h/LwApiLib/ComLib/tinyxpath -O0 -g3 -Wall -c -fmessage-length=0  -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

LwApiLib/Framwork/FwEntryModule.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/LwApiLib/Framwork/FwEntryModule.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_DEBUG_MACRO -D__i386__ -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -I../../../../code/h/LwApiLib/ComLib/log4cpp -I../../../../code/h/LwApiLib/ComLib/tinyxpath -O0 -g3 -Wall -c -fmessage-length=0  -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


