#include "inc/led_task.h"

extern SemaphoreHandle_t xSemaphore;
extern volatile uint8_t toggleFlag;

struct logger_struct led_task_struct;

void ledToggleTask(void *pvParameters)
{
    led_task_struct.data.led = &led_struct;

    uint32_t xTimer_id = 1;
    xTimerHandle xLED_Timer;

    xLED_Timer = xTimerCreate("LED_Toggler", pdMS_TO_TICKS(100), pdTRUE, (void *) xTimer_id, vTimerCallback);

    xTimerStart( xLED_Timer, 0 );

    uint32_t LED1State, LED2State;

    uint32_t led_counter = 0;

    while(1)
    {
        if(toggleFlag)
        {
            led_counter++;
            LED1State = GPIOPinRead(CLP_D2_PORT, CLP_D2_PIN);
            LED2State = GPIOPinRead(CLP_D1_PORT, CLP_D1_PIN);

            GPIOPinWrite(CLP_D2_PORT, CLP_D2_PIN, !LED1State);

            if(!LED2State)
                GPIOPinWrite(CLP_D1_PORT, CLP_D1_PIN, CLP_D1_PIN);
            else
                GPIOPinWrite(CLP_D1_PORT, CLP_D1_PIN, 0);
            toggleFlag = 0;

            xSemaphoreTake(xSemaphore, 0);
            strcpy(led_task_struct.task_name,"LED");
            led_task_struct.ulTimeStamp = xTaskGetTickCount();
            sprintf(led_task_struct.data.led->toggle_count,"LED Count = %d", led_counter);
            sprintf(led_task_struct.data.led->name, "Name = Sanju");
            xQueueSendToBack(queueHandle,( void * ) &led_task_struct, 10) ;
            xSemaphoreGive(xSemaphore);
        }
     }
}
