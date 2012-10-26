################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ExternalModuleExample.cpp 

OBJS += \
./src/ExternalModuleExample.o 

CPP_DEPS += \
./src/ExternalModuleExample.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DTEST_LWDP_EXTERNAL -DLWDP_DEBUG_MACRO -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


