# Project name
TARGET = rtos_sensor_control

# Compiler
CC = gcc

# FreeRTOS kernel path
FREERTOS = FreeRTOS-Kernel

# POSIX port path
POSIX_PORT = $(FREERTOS)/portable/ThirdParty/GCC/Posix

# Memory manager (heap_4 is standard for embedded)
HEAP = $(FREERTOS)/portable/MemMang/heap_4.c

# Compiler flags
CFLAGS = -Wall -Wextra -g \
         -I$(FREERTOS)/include \
         -I$(POSIX_PORT) \
         -I$(POSIX_PORT)/utils \
         -Iinclude \
         -Isrc \
         -lpthread \
         -lrt

# FreeRTOS kernel source files
FREERTOS_SRC = \
    $(FREERTOS)/tasks.c \
    $(FREERTOS)/queue.c \
    $(FREERTOS)/list.c \
    $(FREERTOS)/timers.c \
    $(FREERTOS)/event_groups.c \
    $(FREERTOS)/stream_buffer.c \
    $(POSIX_PORT)/port.c \
    $(POSIX_PORT)/utils/wait_for_event.c \
    $(HEAP)

# Your application source files
APP_SRC = \
    src/main.c \
    src/sensor_task.c \
    src/control_task.c \
    src/comm_task.c

# All source files combined
ALL_SRC = $(FREERTOS_SRC) $(APP_SRC)

# Build target
all:
	$(CC) $(ALL_SRC) $(CFLAGS) -o $(TARGET)
	@echo "Build successful: ./$(TARGET)"

# Clean build artifacts
clean:
	rm -f $(TARGET)

.PHONY: all clean
