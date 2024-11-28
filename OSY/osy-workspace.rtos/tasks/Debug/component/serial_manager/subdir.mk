################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/serial_manager/fsl_component_serial_manager.c \
../component/serial_manager/fsl_component_serial_port_uart.c 

C_DEPS += \
./component/serial_manager/fsl_component_serial_manager.d \
./component/serial_manager/fsl_component_serial_port_uart.d 

OBJS += \
./component/serial_manager/fsl_component_serial_manager.o \
./component/serial_manager/fsl_component_serial_port_uart.o 


# Each subdirectory must supply rules for building sources it contributes
component/serial_manager/%.o: ../component/serial_manager/%.c component/serial_manager/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__NEWLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DSDK_OS_BAREMETAL -DSERIAL_PORT_TYPE_UART=1 -DSDK_DEBUGCONSOLE=1 -DSDK_OS_FREE_RTOS -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/board" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/source" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/freertos/freertos_kernel/include" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/freertos/freertos_kernel/portable/GCC/ARM_CM4F" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/drivers" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/device" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/CMSIS" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/utilities" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/component/serial_manager" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/component/uart" -I"/home/fei/min0150/Projects/VSB/OSY/osy-workspace.rtos/tasks/component/lists" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-component-2f-serial_manager

clean-component-2f-serial_manager:
	-$(RM) ./component/serial_manager/fsl_component_serial_manager.d ./component/serial_manager/fsl_component_serial_manager.o ./component/serial_manager/fsl_component_serial_port_uart.d ./component/serial_manager/fsl_component_serial_port_uart.o

.PHONY: clean-component-2f-serial_manager

