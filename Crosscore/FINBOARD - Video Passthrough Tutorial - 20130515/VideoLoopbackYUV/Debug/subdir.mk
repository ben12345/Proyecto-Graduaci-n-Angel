################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sensor.c \
../SoftConfig_BF609.c \
../SoftConfig_Encoder.c \
../VideoLoopbackYUV.c \
../encoder.c \
../finboard_bsp.c 

OBJS += \
./Sensor.doj \
./SoftConfig_BF609.doj \
./SoftConfig_Encoder.doj \
./VideoLoopbackYUV.doj \
./encoder.doj \
./finboard_bsp.doj 

C_DEPS += \
./Sensor.d \
./SoftConfig_BF609.d \
./SoftConfig_Encoder.d \
./VideoLoopbackYUV.d \
./encoder.d \
./finboard_bsp.d 


# Each subdirectory must supply rules for building sources it contributes
%.doj: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn.exe -c -file-attr ProjectName="VideoLoopbackYUV" -proc ADSP-BF609 -flags-compiler --no_wrap_diagnostics -si-revision 0.0 -g -DCORE0 -DADI_DEBUG -D_DEBUG -I"C:\FinBoard_tutorial1_test4\VideoLoopbackYUV\system" -I"C:/Analog Devices/Video_Encoder_EI3_Extender_Board-Rel1.0.1/Video_Encoder_EI3/Blackfin/include" -I"C:/Analog Devices/Camera_EI3_Extender_Board-Rel1.0.1/Camera_EI3/Blackfin/include" -structs-do-not-overlap -no-multiline -warn-protos -double-size-32 -decls-strong -no-cplbs -gnu-style-dependencies -MD -Mo "$(basename $@).d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

