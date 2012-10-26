################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include LwApiLib/Module/subdir.mk
-include LwApiLib/LwApi/common/subdir.mk
-include LwApiLib/LwApi/Linux/subdir.mk
-include LwApiLib/Framwork/PluginMgr/subdir.mk
-include LwApiLib/Framwork/Lwlog/subdir.mk
-include LwApiLib/Framwork/FwEntry/subdir.mk
-include LwApiLib/Framwork/ErrMgr/subdir.mk
-include LwApiLib/Framwork/subdir.mk
-include LwApiLib/ComLib/tinyxpath/subdir.mk
-include LwApiLib/ComLib/lua/subdir.mk
-include LwApiLib/ComLib/log4cpp/subdir.mk
-include LwApiLib/ComLib/ConvStr/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: ../../../../code/bin/lib/libLwdpApi.a

# Tool invocations
../../../../code/bin/lib/libLwdpApi.a: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC Archiver'
	ar -r  "../../../../code/bin/lib/libLwdpApi.a" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(ARCHIVES)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS) ../../../../code/bin/lib/libLwdpApi.a
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
