#include "inc/alert_task.h"

extern SemaphoreHandle_t xSemaphore;

struct logger_struct alert_task_struct;

void alertTask(void *pvParameters)
{
    alert_task_struct.data.alert = &alert_struct;

    uint32_t ulRecv;

    while(1)
    {
        xTaskNotifyWait(0, 0, &ulRecv, portMAX_DELAY);

        xSemaphoreTake(xSemaphore, 0);
        alert_task_struct.ulTimeStamp = xTaskGetTickCount();
        strcpy(alert_task_struct.task_name,"ALERT");
        sprintf(alert_task_struct.data.alert->notification,"%s","Temperature exceeded set threshold");
        xQueueSendToBack(queueHandle,( void * ) &alert_task_struct, 10) ;
        xSemaphoreGive(xSemaphore);
    }
}
