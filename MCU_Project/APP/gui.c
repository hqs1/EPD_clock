/**
* @file  gui.c
* @author hequan
* @date 2021.01.29
* http://www.hqshome.cn
*/
#include "gui.h"
#include "epdpaint.h"
#include "epd2in13.h"
#include "imagedata.h"
#include <string.h>
#include "SHT30.h"
#include "battery.h"
#include "zigbee.h"

Paint paint;
EPD epd;

uint8_t frame_buffer[EPD_WIDTH * EPD_HEIGHT / 8];

void GUI_Init(const unsigned char *model)
{
    if (EPD_Init(&epd, model) != 0)
    {
        return;
    }
}

void GUI_Clear()
{
    GUI_Init(lut_full_update);
    EPD_ClearFrameMemory(&epd, 0xFF);
    EPD_DisplayFrame(&epd);
    EPD_ClearFrameMemory(&epd, 0xFF);
    EPD_DisplayFrame(&epd);
    EPD_DelayMs(&epd, 1000);
}

void GUI_Init_UI(GUI_INIT_T data)
{
    char value[13];
    GUI_Clear();
    GUI_Init(lut_full_update);
    Paint_Init(&paint, frame_buffer, epd.width, epd.height);
    Paint_Clear(&paint, UNCOLORED);
    memcpy(frame_buffer, INIT_IMAGE_DATA, sizeof(frame_buffer));
    sprintf(value, "Ver:%s", MCU_VER);
    Paint_DrawStringAt(&paint, 5, 115, value, &Font12, COLORED);
    if (data == FLASH_ERROR)
    {
        Paint_DrawStringAt(&paint, 40, 98, "SPI FLASH", &Font12, COLORED);
        Paint_DrawString_CN(&paint, 110, 95, "�����֧��", &Font12CN, COLORED, UNCOLORED);
    }
    else if (data == SENSOR_ERROR)
    {
        Paint_DrawStringAt(&paint, 80, 98, "Sensor", &Font12, COLORED);
        Paint_DrawString_CN(&paint, 130, 95, "����", &Font12CN, COLORED, UNCOLORED);
    }
    EPD_SetFrameMemory(&epd, frame_buffer, WRITE_RAM, 0, 0, epd.width, epd.height);
    EPD_DisplayFrame(&epd);
    EPD_Sleep(&epd);
}

void GUI_SetNet_UI()
{
    GUI_Init(lut_full_update);
    Paint_Init(&paint, frame_buffer, epd.width, epd.height);
    Paint_Clear(&paint, UNCOLORED);
    EPD_SetFrameMemory(&epd, SETWIFI_IMAGE_DATA, WRITE_RAM, 0, 0, epd.width, epd.height);
    EPD_DisplayFrame(&epd);
    EPD_Sleep(&epd);
}

void GUI_Sleep_UI(SLEEP sleep)
{
    GUI_Init(lut_full_update);
    Paint_Init(&paint, frame_buffer, epd.width, epd.height);
    Paint_Clear(&paint, UNCOLORED);
    Paint_DrawFilledCircle(&paint, 65, 65, 50, COLORED);
    Paint_DrawFilledCircle(&paint, 100, 45, 50, UNCOLORED);
    if (sleep == JOINTIMEOUT)
    {
        Paint_DrawString_CN(&paint, 150, 40, "������ʱ", &Font12CN, COLORED, UNCOLORED);
        Paint_DrawString_CN(&paint, 140, 70, "����������", &Font12CN, COLORED, UNCOLORED);
    }
    else
    {
        Paint_DrawString_CN(&paint, 135, 40, "�豸δ����", &Font12CN, COLORED, UNCOLORED);
        Paint_DrawString_CN(&paint, 135, 70, "˯����", &Font12CN, COLORED, UNCOLORED);
    }
    EPD_SetFrameMemory(&epd, frame_buffer, WRITE_RAM, 0, 0, epd.width, epd.height);
    EPD_DisplayFrame(&epd);
    EPD_Sleep(&epd);
}

void GUI_PowerLow_UI(float battery_value)
{
    char value[20];
    GUI_Init(lut_full_update);
    Paint_Init(&paint, frame_buffer, epd.width, epd.height);
    Paint_Clear(&paint, UNCOLORED);
    sprintf(value, "voltage:%0.2fV", battery_value);
    Paint_DrawString_CN(&paint, 110, 40, "��ص�����", &Font12CN, COLORED, UNCOLORED);
    Paint_DrawString_CN(&paint, 128, 58, "����", &Font12CN, COLORED, UNCOLORED);
    Paint_DrawStringAt(&paint, 110, 80, value, &Font12, COLORED);

    for (uint8_t i = 0; i < 3; i++)
    {
        Paint_DrawRectangle(&paint, 38 - i, 21 - i, 56 + i, 27, COLORED);
        Paint_DrawRectangle(&paint, 20 - i, 27 - i, 75 + i, 112 + i, COLORED);
        Paint_DrawVerticalLine(&paint, 38 + i, 44 + i, 10, COLORED);
        Paint_DrawVerticalLine(&paint, 56 + i, 44 + i, 10, COLORED);
        Paint_DrawRectangle(&paint, 32 - i, 57 - i, 62 + i, 78 + i, COLORED);
        Paint_DrawVerticalLine(&paint, 47 + i, 80, 20, COLORED);
    }

    EPD_SetFrameMemory(&epd, frame_buffer, WRITE_RAM, 0, 0, epd.width, epd.height);
    EPD_DisplayFrame(&epd);
    EPD_Sleep(&epd);
}

void Paint_Main_UI_ALL(uint8_t Hours, uint8_t Minutes, float temp, float humidity, uint8_t battery_value, uint8_t charge_state)
{
    char string_buf[5];

    Paint_Init(&paint, frame_buffer, epd.width, epd.height);
    Paint_Clear(&paint, UNCOLORED);

    Paint_DrawFilledRectangle(&paint, 214, 11, 239, 23, COLORED);
    Paint_DrawFilledRectangle(&paint, 216, 13, 237, 21, UNCOLORED);
    Paint_DrawFilledRectangle(&paint, 239, 14, 242, 20, COLORED);
    Paint_DrawCircle(&paint, 103, 110, 2, COLORED);
    Paint_DrawHorizontalLine(&paint, 108, 111, 8, COLORED);
    Paint_DrawVerticalLine(&paint, 107, 112, 8, COLORED);
    Paint_DrawHorizontalLine(&paint, 108, 120, 8, COLORED);
    Paint_DrawLine(&paint, 214, 109, 208, 123, COLORED);
    Paint_DrawCircle(&paint, 208, 111, 1, COLORED);
    Paint_DrawCircle(&paint, 214, 120, 1, COLORED);
    if (charge_state == 1)
    {
        uint8_t batteryUI_num = battery_value / (100 / 7);
        for (uint8_t i = 0; i < batteryUI_num; i++)
        {
            Paint_DrawVerticalLine(&paint, 217 + i * 3, 14, 7, COLORED);
            Paint_DrawVerticalLine(&paint, 217 + i * 3 + 1, 14, 7, COLORED);
        }
    }
    else
    {
        Paint_DrawRectangle(&paint, 221, 14, 227, 20, COLORED);
        Paint_DrawHorizontalLine(&paint, 218, 15, 3, COLORED);
        Paint_DrawHorizontalLine(&paint, 218, 19, 3, COLORED);
        Paint_DrawHorizontalLine(&paint, 228, 17, 8, COLORED);
    }

    sprintf(string_buf, "%02d:%02d", Hours, Minutes);
    Paint_DrawStringAt(&paint, 25, 25, string_buf, &Font48, COLORED);

    memset(string_buf, 0, sizeof(string_buf));
    sprintf(string_buf, "%0.1f", temp - 0.05);
    Paint_DrawStringAt(&paint, 40, 108, string_buf, &Font20, COLORED);

    memset(string_buf, 0, sizeof(string_buf));
    sprintf(string_buf, "%0.1f", humidity - 0.05);
    Paint_DrawStringAt(&paint, 145, 108, string_buf, &Font20, COLORED);
}

void GUI_Main_UI_ALL(uint8_t Hours, uint8_t Minutes, float temp, float humidity, uint8_t battery_value, uint8_t charge_state)
{

    GUI_Clear();
    GUI_Init(lut_partial_update);
    Paint_Main_UI_ALL(Hours, Minutes, temp, humidity, battery_value, charge_state);
    EPD_SetFrameMemory(&epd, frame_buffer, WRITE_RAM, 0, 0, epd.width, epd.height);
    EPD_DisplayFrame(&epd);
    EPD_Sleep(&epd);
}

void GUI_UpDateTime(uint8_t Hours, uint8_t Minutes, float temp, float humidity, uint8_t battery_value, uint8_t charge_state)
{
    GUI_Init(lut_partial_update);
    EPD_SetFrameMemory(&epd, frame_buffer, 0x26, 0, 0, epd.width, epd.height);
    Paint_Main_UI_ALL(Hours, Minutes, temp, humidity, battery_value, charge_state);
    EPD_SetFrameMemory(&epd, frame_buffer, WRITE_RAM, 0, 0, epd.width, epd.height);
    EPD_DisplayFrame(&epd);
    EPD_Sleep(&epd);
}

