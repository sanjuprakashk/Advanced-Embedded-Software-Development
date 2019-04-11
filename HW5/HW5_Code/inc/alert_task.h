/**
 * @\file   alert_task.h
 * @\author Sanju Prakash Kannioth
 * @\brief  This files contains the declarations and header files for the alert module
 * @\date   04/10/2019
 *
 */
#ifndef ALERT_TASK_H
#define ALERT_TASK_H

/*****************************************************************
                        Includes
*****************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "inc/logger_task.h"

/* Structure to store the alert notification */
struct alert_struct
{
    char notification[50];
}alert_struct;

/*
--------------------------------------------------------------------------------------------
alertTask
--------------------------------------------------------------------------------------------
*   This functions is used to issue alerts when temperature crosses threshold values
*
*   @\param         void
*
*   @\return        void
*
*/
void alertTask(void *pvParameters);

#endif
