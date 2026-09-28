#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

extern SemaphoreHandle_t xConsoleMutex;

void vCommTask( void *pvParameters )
{
    int tick = 0;
    ( void ) pvParameters;

    for( ;; )
    {
        /* Acquire mutex before writing to console */
        if( xSemaphoreTake( xConsoleMutex, pdMS_TO_TICKS( 100 ) ) == pdTRUE )
        {
            printf("[CommTask]    Heartbeat tick: %d\n", tick++);
            xSemaphoreGive( xConsoleMutex );
        }

        /* Report every 2 seconds */
        vTaskDelay( pdMS_TO_TICKS( 2000 ) );
    }
}
