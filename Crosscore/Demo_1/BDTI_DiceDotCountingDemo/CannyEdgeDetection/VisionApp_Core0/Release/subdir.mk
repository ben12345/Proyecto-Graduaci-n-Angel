################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../EdgeDetection.c \
../Sensor.c \
../encoder.c \
C:/Users/gregori/Desktop/Projects/ADI_Shire_Port/NewBoard/BDTI_DiceDotCountingDemo/ADI_CannyEdgeDetection/CannyEdgeDetection/common/finboard_bsp.c \
../node_0.c 

OBJS += \
./EdgeDetection.doj \
./Sensor.doj \
./encoder.doj \
./finboard_bsp.doj \
./node_0.doj 

C_DEPS += \
./EdgeDetection.d \
./Sensor.d \
./encoder.d \
./finboard_bsp.d \
./node_0.d 


# Each subdirectory must supply rules for building sources it contributes
%.doj: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn.exe -c -file-attr ProjectName="VisionApp_Core0" -proc ADSP-BF609 -flags-compiler --no_wrap_diagnostics -si-revision 0.0 -O -Ov100 -DCORE0 -DNDEBUG -DADI_MCAPI -I"C:\Users\gregori\Desktop\Projects\ADI_Shire_Port\NewBoard\BDTI_DiceDotCountingDemo\ADI_CannyEdgeDetection\CannyEdgeDetection\VisionApp_Core0\system" -I"C:/Analog Devices/Video_Encoder_EI3_Extender_Board-Rel1.0.1/Video_Encoder_EI3/Blackfin/include" -I"C:/Analog Devices/Camera_EI3_Extender_Board-Rel1.0.1/Camera_EI3/Blackfin/include" -I"C:/Analog Devices/SoftwareModules/VisionAnalyticsToolbox-BF-Rel2.3.0/include" -structs-do-not-overlap -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -I"../../VisionApp_Core0" -I"../../common" -gnu-style-dependencies -MD -Mo "$(basename $@).d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

finboard_bsp.doj: C:/Users/gregori/Desktop/Projects/ADI_Shire_Port/NewBoard/BDTI_DiceDotCountingDemo/ADI_CannyEdgeDetection/CannyEdgeDetection/common/finboard_bsp.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn.exe -c -file-attr ProjectName="VisionApp_Core0" -proc ADSP-BF609 -flags-compiler --no_wrap_diagnostics -si-revision 0.0 -O -Ov100 -DCORE0 -DNDEBUG -DADI_MCAPI -I"C:\Users\gregori\Desktop\Projects\ADI_Shire_Port\NewBoard\BDTI_DiceDotCountingDemo\ADI_CannyEdgeDetection\CannyEdgeDetection\VisionApp_Core0\system" -I"C:/Analog Devices/Video_Encoder_EI3_Extender_Board-Rel1.0.1/Video_Encoder_EI3/Blackfin/include" -I"C:/Analog Devices/Camera_EI3_Extender_Board-Rel1.0.1/Camera_EI3/Blackfin/include" -I"C:/Analog Devices/SoftwareModules/VisionAnalyticsToolbox-BF-Rel2.3.0/include" -structs-do-not-overlap -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -I"../../VisionApp_Core0" -I"../../common" -gnu-style-dependencies -MD -Mo "$(basename $@).d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


