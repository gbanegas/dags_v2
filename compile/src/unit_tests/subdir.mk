################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/unit_tests/check_decapsulation.c \
../src/unit_tests/check_decoding.c \
../src/unit_tests/check_encapsulation.c \
../src/unit_tests/check_gf.c \
../src/unit_tests/check_key_gen.c \
../src/unit_tests/check_matrix.c \
../src/unit_tests/main_test.c 

OBJS += \
./src/unit_tests/check_decapsulation.o \
./src/unit_tests/check_decoding.o \
./src/unit_tests/check_encapsulation.o \
./src/unit_tests/check_gf.o \
./src/unit_tests/check_key_gen.o \
./src/unit_tests/check_matrix.o \
./src/unit_tests/main_test.o 

C_DEPS += \
./src/unit_tests/check_decapsulation.d \
./src/unit_tests/check_decoding.d \
./src/unit_tests/check_encapsulation.d \
./src/unit_tests/check_gf.d \
./src/unit_tests/check_key_gen.d \
./src/unit_tests/check_matrix.d \
./src/unit_tests/main_test.d 


# Each subdirectory must supply rules for building sources it contributes
src/unit_tests/%.o: ../src/unit_tests/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -DDAGS_TOY -DRUN -I"/home/vader/dags_oficial/git/dags_n/include" -I/usr/local/include/keccak -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


