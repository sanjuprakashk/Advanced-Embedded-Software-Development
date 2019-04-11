/**
 * @\file   logger_task.h
 * @\author Sanju Prakash Kannioth
 * @\brief  This files contains the declarations and header files for the logger module
 * @\date   04/10/2019
 *
 */
#ifndef LOGGER_TASK_H
#define LOGGER_TASK_H

/*****************************************************************
                        Includes
*****************************************************************/
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


#define Q_LENGTH 200 // Maximum length of queue

QueueHandle_t queueHandle; // Queue handle

/* Structure to store logging information */
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


/*
--------------------------------------------------------------------------------------------
loggerTask
--------------------------------------------------------------------------------------------
*   This functions is used to log various status messages using freeRTOS queues
*
*   @\param         void
*
*   @\return        void
*
*/
void loggerTask(void *pvParameters);
#endif
