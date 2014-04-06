################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/startup_ldf/app_cplbtab.c \
../system/startup_ldf/app_handler_table.c \
../system/startup_ldf/app_heaptab.c 

S_SRCS += \
../system/startup_ldf/app_startup.s 

LDF_SRCS += \
../system/startup_ldf/app.ldf 

OBJS += \
./system/startup_ldf/app_cplbtab.doj \
./system/startup_ldf/app_handler_table.doj \
./system/startup_ldf/app_heaptab.doj \
./system/startup_ldf/app_startup.doj 

C_DEPS += \
./system/startup_ldf/app_cplbtab.d \
./system/startup_ldf/app_handler_table.d \
./system/startup_ldf/app_heaptab.d 

S_DEPS += \
./system/startup_ldf/app_startup.d 


# Each subdirectory must supply rules for building sources it contributes
system/startup_ldf/%.doj: ../system/startup_ldf/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn.exe -c -file-attr ProjectName="VisionApp_Core0" -proc ADSP-BF609 -flags-compiler --no_wrap_diagnostics -si-revision 0.0 -g -DCORE0 -D_DEBUG -DADI_MCAPI -I"C:\Users\Usuario\Music\PG\Demo_1\BDTI_DiceDotCountingDemo\CannyEdgeDetection\VisionApp_Core0\system" -I"C:/Analog Devices/Video_Encoder_EI3_Extender_Board-Rel1.0.1/Video_Encoder_EI3/Blackfin/include" -I"C:/Analog Devices/SoftwareModules/VisionAnalyticsToolbox-BF-Rel2.4.0/include" -I"C:/Analog Devices/SoftwareModules/Graphics2D_OGL-BF-Rel3.4.0/Include" -I"C:/Analog Devices/SoftwareModules/ImageProcessingToolbox-BF-Rel2.3.0/Include" -I"C:/Analog Devices/Camera_EI3_Extender_Board-Rel1.0.1/Camera_EI3/Blackfin/include" -structs-do-not-overlap -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -I"../../VisionApp_Core0" -I"../../common" -gnu-style-dependencies -MD -Mo "$(basename $@).d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/startup_ldf/%.doj: ../system/startup_ldf/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin Assembler'
	easmblkfn.exe -file-attr ProjectName="VisionApp_Core0" -proc ADSP-BF609 -si-revision 0.0 -g -DCORE0 -D_DEBUG -DADI_MCAPI -i"C:\Users\Usuario\Music\PG\Demo_1\BDTI_DiceDotCountingDemo\CannyEdgeDetection\VisionApp_Core0\system" -i"C:/Analog Devices/Video_Encoder_EI3_Extender_Board-Rel1.0.1/Video_Encoder_EI3/Blackfin/include" -i"C:/Analog Devices/SoftwareModules/ImageProcessingToolbox-BF-Rel2.3.0/Include" -gnu-style-dependencies -MM -Mo "$(basename $@).d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


