/* FreeRTOS 8.2 Tiva Demo
 *
 * main.c
 *
 * Andy Kobyljanec
 *
 * This is a simple demonstration project of FreeRTOS 8.2 on the Tiva Launchpad
 * EK-TM4C1294XL.  TivaWare driverlib sourcecode is included.
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
#include "driverlib/i2c.h"
#include "inc/hw_memmap.h"

// FreeRTOS includes
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

#define TEMP_ADDR 0x48
#define Q_LENGTH 200

uint32_t output_clock_rate_hz;
uint8_t toggleFlag = 0;

char log[100];

QueueHandle_t queueHandle;

QueueHandle_t temperatureQueueHandle;

SemaphoreHandle_t xSemaphore = NULL;

struct temperature_struct
{
    char temp[50];
}temperature_struct;

struct led_struct
{
    char toggle_count[50];
    char name[50];
}led_struct;

struct alert_struct
{
    char notification[50];
}alert_struct;

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

struct logger_struct led_task_struct;

struct logger_struct temperature_task_struct;

struct logger_struct alert_task_struct;


// Demo Task declarations
void demoLEDTask(void *pvParameters);
void demoSerialTask(void *pvParameters);
void tmp102Task(void *pvParameters);
void alertTask(void *pvParameters);
void ledToggleTask(void *pvParameters);
void loggerTask(void *pvParameters);

void vTimerCallback(TimerHandle_t  *pxTimer );

TaskHandle_t task1 = NULL;
TaskHandle_t task2 = NULL;

void i2c_init()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2); // Enable i2c2

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);


    GPIOPinConfigure(GPIO_PN4_I2C2SDA);

    GPIOPinConfigure(GPIO_PN5_I2C2SCL);


    GPIOPinTypeI2CSCL(GPIO_PORTN_BASE, GPIO_PIN_5);

    GPIOPinTypeI2C(GPIO_PORTN_BASE, GPIO_PIN_4);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C2));

    I2CMasterInitExpClk(I2C2_BASE, output_clock_rate_hz, false);

}
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

    led_task_struct.data.led = &led_struct;

    temperature_task_struct.data.temp = &temperature_struct;

    alert_task_struct.data.alert = &alert_struct;

    queueHandle = xQueueCreate(Q_LENGTH, sizeof(logger_struct));

    temperatureQueueHandle = xQueueCreate(Q_LENGTH, sizeof(temperature_struct));

    xSemaphore = xSemaphoreCreateMutex();
    // Create demo tasks

    xTaskCreate(loggerTask, (const portCHAR *)"Logger",
                    configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    xTaskCreate(ledToggleTask, (const portCHAR *)"LEDs",
                configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    xTaskCreate(alertTask, (const portCHAR *)"Alert",
                configMINIMAL_STACK_SIZE, NULL, 1, &task2);

    xTaskCreate(tmp102Task, (const portCHAR *)"Temperature",
                    configMINIMAL_STACK_SIZE, NULL, 1, NULL);

    vTaskStartScheduler();
    return 0;
}


void vTimerCallback(TimerHandle_t  *pxTimer )
{
    toggleFlag = 1;
}

void ledToggleTask(void *pvParameters)
{
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

            strcpy(led_task_struct.task_name,"LED");
            led_task_struct.ulTimeStamp = xTaskGetTickCount();
            sprintf(led_task_struct.data.led->toggle_count,"LED Count = %d", led_counter);
            sprintf(led_task_struct.data.led->name, "Name = Sanju");
            xSemaphoreTake(xSemaphore, 0);
            xQueueSendToBack(queueHandle,( void * ) &led_task_struct, 10) ;
            xSemaphoreGive(xSemaphore);
        }
     }
}

// Write text over the Stellaris debug interface UART port
void demoSerialTask(void *pvParameters)
{
    // Set up the UART which is connected to the virtual COM port
    UARTprintf("\r\nHello, world from FreeRTOS 9.0!");

    for (;;)
    {
//        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

uint8_t I2CGet2Bytes(uint8_t target_addr, uint8_t register_addr, uint16_t* data)
{
    uint8_t higherByte, lowerByte;
    I2CMasterSlaveAddrSet(I2C2_BASE, target_addr, false);

    I2CMasterDataPut(I2C2_BASE, register_addr);

    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_SINGLE_SEND);

    while(!I2CMasterBusy(I2C2_BASE));

    while(I2CMasterBusy(I2C2_BASE));

    I2CMasterSlaveAddrSet(I2C2_BASE, target_addr, true);

    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);

    while(!I2CMasterBusy(I2C2_BASE));

    while(I2CMasterBusy(I2C2_BASE));

    higherByte = I2CMasterDataGet(I2C2_BASE);

    I2CMasterControl(I2C2_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);

    while(!I2CMasterBusy(I2C2_BASE));

    while(I2CMasterBusy(I2C2_BASE));

    lowerByte = I2CMasterDataGet(I2C2_BASE);

    *data = ((higherByte << 8) | lowerByte) >> 4;

    return 0;
}


void tmp102Task(void *pvParameters)
{
    while(1)
    {
        uint16_t temp;
        I2CGet2Bytes(TEMP_ADDR, 0x00, &temp);


        float temperature;
        temperature = temp * 0.0625;

        uint32_t temp_uint = (uint32_t)(10000.0 * temperature);

//        UARTprintf("Temperature = %d.%d\n", (uint32_t)temp_uint/10000, (uint32_t)temp_uint%10000);

        if(temperature > 27)
        {
            xTaskNotify(task2, 0, eNoAction);
        }

        strcpy(temperature_task_struct.task_name,"TEMP");
        temperature_task_struct.ulTimeStamp = xTaskGetTickCount();
        sprintf(temperature_task_struct.data.temp->temp,"Temperature = %d.%d\n",(uint32_t)temp_uint/10000, (uint32_t)temp_uint%10000);
        xSemaphoreTake(xSemaphore, 0);
        xQueueSendToBack(queueHandle,( void * ) &temperature_task_struct, 10) ;
        xSemaphoreGive(xSemaphore);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

}

void alertTask(void *pvParameters)
{
    uint32_t ulRecv;

    while(1)
    {
        xTaskNotifyWait(0, 0, &ulRecv, portMAX_DELAY);
        alert_task_struct.ulTimeStamp = xTaskGetTickCount();
        strcpy(alert_task_struct.task_name,"ALERT");
        sprintf(alert_task_struct.data.alert->notification,"%s","Temperature exceeded set threshold");
        xSemaphoreTake(xSemaphore, 0);
        xQueueSendToBack(queueHandle,( void * ) &alert_task_struct, 10) ;
        xSemaphoreGive(xSemaphore);
    }
}


void loggerTask(void *pvParameters)
{
    struct logger_struct logger;

    while(1)
    {
        if(xQueueReceive(queueHandle, &logger, 10) == pdTRUE)
        {
            if(strcmp(logger.task_name, "LED") == 0)
                UARTprintf("[Timestamp:%d] %s %s\n", logger.ulTimeStamp, logger.data.led->toggle_count, logger.data.led->name);
            else if(strcmp(logger.task_name, "TEMP") == 0)
                UARTprintf("\n\n[Timestamp:%d] %s\n\n", logger.ulTimeStamp, logger.data.temp->temp);
            else if(strcmp(logger.task_name, "ALERT") == 0)
                UARTprintf("\n\n[Timestamp:%d] %s\n\n", logger.ulTimeStamp, logger.data.alert->notification);

        }
    }
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
