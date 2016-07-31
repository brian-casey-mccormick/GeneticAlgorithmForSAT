################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Population.cpp \
../src/SATProblemTypes.cpp \
../src/Support.cpp \
../src/driver.cpp 

OBJS += \
./src/Population.o \
./src/SATProblemTypes.o \
./src/Support.o \
./src/driver.o 

CPP_DEPS += \
./src/Population.d \
./src/SATProblemTypes.d \
./src/Support.d \
./src/driver.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


