#ifndef I2C_TASK_H
#define I2C_TASK_H

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

#define TEMP_ADDR 0x48

struct temperature_struct
{
    char temp[50];
}temperature_struct;

void i2c_init();

void tmp102Task(void *pvParameters);

uint8_t I2CGet2Bytes(uint8_t target_addr, uint8_t register_addr, uint16_t* data);

#endif
