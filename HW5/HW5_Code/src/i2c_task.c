/**
 * @\file   i2c_task.c
 * @\author Sanju Prakash Kannioth
 * @\brief  This files contains the function definitions for i2c temperature sensor module
 * @\date   04/10/2019
 *
 */
#include "inc/i2c_task.h"

extern TaskHandle_t alert_task_handle;
extern SemaphoreHandle_t xSemaphore;
extern uint32_t output_clock_rate_hz;

struct logger_struct temperature_task_struct;


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

uint8_t I2CGet2Bytes(uint8_t target_addr, uint8_t register_addr, int16_t* data)
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
    temperature_task_struct.data.temp = &temperature_struct;
    while(1)
    {
        int16_t temp;
        I2CGet2Bytes(TEMP_ADDR, 0x00, &temp);

        if(temp & 0x800)
        {
            temp = (~(temp) + 1) & 0xFFF;
            temp = -1 * temp;
        }
        float temperature;
        temperature = temp * 0.0625;

        int32_t temp_int = (int32_t)(10000.0 * temperature);

        if(temperature > 27)
        {
            xTaskNotify(alert_task_handle, 0, eNoAction);
        }

        if(temperature < 0)
        {
            xTaskNotify(alert_task_handle, 0, eNoAction);
        }

        xSemaphoreTake(xSemaphore, 0);
        strcpy(temperature_task_struct.task_name,"TEMP");
        temperature_task_struct.ulTimeStamp = xTaskGetTickCount();
        sprintf(temperature_task_struct.data.temp->temp,"Temperature = %d.%d\n",(int32_t)temp_int/10000, (int32_t)temp_int%10000);
        xQueueSendToBack(queueHandle,( void * ) &temperature_task_struct, 10) ;
        xSemaphoreGive(xSemaphore);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

}
