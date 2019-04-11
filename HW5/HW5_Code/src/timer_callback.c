/**
 * @\file   timer_callback.c
 * @\author Sanju Prakash Kannioth
 * @\brief  This files contains the function definitions for the timer module
 * @\date   04/10/2019
 *
 */
#include "inc/timer_callback.h"

volatile uint8_t toggleFlag = 0;

void vTimerCallback(TimerHandle_t  *pxTimer)
{
    toggleFlag = 1;
}
