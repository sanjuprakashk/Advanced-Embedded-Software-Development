#include "inc/timer_callback.h"

volatile uint8_t toggleFlag = 0;

void vTimerCallback(TimerHandle_t  *pxTimer)
{
    toggleFlag = 1;
}
