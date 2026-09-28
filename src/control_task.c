#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

extern QueueHandle_t     xSensorQueue;
extern SemaphoreHandle_t xConsoleMutex;

/* Threshold values for FSM transitions */
#define THRESHOLD_WARNING   60
#define THRESHOLD_FAULT     85

/* FSM states */
typedef enum {
    STATE_NORMAL  = 0,
    STATE_WARNING,
    STATE_FAULT,
    STATE_RECOVERY
} SystemState_t;

static SystemState_t currentState = STATE_NORMAL;

static const char* state_name( SystemState_t s )
{
    switch(s) {
        case STATE_NORMAL:   return "NORMAL";
        case STATE_WARNING:  return "WARNING";
        case STATE_FAULT:    return "FAULT";
        case STATE_RECOVERY: return "RECOVERY";
        default:             return "UNKNOWN";
    }
}

static void run_fsm( int reading )
{
    SystemState_t newState = currentState;

    switch( currentState )
    {
        case STATE_NORMAL:
            if( reading >= THRESHOLD_FAULT )
                newState = STATE_FAULT;
            else if( reading >= THRESHOLD_WARNING )
                newState = STATE_WARNING;
            break;

        case STATE_WARNING:
            if( reading >= THRESHOLD_FAULT )
                newState = STATE_FAULT;
            else if( reading < THRESHOLD_WARNING )
                newState = STATE_NORMAL;
            break;

        case STATE_FAULT:
            newState = STATE_RECOVERY;
            break;

        case STATE_RECOVERY:
            if( reading < THRESHOLD_WARNING )
                newState = STATE_NORMAL;
            break;
    }

    if( newState != currentState )
    {
        if( xSemaphoreTake( xConsoleMutex, pdMS_TO_TICKS( 50 ) ) == pdTRUE )
        {
            printf("[ControlTask] FSM: %s -> %s (reading=%d)\n",
                   state_name(currentState), state_name(newState), reading);
            xSemaphoreGive( xConsoleMutex );
        }
        currentState = newState;
    }
}

void vControlTask( void *pvParameters )
{
    int sensorReading;
    ( void ) pvParameters;

    for( ;; )
    {
        /* Block until a sensor reading arrives in the queue */
        if( xQueueReceive( xSensorQueue, &sensorReading,
                           pdMS_TO_TICKS( 1000 ) ) == pdPASS )
        {
            /* Run the FSM with the new reading */
            run_fsm( sensorReading );
        }
    }
}
