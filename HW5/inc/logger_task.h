#ifndef LOGGER_TASK_H
#define LOGGER_TASK_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "utils/uartstdio.h"

#include "FreeRTOS.h"
#include "queue.h"

#include "inc/led_task.h"
#include "inc/i2c_task.h"
#include "inc/alert_task.h"


#define Q_LENGTH 200

QueueHandle_t queueHandle;

struct logger_struct
{
    char task_name[6];
    uint32_t ulTimeStamp;

    union data
    {
        struct temperature_struct *temp;
        struct led_struct *led;
        struct alert_struct *alert;
    }data;

}logger_struct;

void loggerTask(void *pvParameters);
#endif
