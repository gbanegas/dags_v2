################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf
CC=clang
#Select the desired dags you would like to build
#DAGSVER=-DDAGS_1
DAGSVER=-DDAGS_3
#DAGSVER=-DDAGS_5

CFLAGS=-O3 -g3 -Wall -DRUN -march=native
LIBS=-lcrypto -lm -lsodium -lkeccak
# All of the sources participating in the build are defined here
-include sources.mk
-include src/util/subdir.mk
-include src/structures/subdir.mk
-include src/random/subdir.mk
-include src/gf/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs
SRC=$(wildcard src/*.c src/gf/*.c src/random/*.c src/structures/*.c src/util/*.c)
# Add inputs and outputs from these tool invocations to the build variables 
# All Target
all: dags

# Tool invocations
dags: $(SRC)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	$(CC) $(CFLAGS) $(DAGSVER) $  -o "dags" $(SRC)  $(LIBS) 
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) dags *.o
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets
