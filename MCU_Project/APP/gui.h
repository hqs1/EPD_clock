/**
* @file  gui.h
* @author hequan
* @date 2021.01.29
* http://www.hqshome.cn
*/
#ifndef GUI_H
#define GUI_H

#include "main.h"
#include "zigbee.h"
typedef enum
{
    JOINTIMEOUT,
    UNJOINTIMEOUT
} SLEEP;

typedef enum
{
    GUI_FLASH_ERROR,
    GUI_OTA_MESSAGE,
    GUI_OTA_ERROR,
    GUI_OTA_SUCCESS
} GUI_OTA_T;

typedef enum
{
    NOERROR,
    FLASH_ERROR,
    SENSOR_ERROR
} GUI_INIT_T;

void GUI_Init(const unsigned char *model);
void GUI_Clear();
void GUI_Init_UI(GUI_INIT_T data);
void GUI_SetNet_UI();
void GUI_Sleep_UI(SLEEP sleep);
void GUI_Main_UI_ALL(uint8_t Hours, uint8_t Minutes, float temp, float humidity, uint8_t battery_value, uint8_t charge_state);
void GUI_UpDateTime(uint8_t Hours, uint8_t Minutes, float temp, float humidity, uint8_t battery_value, uint8_t charge_state);
void GUI_PowerLow_UI(float battery_value);
void GUI_OtaUI(GUI_OTA_T data, _ota_mcu_fw fw_data);
#endif