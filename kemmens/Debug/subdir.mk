################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SocketClient.c \
../SocketCommons.c \
../SocketServer.c \
../ThreadManager.c \
../config.c \
../logger.c \
../megekemmen.c 

OBJS += \
./SocketClient.o \
./SocketCommons.o \
./SocketServer.o \
./ThreadManager.o \
./config.o \
./logger.o \
./megekemmen.o 

C_DEPS += \
./SocketClient.d \
./SocketCommons.d \
./SocketServer.d \
./ThreadManager.d \
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


