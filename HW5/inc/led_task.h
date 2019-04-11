/**
 * @\file   led_task.h
 * @\author Sanju Prakash Kannioth
 * @\brief  This files contains the declarations and header files for the led module
 * @\date   04/10/2019
 *
 */
#ifndef LED_TASK_H
#define LED_TASK_H

/*****************************************************************
                        Includes
*****************************************************************/
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

/* Structure to store the led status and count */
struct led_struct
{
    char toggle_count[50];
    char name[50];
}led_struct;

void ledToggleTask(void *pvParameters);
/*
--------------------------------------------------------------------------------------------
ledToggleTask
--------------------------------------------------------------------------------------------
*   This functions is used to toggle LEDs
*
*   @\param         void
*
*   @\return        void
*
*/

#endif
