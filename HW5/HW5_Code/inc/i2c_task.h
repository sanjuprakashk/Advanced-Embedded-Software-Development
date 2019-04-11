/**
 * @\file   i2c_task.h
 * @\author Sanju Prakash Kannioth
 * @\brief  This files contains the declarations and header files for the i2c temperature sensor module
 * @\date   04/10/2019
 *
 */

#ifndef I2C_TASK_H
#define I2C_TASK_H

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

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "inc/logger_task.h"

#define TEMP_ADDR 0x48 //I2C Address of temperature sensor

/* Structure to store the temperature value */
struct temperature_struct
{
    char temp[35];
}temperature_struct;



/*
--------------------------------------------------------------------------------------------
i2c_init
--------------------------------------------------------------------------------------------
*   This functions is used to initialize the i2c
*
*   @\param         void
*
*   @\return        void
*
*/
void i2c_init();

/*
--------------------------------------------------------------------------------------------
tmp102Task
--------------------------------------------------------------------------------------------
*   This functions is used to read the temperature from the tmp102 sensor
*
*   @\param         void
*
*   @\return        void
*
*/
void tmp102Task(void *pvParameters);


/*
--------------------------------------------------------------------------------------------
I2CGet2Bytes
--------------------------------------------------------------------------------------------
*   This functions is used to read 2 bytes from the i2c
*
*   @\param         target_addr, register_addr, data
*
*   @\return        0 for SUCCESS
*                   -1 for ERROR
*
*/
uint8_t I2CGet2Bytes(uint8_t target_addr, uint8_t register_addr, int16_t* data);

#endif
