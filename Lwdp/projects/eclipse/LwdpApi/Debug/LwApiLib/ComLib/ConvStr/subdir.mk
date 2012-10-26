################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/LwApiLib/ComLib/ConvStr/IntToString.cpp \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/LwApiLib/ComLib/ConvStr/MyString.cpp \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/LwApiLib/ComLib/ConvStr/StringConvert.cpp \
/mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/LwApiLib/ComLib/ConvStr/StringToInt.cpp 

OBJS += \
./LwApiLib/ComLib/ConvStr/IntToString.o \
./LwApiLib/ComLib/ConvStr/MyString.o \
./LwApiLib/ComLib/ConvStr/StringConvert.o \
./LwApiLib/ComLib/ConvStr/StringToInt.o 

CPP_DEPS += \
./LwApiLib/ComLib/ConvStr/IntToString.d \
./LwApiLib/ComLib/ConvStr/MyString.d \
./LwApiLib/ComLib/ConvStr/StringConvert.d \
./LwApiLib/ComLib/ConvStr/StringToInt.d 


# Each subdirectory must supply rules for building sources it contributes
LwApiLib/ComLib/ConvStr/IntToString.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/LwApiLib/ComLib/ConvStr/IntToString.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_DEBUG_MACRO -D__i386__ -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -I../../../../code/h/LwApiLib/ComLib/log4cpp -I../../../../code/h/LwApiLib/ComLib/tinyxpath -O0 -g3 -Wall -c -fmessage-length=0  -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

LwApiLib/ComLib/ConvStr/MyString.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/LwApiLib/ComLib/ConvStr/MyString.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_DEBUG_MACRO -D__i386__ -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -I../../../../code/h/LwApiLib/ComLib/log4cpp -I../../../../code/h/LwApiLib/ComLib/tinyxpath -O0 -g3 -Wall -c -fmessage-length=0  -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

LwApiLib/ComLib/ConvStr/StringConvert.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/LwApiLib/ComLib/ConvStr/StringConvert.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_DEBUG_MACRO -D__i386__ -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -I../../../../code/h/LwApiLib/ComLib/log4cpp -I../../../../code/h/LwApiLib/ComLib/tinyxpath -O0 -g3 -Wall -c -fmessage-length=0  -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

LwApiLib/ComLib/ConvStr/StringToInt.o: /mnt/hgfs/tmp/workspace/LwdpGit/Lwdp/code/LwApiLib/ComLib/ConvStr/StringToInt.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DLWDP_DEBUG_MACRO -D__i386__ -I../../../../code/h -I../../../../code/h/LwApiLib/ComLib -I../../../../code/h/LwApiLib/ComLib/log4cpp -I../../../../code/h/LwApiLib/ComLib/tinyxpath -O0 -g3 -Wall -c -fmessage-length=0  -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


