################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/gf/gf.c 

OBJS += \
./src/gf/gf.o 

C_DEPS += \
./src/gf/gf.d 


# Each subdirectory must supply rules for building sources it contributes
src/gf/%.o: ../src/gf/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -DDAGS_TOY -DRUN -I"/home/vader/dags_oficial/git/dags_n/include" -I/usr/local/include/keccak -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


