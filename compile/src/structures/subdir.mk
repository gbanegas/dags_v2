################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/structures/matrix_operations.c \
../src/structures/mxm.c \
../src/structures/polynomial.c 

OBJS += \
./src/structures/matrix_operations.o \
./src/structures/mxm.o \
./src/structures/polynomial.o 

C_DEPS += \
./src/structures/matrix_operations.d \
./src/structures/mxm.d \
./src/structures/polynomial.d 


# Each subdirectory must supply rules for building sources it contributes
src/structures/%.o: ../src/structures/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -DDAGS_TOY -DRUN -I"/home/vader/dags_oficial/git/dags_n/include" -I/usr/local/include/keccak -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


