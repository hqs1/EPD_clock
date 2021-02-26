#ifndef _BATTERY_H_
#define _BATTERY_H_

#include "main.h"

#define ADCtBatteryValue 1.9679f
#define BATTERY_LOW 3.65f
#define BATTERY_FULL 4.2f

typedef struct
{
    HAL_StatusTypeDef state;
    uint8_t battery_per;
    float battery_value;
} BATTERY_T;

typedef struct
{
    uint8_t usb_flag;
    uint8_t charge_flag;
} CHARGE_T;

BATTERY_T Battery_Read();
uint8_t Battery_Serve();
CHARGE_T Charg_Read();
void Charge_Serve();

#endif