################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Graphics.c \
../Sensor.c \
../Stage_1.c \
../adi_contour_mem_alloc.c \
../adi_contours_wrapper.c \
../adi_dma_vars.c \
../adi_find_contours.c \
../adi_mem_move_bf609.c \
../adi_memory.c \
../contour_dynamic_mem_alloc.c \
../contour_pseudo_dynamic_mem_alloc.c \
../encoder.c \
C:/Users/Usuario/Music/PG/Demo_1/BDTI_DiceDotCountingDemo/CannyEdgeDetection/common/finboard_bsp.c 

ASM_SRCS += \
../adi_dma_func.asm \
../arial_12pt.asm \
../color_conversion.asm 

OBJS += \
./Graphics.doj \
./Sensor.doj \
./Stage_1.doj \
./adi_contour_mem_alloc.doj \
./adi_contours_wrapper.doj \
./adi_dma_func.doj \
./adi_dma_vars.doj \
./adi_find_contours.doj \
./adi_mem_move_bf609.doj \
./adi_memory.doj \
./arial_12pt.doj \
./color_conversion.doj \
./contour_dynamic_mem_alloc.doj \
./contour_pseudo_dynamic_mem_alloc.doj \
./encoder.doj \
./finboard_bsp.doj 

C_DEPS += \
./Graphics.d \
./Sensor.d \
./Stage_1.d \
./adi_contour_mem_alloc.d \
./adi_contours_wrapper.d \
./adi_dma_vars.d \
./adi_find_contours.d \
./adi_mem_move_bf609.d \
./adi_memory.d \
./contour_dynamic_mem_alloc.d \
./contour_pseudo_dynamic_mem_alloc.d \
./encoder.d \
./finboard_bsp.d 

ASM_DEPS += \
./adi_dma_func.d \
./arial_12pt.d \
./color_conversion.d 


# Each subdirectory must supply rules for building sources it contributes
%.doj: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn.exe -c -file-attr ProjectName="VisionApp_Core0" -proc ADSP-BF609 -flags-compiler --no_wrap_diagnostics -si-revision 0.0 -g -DCORE0 -D_DEBUG -DADI_MCAPI -I"C:\Users\Usuario\Music\PG\Demo_1\BDTI_DiceDotCountingDemo\CannyEdgeDetection\VisionApp_Core0\system" -I"C:/Analog Devices/Video_Encoder_EI3_Extender_Board-Rel1.0.1/Video_Encoder_EI3/Blackfin/include" -I"C:/Analog Devices/SoftwareModules/VisionAnalyticsToolbox-BF-Rel2.4.0/include" -I"C:/Analog Devices/SoftwareModules/Graphics2D_OGL-BF-Rel3.4.0/Include" -I"C:/Analog Devices/SoftwareModules/ImageProcessingToolbox-BF-Rel2.3.0/Include" -I"C:/Analog Devices/Camera_EI3_Extender_Board-Rel1.0.1/Camera_EI3/Blackfin/include" -structs-do-not-overlap -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -I"../../VisionApp_Core0" -I"../../common" -gnu-style-dependencies -MD -Mo "$(basename $@).d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.doj: ../%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin Assembler'
	easmblkfn.exe -file-attr ProjectName="VisionApp_Core0" -proc ADSP-BF609 -si-revision 0.0 -g -DCORE0 -D_DEBUG -DADI_MCAPI -i"C:\Users\Usuario\Music\PG\Demo_1\BDTI_DiceDotCountingDemo\CannyEdgeDetection\VisionApp_Core0\system" -i"C:/Analog Devices/Video_Encoder_EI3_Extender_Board-Rel1.0.1/Video_Encoder_EI3/Blackfin/include" -i"C:/Analog Devices/SoftwareModules/ImageProcessingToolbox-BF-Rel2.3.0/Include" -gnu-style-dependencies -MM -Mo "$(basename $@).d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

finboard_bsp.doj: C:/Users/Usuario/Music/PG/Demo_1/BDTI_DiceDotCountingDemo/CannyEdgeDetection/common/finboard_bsp.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn.exe -c -file-attr ProjectName="VisionApp_Core0" -proc ADSP-BF609 -flags-compiler --no_wrap_diagnostics -si-revision 0.0 -g -DCORE0 -D_DEBUG -DADI_MCAPI -I"C:\Users\Usuario\Music\PG\Demo_1\BDTI_DiceDotCountingDemo\CannyEdgeDetection\VisionApp_Core0\system" -I"C:/Analog Devices/Video_Encoder_EI3_Extender_Board-Rel1.0.1/Video_Encoder_EI3/Blackfin/include" -I"C:/Analog Devices/SoftwareModules/VisionAnalyticsToolbox-BF-Rel2.4.0/include" -I"C:/Analog Devices/SoftwareModules/Graphics2D_OGL-BF-Rel3.4.0/Include" -I"C:/Analog Devices/SoftwareModules/ImageProcessingToolbox-BF-Rel2.3.0/Include" -I"C:/Analog Devices/Camera_EI3_Extender_Board-Rel1.0.1/Camera_EI3/Blackfin/include" -structs-do-not-overlap -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -I"../../VisionApp_Core0" -I"../../common" -gnu-style-dependencies -MD -Mo "$(basename $@).d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


