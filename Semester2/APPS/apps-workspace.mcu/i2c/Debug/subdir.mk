################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../i2c-lib.cpp \
../main-i2c.cpp \
../si4735-lib.cpp 

CPP_DEPS += \
./i2c-lib.d \
./main-i2c.d \
./si4735-lib.d 

OBJS += \
./i2c-lib.o \
./main-i2c.o \
./si4735-lib.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -std=gnu++14 -DTARGET_NAME=K64F -DCOMPONENT_SD=1 -DDEVICE_RTC=1 -DFSL_RTOS_MBED -DTOOLCHAIN_GCC -DTARGET_LIKE_CORTEX_M4 -DTARGET_Freescale -DTARGET_MCUXpresso_MCUS -DTARGET_KPSDK_CODE -DTOOLCHAIN_GCC_ARM -DFEATURE_PSA=1 -DDEVICE_USTICKER=1 -DDEVICE_PORTIN=1 -DDEVICE_USBDEVICE=1 -DDEVICE_STDIO_MESSAGES=1 -DCOMPONENT_FLASHIAP=1 -DDEVICE_EMAC=1 -DDEVICE_TRNG=1 -DMBED_SPLIT_HEAP -DTARGET_KPSDK_MCUS -DTARGET_MCU_K64F -DDEVICE_PORTOUT=1 -D__MBED__=1 -DTARGET_Freescale_EMAC -DMBED_TICKLESS -DDEVICE_SERIAL=1 -DTARGET_PSA_Target -DDEVICE_PORTINOUT=1 -DDEVICE_LPTICKER=1 -DDEVICE_SERIAL_FC=1 -DTARGET_KSDK2_MCUS -DDEVICE_WATCHDOG=1 -DTARGET_PSA_V7_M -DDEVICE_ANALOGOUT=1 -DDEVICE_SPI_ASYNCH=1 -DDEVICE_ANALOGIN=1 -DDEVICE_I2C=1 -DTARGET_M4 -DDEVICE_SLEEP=1 -D__CORTEX_M4 -D__MBED_CMSIS_RTOS_CM -DDEVICE_FLASH=1 -DTARGET_K64F -DARM_MATH_CM4 -DMBED_BUILD_TIMESTAMP=1645455281.1211684 -DDEVICE_INTERRUPTIN=1 -DTARGET_RELEASE -DTARGET_LIKE_MBED -DTARGET_RTOS_M4_M7 -D__FPU_PRESENT=1 -DDEVICE_SERIAL_ASYNCH=1 -DTARGET_FRDM -DDEVICE_SPI=1 -D__CMSIS_RTOS -DTARGET_MBED_PSA_SRV -DDEVICE_RESET_REASON=1 -DDEVICE_SPISLAVE=1 -DTARGET_CORTEX -DTARGET_CORTEX_M -DDEVICE_PWMOUT=1 -DTARGET_FF_ARDUINO_UNO -DDEVICE_I2CSLAVE=1 -DDEVICE_CRC=1 -DCPU_MK64FN1M0VMD12 -D__NEWLIB__ -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/." -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/cmsis" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/cmsis/CMSIS_5" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/cmsis/CMSIS_5/CMSIS" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/cmsis/CMSIS_5/CMSIS/TARGET_CORTEX_M" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/cmsis/CMSIS_5/CMSIS/TARGET_CORTEX_M/Include" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/cmsis/device" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/cmsis/device/RTE" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/cmsis/device/RTE/include" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/drivers" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/drivers/include" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/drivers/include/drivers" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/drivers/include/drivers/interfaces" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/features" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/features/frameworks" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/hal" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/hal/include" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/hal/include/hal" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/hal/usb" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/hal/usb/include" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/hal/usb/include/usb" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/platform" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/platform/cxxsupport" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/platform/include" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/platform/include/platform" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/platform/include/platform/internal" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/platform/mbed-trace" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/platform/mbed-trace/include" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/platform/mbed-trace/include/mbed-trace" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/platform/source" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/platform/source/minimal-printf" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/rtos" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/rtos/include" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/rtos/include/rtos" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/rtos/include/rtos/internal" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/rtos/source" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/storage" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/storage/blockdevice" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/storage/blockdevice/include" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/storage/blockdevice/include/blockdevice" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/storage/blockdevice/include/blockdevice/internal" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/targets/TARGET_Freescale" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/TARGET_FRDM" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/device" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/drivers" -I"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed-os/targets/TARGET_Freescale/TARGET_MCUXpresso_MCUS/api" -include"/home/fei/min0150/Projects/VSB/APPS/apps-workspace.mcu/i2c/mbed_config.h" -fno-rtti -fmessage-length=0 -fno-exceptions -ffunction-sections -fdata-sections -funsigned-char -fomit-frame-pointer -g3 -Wall -Wextra -c -DMBED_DEBUG -DMBED_TRAP_ERRORS_ENABLED=1 -DMBED_MINIMAL_PRINTF -c -g3 -Wno-unused-parameter -Wno-missing-field-initializers -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean--2e-

clean--2e-:
	-$(RM) ./i2c-lib.d ./i2c-lib.o ./main-i2c.d ./main-i2c.o ./si4735-lib.d ./si4735-lib.o

.PHONY: clean--2e-

