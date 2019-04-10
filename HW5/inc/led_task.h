#ifndef LED_TASK_H
#define LED_TASK_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "driverlib/i2c.h"
#include "driverlib/inc/hw_memmap.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "drivers/pinout.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"


#include "inc/logger_task.h"
#include "inc/timer_callback.h"


struct led_struct
{
    char toggle_count[50];
    char name[50];
}led_struct;


void ledToggleTask(void *pvParameters);

#endif
