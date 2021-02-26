/**
* @file  ota_flash.c
* @author hequan
* @date 2021.01.29
* http://www.hqshome.cn
*/
#include "ota_flash.h"
#include "spi.h"
#include "gpio.h"
#include "usart.h"
#include "zigbee.h"
#include "gui.h"

uint8_t EraseFlag = 0;
uint8_t pagecount = 0;

static HAL_StatusTypeDef SPI_Transfer_data(uint8_t *data, uint32_t length);
static HAL_StatusTypeDef SPI_Transfer_cmd(uint8_t cmd);
static void Set_Address(uint32_t address);
static uint8_t Get_FlashStatus();
static void flash_Write(uint32_t address, uint8_t *data, uint32_t length);
static void Erase_FlashBlock(uint32_t address);
static void GPIO_FLASH_init();

static void GPIO_FLASH_init()
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(FLASH_EN_GPIO_Port, FLASH_EN_Pin, GPIO_PIN_RESET);
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    /*Configure GPIO pins : PAPin PAPin */
    GPIO_InitStruct.Pin = FLASH_CS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(FLASH_CS_GPIO_Port, &GPIO_InitStruct);
    /*Configure GPIO pins : PBPin PBPin */
    GPIO_InitStruct.Pin = FLASH_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(FLASH_EN_GPIO_Port, &GPIO_InitStruct);
}

static HAL_StatusTypeDef SPI_Transfer_data(uint8_t *data, uint32_t length)
{
    HAL_StatusTypeDef state = HAL_OK;
    while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY)
        ;
    state = HAL_SPI_Transmit(&hspi1, data, length, length * SPI_FLASH_DELAY);
    if (state != HAL_OK)
        Error_Handler();
    return state;
}

static HAL_StatusTypeDef SPI_Transfer_cmd(uint8_t cmd)
{

    HAL_StatusTypeDef state = HAL_OK;
    uint8_t data = cmd;
    while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY)
        ;
    state = HAL_SPI_Transmit(&hspi1, &data, 1, SPI_FLASH_DELAY);
    if (state != HAL_OK)
        Error_Handler();
    return state;
}

static void Set_Address(uint32_t address)
{
    uint8_t addr[3];
    addr[0] = (uint8_t)(address >> 16);
    addr[1] = (uint8_t)(address >> 8);
    addr[2] = (uint8_t)(address);
    SPI_Transfer_data(&addr[0], 3);
}
static uint8_t Get_FlashStatus()
{
    uint8_t status;
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);
    SPI_Transfer_cmd(COMMAND_RDSR);
    HAL_SPI_Receive(&hspi1, &status, 1, SPI_FLASH_DELAY);
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);
    return status;
}

static void Erase_FlashBlock(uint32_t address)
{
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);
    SPI_Transfer_cmd(COMMAND_WREN);
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);
    SPI_Transfer_cmd(COMMAND_BE);
    Set_Address(address);
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);
    while (Get_FlashStatus() & STATUS_WIP)
        ;
}

void Get_FlashID(uint8_t *data)
{
    GPIO_FLASH_init();
    HAL_Delay(10);
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);
    SPI_Transfer_cmd(COMMAND_RDID);
    HAL_SPI_Receive(&hspi1, data, 3, 0xff);
    HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);
}

uint8_t OTA_Flash_Erase()
{
    uint8_t flashid[3];

    //check flash
    Get_FlashID(flashid);
    if (flashid[0] != MXICID && flashid[1] != MEMORY_TYPE)
    {
        //spi flash id error
        return 0;
    }
    //erase block
    Erase_FlashBlock(BlockoneADDR);
    return 1;
}

void OTA_Write_Flash(uint32_t fwsize, uint32_t address, uint8_t *data, uint8_t length)
{
    if (address == 0)
        pagecount = 0;
    if (pagecount == 0 || pagecount > 7)
    {
        HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);
        while (Get_FlashStatus() & STATUS_WIP)
            ;
        HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);
        SPI_Transfer_cmd(COMMAND_WREN);
        HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET);
        SPI_Transfer_cmd(COMMAND_WRITE);
        Set_Address(address);
    }
    SPI_Transfer_data(data, length);
    pagecount++;
    //send last page data
    if (address + length >= fwsize)
    {
        HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET);
        while (Get_FlashStatus() & STATUS_WIP)
            ;
        //set otaflag to eeprom addr:0x08080000UL
        HAL_FLASHEx_DATAEEPROM_Unlock();
        HAL_FLASHEx_DATAEEPROM_Erase(DATA_EEPROM_BASE);
        HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, DATA_EEPROM_BASE, 0x01);
        HAL_FLASHEx_DATAEEPROM_Lock();
    }
}
