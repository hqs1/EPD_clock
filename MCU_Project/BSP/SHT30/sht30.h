/**
* @file  sht30.h
* @author hequan
* @date 2021.01.29
* http://www.hqshome.cn
*/
#ifndef SHT_30_H
#define SHT_30_H

#include "main.h"

#define SHT30_ADDR 0x44
#define SHT30_Read_CMD 0x2C
#define SHT30_Read_High 0x06

typedef struct
{
    HAL_StatusTypeDef state;
    float temp;
    float humidity;
} SHT30_T;

SHT30_T SHT30_ReadData();

#endif