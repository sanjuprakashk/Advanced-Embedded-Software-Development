#ifndef ALERT_TASK_H
#define ALERT_TASK_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "inc/logger_task.h"

struct alert_struct
{
    char notification[50];
}alert_struct;


void alertTask(void *pvParameters);

#endif
