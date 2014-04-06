################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Analog\ Devices/Video_Encoder_EI3_Extender_Board-Rel1.0.1/Video_Encoder_EI3/Blackfin/src/drivers/encoder/adv7511/adi_adv7511.c 

OBJS += \
./system/Video_Encoder_EI3/Blackfin/src/drivers/encoder/adv7511/adi_adv7511.doj 

C_DEPS += \
./system/Video_Encoder_EI3/Blackfin/src/drivers/encoder/adv7511/adi_adv7511.d 


# Each subdirectory must supply rules for building sources it contributes
system/Video_Encoder_EI3/Blackfin/src/drivers/encoder/adv7511/adi_adv7511.doj: C:/Analog\ Devices/Video_Encoder_EI3_Extender_Board-Rel1.0.1/Video_Encoder_EI3/Blackfin/src/drivers/encoder/adv7511/adi_adv7511.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn.exe -c -file-attr ProjectName="VisionApp_Core0" -proc ADSP-BF609 -flags-compiler --no_wrap_diagnostics -si-revision 0.0 -O -Ov100 -DCORE0 -DNDEBUG -DADI_MCAPI -I"C:\Users\gregori\Desktop\Projects\ADI_Shire_Port\NewBoard\BDTI_DiceDotCountingDemo\ADI_CannyEdgeDetection\CannyEdgeDetection\VisionApp_Core0\system" -I"C:/Analog Devices/Video_Encoder_EI3_Extender_Board-Rel1.0.1/Video_Encoder_EI3/Blackfin/include" -I"C:/Analog Devices/Camera_EI3_Extender_Board-Rel1.0.1/Camera_EI3/Blackfin/include" -I"C:/Analog Devices/SoftwareModules/VisionAnalyticsToolbox-BF-Rel2.3.0/include" -structs-do-not-overlap -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -I"../../VisionApp_Core0" -I"../../common" -gnu-style-dependencies -MD -Mo "$(basename $@).d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


