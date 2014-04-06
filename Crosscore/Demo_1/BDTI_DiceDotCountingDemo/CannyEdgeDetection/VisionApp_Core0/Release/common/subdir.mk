################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/gregori/Desktop/Projects/ADI_Shire_Port/NewBoard/BDTI_DiceDotCountingDemo/ADI_CannyEdgeDetection/CannyEdgeDetection/common/BF609_GPIO.c \
C:/Users/gregori/Desktop/Projects/ADI_Shire_Port/NewBoard/BDTI_DiceDotCountingDemo/ADI_CannyEdgeDetection/CannyEdgeDetection/common/PVPInit.c \
C:/Users/gregori/Desktop/Projects/ADI_Shire_Port/NewBoard/BDTI_DiceDotCountingDemo/ADI_CannyEdgeDetection/CannyEdgeDetection/common/SoftConfig_BF609.c \
C:/Users/gregori/Desktop/Projects/ADI_Shire_Port/NewBoard/BDTI_DiceDotCountingDemo/ADI_CannyEdgeDetection/CannyEdgeDetection/common/SoftConfig_Encoder.c \
C:/Users/gregori/Desktop/Projects/ADI_Shire_Port/NewBoard/BDTI_DiceDotCountingDemo/ADI_CannyEdgeDetection/CannyEdgeDetection/common/SystemInit.c \
C:/Users/gregori/Desktop/Projects/ADI_Shire_Port/NewBoard/BDTI_DiceDotCountingDemo/ADI_CannyEdgeDetection/CannyEdgeDetection/common/vat_ed_pvp.c 

OBJS += \
./common/BF609_GPIO.doj \
./common/PVPInit.doj \
./common/SoftConfig_BF609.doj \
./common/SoftConfig_Encoder.doj \
./common/SystemInit.doj \
./common/vat_ed_pvp.doj 

C_DEPS += \
./common/BF609_GPIO.d \
./common/PVPInit.d \
./common/SoftConfig_BF609.d \
./common/SoftConfig_Encoder.d \
./common/SystemInit.d \
./common/vat_ed_pvp.d 


# Each subdirectory must supply rules for building sources it contributes
common/BF609_GPIO.doj: C:/Users/gregori/Desktop/Projects/ADI_Shire_Port/NewBoard/BDTI_DiceDotCountingDemo/ADI_CannyEdgeDetection/CannyEdgeDetection/common/BF609_GPIO.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn.exe -c -file-attr ProjectName="VisionApp_Core0" -proc ADSP-BF609 -flags-compiler --no_wrap_diagnostics -si-revision 0.0 -O -Ov100 -DCORE0 -DNDEBUG -DADI_MCAPI -I"C:\Users\gregori\Desktop\Projects\ADI_Shire_Port\NewBoard\BDTI_DiceDotCountingDemo\ADI_CannyEdgeDetection\CannyEdgeDetection\VisionApp_Core0\system" -I"C:/Analog Devices/Video_Encoder_EI3_Extender_Board-Rel1.0.1/Video_Encoder_EI3/Blackfin/include" -I"C:/Analog Devices/Camera_EI3_Extender_Board-Rel1.0.1/Camera_EI3/Blackfin/include" -I"C:/Analog Devices/SoftwareModules/VisionAnalyticsToolbox-BF-Rel2.3.0/include" -structs-do-not-overlap -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -I"../../VisionApp_Core0" -I"../../common" -gnu-style-dependencies -MD -Mo "$(basename $@).d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

common/PVPInit.doj: C:/Users/gregori/Desktop/Projects/ADI_Shire_Port/NewBoard/BDTI_DiceDotCountingDemo/ADI_CannyEdgeDetection/CannyEdgeDetection/common/PVPInit.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn.exe -c -file-attr ProjectName="VisionApp_Core0" -proc ADSP-BF609 -flags-compiler --no_wrap_diagnostics -si-revision 0.0 -O -Ov100 -DCORE0 -DNDEBUG -DADI_MCAPI -I"C:\Users\gregori\Desktop\Projects\ADI_Shire_Port\NewBoard\BDTI_DiceDotCountingDemo\ADI_CannyEdgeDetection\CannyEdgeDetection\VisionApp_Core0\system" -I"C:/Analog Devices/Video_Encoder_EI3_Extender_Board-Rel1.0.1/Video_Encoder_EI3/Blackfin/include" -I"C:/Analog Devices/Camera_EI3_Extender_Board-Rel1.0.1/Camera_EI3/Blackfin/include" -I"C:/Analog Devices/SoftwareModules/VisionAnalyticsToolbox-BF-Rel2.3.0/include" -structs-do-not-overlap -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -I"../../VisionApp_Core0" -I"../../common" -gnu-style-dependencies -MD -Mo "$(basename $@).d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