void GUI_OtaUI(GUI_OTA_T data, _ota_mcu_fw fw_data)
{
    char string_buf[20];
    GUI_Clear();
    GUI_Init(lut_partial_update);
    Paint_Init(&paint, frame_buffer, epd.width, epd.height);
    Paint_Clear(&paint, UNCOLORED);
    Paint_DrawString_CN(&paint, 100, 10, "�̼�����", &Font12CN, COLORED, UNCOLORED);
    switch (data)
    {
    case GUI_FLASH_ERROR:

        for (uint8_t i = 0; i < 3; i++)
        {
            Paint_DrawCircle(&paint, 50, 70, 38 + i, COLORED);
        }
        Paint_DrawFilledRectangle(&paint, 48, 45, 52, 80, COLORED);
        Paint_DrawFilledCircle(&paint, 50, 92, 2, COLORED);
        Paint_DrawStringAt(&paint, 110, 50, "SPI FLASH", &Font12, COLORED);
        Paint_DrawString_CN(&paint, 110, 70, "�����֧��", &Font12CN, COLORED, UNCOLORED);
        break;

    case GUI_OTA_MESSAGE:

        float fwsize = fw_data.mcu_ota_fw_size;
        sprintf(string_buf, "OTA Ver:%d.%d.%d", (fw_data.mcu_ota_ver & 0xc0) >> 6, (fw_data.mcu_ota_ver & 0x30) >> 4, fw_data.mcu_ota_ver & 0x0f);
        Paint_DrawStringAt(&paint, 5, 50, string_buf, &Font12, COLORED);
        memset(string_buf, 0, sizeof(string_buf));
        sprintf(string_buf, "OTA size:%0.2fKB", fwsize / 1024);
        Paint_DrawStringAt(&paint, 5, 65, string_buf, &Font12, COLORED);
        Paint_DrawString_CN(&paint, 5, 85, "�̼������У�����ϵ磡", &Font12CN, COLORED, UNCOLORED);
        break;

    case GUI_OTA_ERROR:

        for (uint8_t i = 0; i < 3; i++)
        {
            Paint_DrawCircle(&paint, 50, 70, 38 + i, COLORED);
        }
        Paint_DrawFilledRectangle(&paint, 48, 45, 52, 80, COLORED);
        Paint_DrawFilledCircle(&paint, 50, 92, 2, COLORED);
        Paint_DrawString_CN(&paint, 110, 50, "�̼�������ʱ", &Font12CN, COLORED, UNCOLORED);
        Paint_DrawString_CN(&paint, 110, 70, "�����ԣ�", &Font12CN, COLORED, UNCOLORED);
        break;

    case GUI_OTA_SUCCESS:
        Paint_DrawString_CN(&paint, 5, 50, "�̼����ճɹ�,", &Font12CN, COLORED, UNCOLORED);
        Paint_DrawString_CN(&paint, 5, 70, "�豸�����������й̼�����", &Font12CN, COLORED, UNCOLORED);
        Paint_DrawString_CN(&paint, 5, 90, "����ϵ磡", &Font12CN, COLORED, UNCOLORED);
        break;
    }
    EPD_SetFrameMemory(&epd, frame_buffer, WRITE_RAM, 0, 0, epd.width, epd.height);
    EPD_DisplayFrame(&epd);
}

/*
��������ʾOTA���£�������Ļˢ��̫��,���Ӹ���ʱ��
˫SPI+DMA��ʽ�ɻ���
(����)
void GUI_OtaUI(uint8_t pro_value, uint32_t fwsize, uint8_t fwver)
{
    char string_buf[20];
    uint8_t xvalue;
    float fwsizef = fwsize;

    if (pro_value > 100)
        return;
    GUI_Init(lut_partial_update);
    Paint_Init(&paint, frame_buffer, epd.width, epd.height);
    Paint_Clear(&paint, UNCOLORED);
    Paint_DrawString_CN(&paint, 100, 10, "�̼�����", &Font12CN, COLORED, UNCOLORED);

    if (pro_value != 0)
    {
        sprintf(string_buf, "OTA Ver:%d.%d.%d", (fwver & 0xc0) >> 6, (fwver & 0x30) >> 4, fwver & 0x0f);
        Paint_DrawStringAt(&paint, 5, 50, string_buf, &Font12, COLORED);
        memset(string_buf, 0, sizeof(string_buf));
        sprintf(string_buf, "OTA size:%0.2fKB", fwsizef / 1024);
        Paint_DrawStringAt(&paint, 5, 65, string_buf, &Font12, COLORED);
        Paint_DrawRectangle(&paint, 5, 80, 245, 110, COLORED);
        xvalue = pro_value * 2.36 + 7;
        Paint_DrawFilledRectangle(&paint, 7, 82, xvalue, 108, COLORED);
        memset(string_buf, 0, sizeof(string_buf));
        sprintf(string_buf, "%d%%", pro_value);
        Paint_DrawStringAt(&paint, xvalue - 45, 90, string_buf, &Font16, UNCOLORED);
    }
    else
    {
        for (uint8_t i = 0; i < 3; i++)
        {
            Paint_DrawCircle(&paint, 50, 70, 38 + i, COLORED);
        }
        Paint_DrawFilledRectangle(&paint, 48, 45, 52, 80, COLORED);
        Paint_DrawFilledCircle(&paint, 50, 92, 2, COLORED);
        Paint_DrawStringAt(&paint, 110, 50, "SPI FLASH", &Font12, COLORED);
        Paint_DrawString_CN(&paint, 110, 70, "�����֧��", &Font12CN, COLORED, UNCOLORED);
    }
    EPD_SetFrameMemory(&epd, frame_buffer, WRITE_RAM, 0, 0, epd.width, epd.height);
    EPD_DisplayFrame(&epd);
    EPD_Sleep(&epd);
}

*/