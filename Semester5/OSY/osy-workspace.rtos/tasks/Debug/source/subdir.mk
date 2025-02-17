################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../source/main-tasks.cpp 

C_SRCS += \
../source/semihost_hardfault.c 

CPP_DEPS += \
./source/main-tasks.d 

C_DEPS += \
./source/semihost_hardfault.d 

OBJS += \
./source/main-tasks.o \
./source/semihost_hardfault.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.cpp source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DSDK_OS_BAREMETAL -DSERIAL_PORT_TYPE_UART=1 -DSDK_DEBUGCONSOLE=1 -DSDK_OS_FREE_RTOS -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/board" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/source" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/freertos/freertos_kernel/include" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/freertos/freertos_kernel/portable/GCC/ARM_CM4F" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/drivers" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/device" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/CMSIS" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/utilities" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/component/serial_manager" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/component/uart" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/component/lists" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fno-rtti -fno-exceptions -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__NEWLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DSDK_OS_BAREMETAL -DSERIAL_PORT_TYPE_UART=1 -DSDK_DEBUGCONSOLE=1 -DSDK_OS_FREE_RTOS -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/board" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/source" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/freertos/freertos_kernel/include" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/freertos/freertos_kernel/portable/GCC/ARM_CM4F" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/drivers" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/device" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/CMSIS" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/utilities" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/component/serial_manager" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/component/uart" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/component/lists" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/main-tasks.d ./source/main-tasks.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o

.PHONY: clean-source

