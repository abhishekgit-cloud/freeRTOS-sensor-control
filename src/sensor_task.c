#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* Shared handles declared in main.c */
extern QueueHandle_t    xSensorQueue;
extern SemaphoreHandle_t xConsoleMutex;

/* Simulated sensor reading - increments to mimic changing sensor value */
static int simulate_sensor_reading( void )
{
    static int value = 0;
    value += 5;
    if( value > 100 ) value = 0;
    return value;
}

void vSensorTask( void *pvParameters )
{
    int sensorReading;
    ( void ) pvParameters;

    for( ;; )
    {
        /* Simulate acquiring a sensor reading */
        sensorReading = simulate_sensor_reading();

        /* Push reading into the queue */
        if( xQueueSend( xSensorQueue, &sensorReading, pdMS_TO_TICKS( 100 ) ) != pdPASS )
        {
            /* Queue full - protect console with mutex before printing */
            if( xSemaphoreTake( xConsoleMutex, pdMS_TO_TICKS( 50 ) ) == pdTRUE )
            {
                printf("[SensorTask] WARNING: Queue full, reading dropped\n");
                xSemaphoreGive( xConsoleMutex );
            }
        }

        /* Run every 500ms */
        vTaskDelay( pdMS_TO_TICKS( 500 ) );
    }
}
