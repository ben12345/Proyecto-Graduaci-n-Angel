################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 1.0.1/Blackfin/lib/src/drivers/source/ppi/adi_ppi.c 

OBJS += \
./system/drivers/ppi/adi_ppi.doj 

C_DEPS += \
./system/drivers/ppi/adi_ppi.d 


# Each subdirectory must supply rules for building sources it contributes
system/drivers/ppi/adi_ppi.doj: C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 1.0.1/Blackfin/lib/src/drivers/source/ppi/adi_ppi.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn.exe -c -file-attr ProjectName="VideoLoopbackYUV" -proc ADSP-BF609 -flags-compiler --no_wrap_diagnostics -si-revision 0.0 -g -DCORE0 -DADI_DEBUG -D_DEBUG -I"C:\FinBoard_tutorial1_test4\VideoLoopbackYUV\system" -I"C:/Analog Devices/Video_Encoder_EI3_Extender_Board-Rel1.0.1/Video_Encoder_EI3/Blackfin/include" -I"C:/Analog Devices/Camera_EI3_Extender_Board-Rel1.0.1/Camera_EI3/Blackfin/include" -structs-do-not-overlap -no-multiline -warn-protos -double-size-32 -decls-strong -no-cplbs -gnu-style-dependencies -MD -Mo "$(basename $@).d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


