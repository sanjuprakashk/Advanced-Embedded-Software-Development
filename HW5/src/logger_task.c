#include "inc/logger_task.h"


void loggerTask(void *pvParameters)
{
    struct logger_struct logger;

    while(1)
    {
        if(xQueueReceive(queueHandle, &logger, 10) == pdTRUE)
        {
            if(strcmp(logger.task_name, "LED") == 0)
                UARTprintf("\n[Timestamp:%d] %s %s\n", logger.ulTimeStamp, logger.data.led->toggle_count, logger.data.led->name);
            else if(strcmp(logger.task_name, "TEMP") == 0)
                UARTprintf("\n[Timestamp:%d] %s\n", logger.ulTimeStamp, logger.data.temp->temp);
            else if(strcmp(logger.task_name, "ALERT") == 0)
                UARTprintf("\n[Timestamp:%d] %s\n", logger.ulTimeStamp, logger.data.alert->notification);

        }
    }
}
