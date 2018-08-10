################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/amd64cpuinfo.c \
../src/decapsulation.c \
../src/decoding.c \
../src/enc.c \
../src/encapsulation.c \
../src/keygeneration.c \
../src/main.c \
../src/main_t.c 

OBJS += \
./src/amd64cpuinfo.o \
./src/decapsulation.o \
./src/decoding.o \
./src/enc.o \
./src/encapsulation.o \
./src/keygeneration.o \
./src/main.o \
./src/main_t.o 

C_DEPS += \
./src/amd64cpuinfo.d \
./src/decapsulation.d \
./src/decoding.d \
./src/enc.d \
./src/encapsulation.d \
./src/keygeneration.d \
./src/main.d \
./src/main_t.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -DDAGS_TOY -DRUN -I"/home/vader/dags_oficial/git/dags_n/include" -I/usr/local/include/keccak -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


