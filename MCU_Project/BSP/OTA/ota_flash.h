/**
* @file  ota_flash.h
* @author hequan
* @date 2021.01.29
* http://www.hqshome.cn
*/
#ifndef OTA_FLASH_H
#define OTA_FLASH_H

#include "stm32l0xx_hal.h"

#define SPI_FLASH_DELAY 10
#define COMMAND_WREN 0x06
#define COMMAND_WRDI 0x04
#define COMMAND_RDSR 0x05
#define COMMAND_WRSR 0x01
#define COMMAND_READ 0x03
#define COMMAND_FREAD 0x0B
#define COMMAND_WRITE 0x02 // page program command
#define COMMAND_RDID 0x9F
#define COMMAND_SE 0x20
#define COMMAND_BE 0x52
#define COMMAND_CE 0x60

#define STATUS_WIP 0x01
#define STATUS_WEL 0x02
#define STATUS_BP0 0x04
#define STATUS_BP1 0x08
#define STATUS_BP2 0x10
#define STATUS_BP3 0x20
#define STATUS_RES 0x40
#define STATUS_SWRD 0x80

#define DUMMY 0xFF
#define BlockoneADDR 0x0
// #define BlocktwoADDR 0x010000

//mxic flash
#define MXICID 0xC2
#define MEMORY_TYPE 0x20

void Get_FlashID(uint8_t *data);
uint8_t OTA_Flash_Erase();
void OTA_Write_Flash(uint32_t fwsize, uint32_t address, uint8_t *data, uint8_t length);
#endif