common/SoftConfig_BF609.doj: C:/Users/gregori/Desktop/Projects/ADI_Shire_Port/NewBoard/BDTI_DiceDotCountingDemo/ADI_CannyEdgeDetection/CannyEdgeDetection/common/SoftConfig_BF609.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn.exe -c -file-attr ProjectName="VisionApp_Core0" -proc ADSP-BF609 -flags-compiler --no_wrap_diagnostics -si-revision 0.0 -O -Ov100 -DCORE0 -DNDEBUG -DADI_MCAPI -I"C:\Users\gregori\Desktop\Projects\ADI_Shire_Port\NewBoard\BDTI_DiceDotCountingDemo\ADI_CannyEdgeDetection\CannyEdgeDetection\VisionApp_Core0\system" -I"C:/Analog Devices/Video_Encoder_EI3_Extender_Board-Rel1.0.1/Video_Encoder_EI3/Blackfin/include" -I"C:/Analog Devices/Camera_EI3_Extender_Board-Rel1.0.1/Camera_EI3/Blackfin/include" -I"C:/Analog Devices/SoftwareModules/VisionAnalyticsToolbox-BF-Rel2.3.0/include" -structs-do-not-overlap -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -I"../../VisionApp_Core0" -I"../../common" -gnu-style-dependencies -MD -Mo "$(basename $@).d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

common/SoftConfig_Encoder.doj: C:/Users/gregori/Desktop/Projects/ADI_Shire_Port/NewBoard/BDTI_DiceDotCountingDemo/ADI_CannyEdgeDetection/CannyEdgeDetection/common/SoftConfig_Encoder.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn.exe -c -file-attr ProjectName="VisionApp_Core0" -proc ADSP-BF609 -flags-compiler --no_wrap_diagnostics -si-revision 0.0 -O -Ov100 -DCORE0 -DNDEBUG -DADI_MCAPI -I"C:\Users\gregori\Desktop\Projects\ADI_Shire_Port\NewBoard\BDTI_DiceDotCountingDemo\ADI_CannyEdgeDetection\CannyEdgeDetection\VisionApp_Core0\system" -I"C:/Analog Devices/Video_Encoder_EI3_Extender_Board-Rel1.0.1/Video_Encoder_EI3/Blackfin/include" -I"C:/Analog Devices/Camera_EI3_Extender_Board-Rel1.0.1/Camera_EI3/Blackfin/include" -I"C:/Analog Devices/SoftwareModules/VisionAnalyticsToolbox-BF-Rel2.3.0/include" -structs-do-not-overlap -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -I"../../VisionApp_Core0" -I"../../common" -gnu-style-dependencies -MD -Mo "$(basename $@).d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

common/SystemInit.doj: C:/Users/gregori/Desktop/Projects/ADI_Shire_Port/NewBoard/BDTI_DiceDotCountingDemo/ADI_CannyEdgeDetection/CannyEdgeDetection/common/SystemInit.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn.exe -c -file-attr ProjectName="VisionApp_Core0" -proc ADSP-BF609 -flags-compiler --no_wrap_diagnostics -si-revision 0.0 -O -Ov100 -DCORE0 -DNDEBUG -DADI_MCAPI -I"C:\Users\gregori\Desktop\Projects\ADI_Shire_Port\NewBoard\BDTI_DiceDotCountingDemo\ADI_CannyEdgeDetection\CannyEdgeDetection\VisionApp_Core0\system" -I"C:/Analog Devices/Video_Encoder_EI3_Extender_Board-Rel1.0.1/Video_Encoder_EI3/Blackfin/include" -I"C:/Analog Devices/Camera_EI3_Extender_Board-Rel1.0.1/Camera_EI3/Blackfin/include" -I"C:/Analog Devices/SoftwareModules/VisionAnalyticsToolbox-BF-Rel2.3.0/include" -structs-do-not-overlap -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -I"../../VisionApp_Core0" -I"../../common" -gnu-style-dependencies -MD -Mo "$(basename $@).d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

common/vat_ed_pvp.doj: C:/Users/gregori/Desktop/Projects/ADI_Shire_Port/NewBoard/BDTI_DiceDotCountingDemo/ADI_CannyEdgeDetection/CannyEdgeDetection/common/vat_ed_pvp.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn.exe -c -file-attr ProjectName="VisionApp_Core0" -proc ADSP-BF609 -flags-compiler --no_wrap_diagnostics -si-revision 0.0 -O -Ov100 -DCORE0 -DNDEBUG -DADI_MCAPI -I"C:\Users\gregori\Desktop\Projects\ADI_Shire_Port\NewBoard\BDTI_DiceDotCountingDemo\ADI_CannyEdgeDetection\CannyEdgeDetection\VisionApp_Core0\system" -I"C:/Analog Devices/Video_Encoder_EI3_Extender_Board-Rel1.0.1/Video_Encoder_EI3/Blackfin/include" -I"C:/Analog Devices/Camera_EI3_Extender_Board-Rel1.0.1/Camera_EI3/Blackfin/include" -I"C:/Analog Devices/SoftwareModules/VisionAnalyticsToolbox-BF-Rel2.3.0/include" -structs-do-not-overlap -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -I"../../VisionApp_Core0" -I"../../common" -gnu-style-dependencies -MD -Mo "$(basename $@).d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


