# FreeRTOS Real-Time Sensor & Actuator Control System

A real-time sensor and actuator control system built using FreeRTOS
(POSIX simulation port), demonstrating priority-based task scheduling,
inter-task communication, and fault-tolerant state machine design.

## Architecture

Three concurrent tasks under FreeRTOS preemptive scheduling:

- **SensorTask (Priority 3):** Simulates periodic sensor acquisition,
  pushes readings into a FreeRTOS queue every 500ms.
- **ControlTask (Priority 2):** Blocks on queue receive, runs a
  threshold-based FSM, drives actuator logic.
- **CommTask (Priority 1):** Heartbeat logging behind a mutex-protected
  console output every 2 seconds.

## FSM States

| State    | Condition                        |
|----------|----------------------------------|
| NORMAL   | reading < 60                     |
| WARNING  | 60 <= reading < 85               |
| FAULT    | reading >= 85                    |
| RECOVERY | Auto-transition from FAULT       |

## Key FreeRTOS Concepts Demonstrated

- Priority-based preemptive scheduling
- Queue for producer-consumer data flow between tasks
- Mutex for shared resource protection (console output)
- Finite State Machine with four operating modes
- Software timer and watchdog pattern for fault recovery
- Interrupt-to-task synchronization pattern

## Build and Run

### Requirements
- Ubuntu Linux (or WSL2)
- GCC, make, git

### Steps
```bash
git clone https://github.com/abhishekgit-cloud/freeRTOS-sensor-control.git
cd freeRTOS-sensor-control
git submodule update --init --recursive
make
./rtos_sensor_control
```

## Design Decisions

**Why FreeRTOS queues instead of global variables?**
Queues are thread-safe by design. Global variables shared between tasks
require explicit synchronization. Queues decouple producer and consumer,
which is the correct embedded systems pattern.

**Why mutex for console output?**
printf is not reentrant. Without the mutex, two tasks printing
simultaneously would produce garbled output. In real firmware this
protects a shared UART peripheral.

**Why a FSM for fault handling?**
A FSM makes state transitions explicit, auditable, and testable.
It prevents illegal transitions (e.g. jumping from NORMAL to RECOVERY)
and is standard practice in safety-conscious embedded systems.
