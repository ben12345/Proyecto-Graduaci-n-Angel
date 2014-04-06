################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := ${ComSpec}

-include ../makefile.init

RM := clean

# All of the sources participating in the build are defined here
-include sources.mk
-include system/startup_ldf/subdir.mk
-include system/pinmux/GeneratedSources/subdir.mk
-include system/mcapi/GeneratedSources/subdir.mk
-include system/Video_Encoder_EI3/Blackfin/src/drivers/encoder/adv7511/subdir.mk
-include system/Camera_EI3/drivers/sensor/aptina/mt9m114/subdir.mk
-include system/subdir.mk
-include common/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(DSP_DEPS)),)
-include $(DSP_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LINKER_LOG_OUTPUTS += \
linker_log.xml \


# All Target
all: VisionApp_Core0.dxe

# Tool invocations
VisionApp_Core0.dxe: $(OBJS) $(LDF_SRCS)
	@echo 'Building target: $@'
	@echo 'Invoking: CrossCore Blackfin Linker'
	ccblkfn.exe -proc ADSP-BF609 -si-revision 0.0 -TC:/Users/Usuario/Music/PG/Demo_1/BDTI_DiceDotCountingDemo/CannyEdgeDetection/VisionApp_Core0/system/startup_ldf/app.ldf -no-mem -map VisionApp_Core0.map.xml -LC:/Users/Usuario/Music/PG/Demo_1/BDTI_DiceDotCountingDemo/CannyEdgeDetection/VisionApp_Core0/Debug/system/startup_ldf -LC:/Analog\ Devices/SoftwareModules/VisionAnalyticsToolbox-BF-Rel2.4.0/src/Build-CCES -LC:/Analog\ Devices/SoftwareModules/VisionAnalyticsToolbox-BF-Rel2.4.0/Lib-Production-CCES -LC:/Analog\ Devices/SoftwareModules/Graphics2D_OGL-BF-Rel3.4.0/Lib-Production-CCES -LC:/Analog\ Devices/SoftwareModules/ImageProcessingToolbox-BF-Rel2.3.0/Lib-Production-CCES -flags-link -MDDEBUG,-MDADI_MCAPI -flags-link -e -add-debug-libpaths -flags-link libadi_vat_bf.dlb, -flags-link libadi_graphics2d.dlb, -flags-link libadi_image_tool_box_cces.dlb -o VisionApp_Core0.dxe $(OBJS) $(LDF_SRCS)  -flags-link -od,.\.
	@echo 'Finished building target: $@'
	@echo ' '

linker_log.xml: VisionApp_Core0.dxe $(OBJS) $(LDF_SRCS)

# Other Targets
clean:
	-$(RM) $(C++_DEPS)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(CC_DEPS)$(DSP_DEPS)$(DXE)$(CPP_DEPS)$(LINKER_LOG_OUTPUTS)$(S_DEPS)$(CXX_DEPS) VisionApp_Core0.dxe
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
