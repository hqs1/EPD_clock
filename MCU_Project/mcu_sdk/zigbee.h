/**
* @file  zigbee.h
* @brief define the macros which used in this SDK
* @author luchao
* @date 2020.03.13
* @par email:
* luchao@tuya.com
* @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
* @par company
* http://www.tuya.com
*/

#ifndef __ZIGBEE_H_
#define __ZIGBEE_H_

#include "stm32l0xx_hal.h"

#include "protocol.h"
#include "system.h"
#include "mcu_api.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define  MCU_SDK_VER   (1.0.1)

///< constant define 
#ifndef         TRUE
#define         TRUE                1
#endif

#ifndef         FALSE
#define         FALSE               0
#endif

#ifndef         NULL
#define         NULL                ((void *) 0)
#endif

#ifndef         SUCCESS
#define         SUCCESS             1
#endif

#ifndef         ERROR
#define         ERROR               0
#endif

#ifndef         INVALID
#define         INVALID             0xFF
#endif

#ifndef         ENABLE
#define         ENABLE                1
#endif

#ifndef         DISABLE
#define         DISABLE               0
#endif

///< dp type
#define         DP_TYPE_RAW                     0x00				 //RAW
#define         DP_TYPE_BOOL                    0x01	       //bool
#define         DP_TYPE_VALUE                   0x02	       //value
#define         DP_TYPE_STRING                  0x03				//string
#define         DP_TYPE_ENUM                    0x04				//enum 
#define         DP_TYPE_BITMAP                  0x05				//fault
#define         DP_TYPE_FAULT                   DP_TYPE_BITMAP    
//< work mode 
#define         NORMAL_MODE             0x00                      //normal mode
#define         FACTORY_MODE            0x01                     //factory mode 
#define         UPDATE_MODE             0X02                    //updata mode  


#define         ZG_NO_NWK               0x00                                    
#define         ZG_JOIN_NWK             0x01                                    
#define         ZG_JOIN_CLOUD           0X02                                   
#define         ZG_NO_CLOUD             0x03                                    
#define         ZG_REPORT_OK            0x10                                    
#define         ZG_REPORT_ERR           0X20                                    	

///< dp struct 
typedef struct {
  unsigned char dp_id;                     
  unsigned char dp_type;           
} DOWNLOAD_CMD_S;

#ifdef __cplusplus
}
#endif
#endif
