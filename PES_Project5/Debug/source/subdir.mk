################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/Logger.c \
../source/System.c \
../source/Testsuite.c \
../source/Time_Systick.c \
../source/UARTFunction_Interrupt.c \
../source/UARTFunction_polled.c \
../source/circular_buffer.c \
../source/led_control.c \
../source/main.c \
../source/mtb.c 

OBJS += \
./source/Logger.o \
./source/System.o \
./source/Testsuite.o \
./source/Time_Systick.o \
./source/UARTFunction_Interrupt.o \
./source/UARTFunction_polled.o \
./source/circular_buffer.o \
./source/led_control.o \
./source/main.o \
./source/mtb.o 

C_DEPS += \
./source/Logger.d \
./source/System.d \
./source/Testsuite.d \
./source/Time_Systick.d \
./source/UARTFunction_Interrupt.d \
./source/UARTFunction_polled.d \
./source/circular_buffer.d \
./source/led_control.d \
./source/main.d \
./source/mtb.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DSDK_OS_BAREMETAL -DFSL_RTOS_BM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I../board -I../source -I../ -I../drivers -I../CMSIS -I../utilities -I../startup -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


