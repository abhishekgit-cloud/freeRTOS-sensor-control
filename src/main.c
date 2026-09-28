#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

/* Queue handle - sensor task pushes readings, control task reads them */
QueueHandle_t xSensorQueue;

/* Mutex handle - protects shared UART/console output */
SemaphoreHandle_t xConsoleMutex;

/* Forward declarations */
void vSensorTask( void *pvParameters );
void vControlTask( void *pvParameters );
void vCommTask( void *pvParameters );

int main( void )
{
    printf("=== FreeRTOS Real-Time Sensor & Actuator Control System ===\n");
    printf("Starting scheduler...\n\n");

    /* Create queue: holds 10 sensor readings (int values) */
    xSensorQueue = xQueueCreate( 10, sizeof( int ) );
    if( xSensorQueue == NULL )
    {
        printf("ERROR: Failed to create sensor queue\n");
        return -1;
    }

    /* Create mutex for console output protection */
    xConsoleMutex = xSemaphoreCreateMutex();
    if( xConsoleMutex == NULL )
    {
        printf("ERROR: Failed to create console mutex\n");
        return -1;
    }

    /* Create tasks with different priorities */
    xTaskCreate( vSensorTask,   "SensorTask",   256, NULL, 3, NULL );
    xTaskCreate( vControlTask,  "ControlTask",  256, NULL, 2, NULL );
    xTaskCreate( vCommTask,     "CommTask",     256, NULL, 1, NULL );

    /* Start the FreeRTOS scheduler - this call never returns */
    vTaskStartScheduler();

    /* Should never reach here */
    printf("ERROR: Scheduler exited unexpectedly\n");
    return 0;
}
