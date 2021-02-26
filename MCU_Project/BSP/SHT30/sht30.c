/**
* @file  sht30.c
* @author hequan
* @date 2021.01.29
* http://www.hqshome.cn
*/
#include "sht30.h"
#include "main.h"
#include "i2c.h"

SHT30_T SHT30_ReadData()
{
    MX_I2C1_Init();

    uint8_t IIC_TXbuf[2] = {SHT30_Read_CMD, SHT30_Read_High};
    uint8_t IIC_RXbuf[6];

    HAL_StatusTypeDef status = HAL_OK;
    SHT30_T data;

    status = HAL_I2C_Master_Transmit(&hi2c1, SHT30_ADDR << 1, IIC_TXbuf, 2, 0xff);
    if (status != HAL_OK)
    {
        HAL_I2C_DeInit(&hi2c1);
        MX_I2C1_Init();
    }
    status = HAL_I2C_Master_Receive(&hi2c1, SHT30_ADDR << 1, IIC_RXbuf, 6, 0xff);
    if (status != HAL_OK)
    {
        HAL_I2C_DeInit(&hi2c1);
        MX_I2C1_Init();
    }
    data.state = status;
    data.temp = (float)(((IIC_RXbuf[0] << 8) + IIC_RXbuf[1]) * 175) / 65535 - 45;
    data.humidity = (float)(((IIC_RXbuf[3] << 8) + IIC_RXbuf[4]) * 100) / 65535;
    HAL_I2C_DeInit(&hi2c1);
    return data;
}
