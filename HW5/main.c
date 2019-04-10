/*
 * Author: Sanju Prakash Kannioth
 *
 * references: https://energia.nu/pinmaps/ek-tm4c1294xl/,
 *             https://github.com/rheidebr/ECEN5013-LUX_Temp_demo,
 *             https://github.com/akobyl/TM4C129_FreeRTOS_Demo
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"
#include "drivers/pinout.h"
#include "utils/uartstdio.h"


// TivaWare includes
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/inc/hw_memmap.h"

// FreeRTOS includes
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

//User defined includes
#include "inc/i2c_task.h"
#include "inc/logger_task.h"
#include "inc/led_task.h"
#include "inc/alert_task.h"
#include "inc/timer_callback.h"


uint32_t output_clock_rate_hz;

SemaphoreHandle_t xSemaphore = NULL;

TaskHandle_t alert_task_handle = NULL;

// Main function
int main(void)
{
    // Initialize system clock to 120 MHz
    output_clock_rate_hz = SysCtlClockFreqSet(
                               (SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN |
                                SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480),
                               SYSTEM_CLOCK);
    ASSERT(output_clock_rate_hz == SYSTEM_CLOCK);

    UARTStdioConfig(0, 115200, SYSTEM_CLOCK);


    // Initialize the GPIO pins for the Launchpad
    PinoutSet(false, false);

    i2c_init();

    queueHandle = xQueueCreate(Q_LENGTH, sizeof(logger_struct));

    xSemaphore = xSemaphoreCreateMutex();

    xTaskCreate(ledToggleTask, (const portCHAR *)"LEDs",
                configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    xTaskCreate(alertTask, (const portCHAR *)"Alert",
                configMINIMAL_STACK_SIZE, NULL, 1, &alert_task_handle);

    xTaskCreate(tmp102Task, (const portCHAR *)"Temperature",
                    configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    xTaskCreate(loggerTask, (const portCHAR *)"Logger",
                        configMINIMAL_STACK_SIZE, NULL, 1, NULL);


    vTaskStartScheduler();
    return 0;
}

/*  ASSERT() Error function
 *
 *  failed ASSERTS() from driverlib/debug.h are executed in this function
 */
void __error__(char *pcFilename, uint32_t ui32Line)
{
    // Place a breakpoint here to capture errors until logging routine is finished
    while (1)
    {
    }
}
