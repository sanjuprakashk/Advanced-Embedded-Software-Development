/**
 * @\file   timer_callback.h
 * @\author Sanju Prakash Kannioth
 * @\brief  This files contains the declarations and header files for the timer module
 * @\date   04/10/2019
 *
 */
#ifndef TIMER_CALLBACK_H
#define TIMER_CALLBACK_H

/*****************************************************************
                        Includes
*****************************************************************/
#include <stdio.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "timers.h"

/*
--------------------------------------------------------------------------------------------
vTimerCallback
--------------------------------------------------------------------------------------------
*   This functions is used as the callback function for the timer
*
*   @\param         pxTimer - timer handle
*
*   @\return        void
*
*/
void vTimerCallback(TimerHandle_t  *pxTimer );
#endif

