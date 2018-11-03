################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CommandInterpreter.c \
../Serialization.c \
../SocketClient.c \
../SocketCommons.c \
../SocketServer.c \
../StringUtils.c \
../ThreadManager.c \
../ThreadPool.c \
../Utils.c \
../config.c \
../logger.c \
../megekemmen.c 

OBJS += \
./CommandInterpreter.o \
./Serialization.o \
./SocketClient.o \
./SocketCommons.o \
./SocketServer.o \
./StringUtils.o \
./ThreadManager.o \
./ThreadPool.o \
./Utils.o \
./config.o \
./logger.o \
./megekemmen.o 

C_DEPS += \
./CommandInterpreter.d \
./Serialization.d \
./SocketClient.d \
./SocketCommons.d \
./SocketServer.d \
./StringUtils.d \
./ThreadManager.d \
./ThreadPool.d \
./Utils.d \
./config.d \
./logger.d \
./megekemmen.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


