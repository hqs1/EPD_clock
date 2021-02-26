/**
* @file  system.h
* @brief declaration of fuction in system.c and define maroc which is used in system.c
* @author luchao
* @date 2020.03.13
* @par email:
* luchao@tuya.com
* @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
* @par company
* http://www.tuya.com
*/

#include "protocol.h"

#ifndef __SYSTEM_H_
#define __SYSTEM_H_


#ifdef SYSTEM_GLOBAL
  #define SYSTEM_EXTERN
#else
  #define SYSTEM_EXTERN   extern
#endif

#ifdef SUPPORT_MCU_RTC_CHECK

typedef struct 
{
	unsigned char hour;
	unsigned char min;
	unsigned char sec;
	unsigned short w_year;
	unsigned char  w_month;
	unsigned char  w_date; 
}_calendar_obj; 
#endif


#ifdef SUPPORT_MCU_OTA
#define FW_SINGLE_PACKET_SIZE 		0x20	     ///< firmware single packet size, define by mcu 
SYSTEM_EXTERN unsigned char current_mcu_pid[8];	///< current mcu pid

/** ota mcu firmware struct*/
typedef struct
{
	unsigned char mcu_ota_pid[8];	  //OTA fw PID
	unsigned char mcu_ota_ver;	     //OTA fw version
	unsigned int mcu_ota_fw_size;	 //OTA fw size
	unsigned int mcu_current_offset; //current offset 
	unsigned int mcu_ota_checksum;  //OTA fw checksum	
}_ota_mcu_fw;

SYSTEM_EXTERN _ota_mcu_fw ota_fw_info;	///< ota firmware infomation
#endif

#ifdef SET_ZIGBEE_NWK_PARAMETER
typedef struct
{
	unsigned short   heart_period; 
	unsigned short   join_nwk_time;	    
	unsigned short   rejion_period;	 
	unsigned short   poll_period; 
	unsigned short   fast_poll_period;  
	unsigned char    poll_failure_times;  
	unsigned char    rejoin_try_times;  
	unsigned char    app_trigger_rejoin; 
	unsigned char    rf_send_power;  
}nwk_parameter_t
#endif

///< Byte order of frames
#define         HEAD_FIRST                      0
#define         HEAD_SECOND                     1
#define         PROTOCOL_VERSION                2
#define         SEQ_HIGH                        3
#define         SEQ_LOW                         4
#define         FRAME_TYPE                      5
#define         LENGTH_HIGH                     6
#define         LENGTH_LOW                      7
#define         DATA_START                      8

///< define cmd of frame 
#define         PRODUCT_INFO_CMD                1                               //pid information 
#define         ZIGBEE_STATE_CMD                2                               //zigbee module send network state to mcu 	
#define         ZIGBEE_CFG_CMD                  3                               //mcu config zigbee state 
#define         ZIGBEE_DATA_REQ_CMD             4                               //zigbee module shend DP data to mcu 
#define         DATA_DATA_RES_CMD               5                               //mcu response zigbee DP send 
#define         DATA_REPORT_CMD                 6                               //mcu report DP data to zigbee 
#define         ZIGBEE_RF_TEST_CMD              8                               // 
#define         DYNAMIC_TIME_CMD                24                              //
#define         QUERY_KEY_INFO_CMD              9                               //
#define         CALL_SCENE_CMD                  10                              //

#define         TIME_GET_CMD                        0x24         
#define         CHECK_MCU_TYPE_CMD                  0x25      
#define         SET_ZIGBEE_NEK_PARAMETER_CMD        0x26  
#define         SEND_BROADCAST_DATA_CMD             0x27         

#define			MCU_OTA_VERSION_CMD				        0x0B							//zigbee request mcu version 
#define			MCU_OTA_NOTIFY_CMD				    	0x0C							//mcu ota notify 
#define			MCU_OTA_DATA_REQUEST_CMD			    0x0D							//MCU OTA data request 
#define			MCU_OTA_RESULT_CMD					    0x0E							//MCU OTA result report 

///< frme data define 
#define SERIAL_PROTOCOL_VER                 0x02                                            // the version of frame 
#define PROTOCOL_HEAD                       0x09                                           //the lcation of frame length except for tail
#define FIRST_FRAME_HEAD                    0x55                                            // first byte of frame 
#define SECOND_FRAME_HEAD                   0xaa                                            // second byte of frame 

///< define communication state 
#define ZIGBEE_NOT_JION							        0x00	//zigbee module not jion network
#define ZIGBEE_JOIN_GATEWAY								0x01	//zigbee module had jioned network 
#define ZIGBEE_JOIN_ERROR								0x02	//zigbee module network state error
#define ZIGBEE_JOINING							    	0x03	//zigbee module jioning 

#define REPORT_DATA_SUCESS	                            0x10	//translate sucess
#define REPORT_DATA_FAILURE	                            0x20	//translate failure 
#define REPORT_DATA_TIMEOUT	                            0x40	//translate timeout 
#define REPORT_DATA_BUSY                                0x80	//translate busy

#define RESET_ZIGBEE_OK			0x00	//rest zigbee success
#define RESET_ZIGBEE_ERROR  	0x01	//reset zigbee error

SYSTEM_EXTERN volatile unsigned char zigbee_queue_buf[PROTOCOL_HEAD + ZIGBEE_UART_QUEUE_LMT];        ///< serial received queue 
SYSTEM_EXTERN volatile unsigned char zigbee_uart_rx_buf[PROTOCOL_HEAD + ZIGBEE_UART_RECV_BUF_LMT];   ///< serial received buf
SYSTEM_EXTERN volatile unsigned char zigbee_uart_tx_buf[PROTOCOL_HEAD + ZIGBEE_UART_SEND_BUF_LMT];   ///< serial translate buf

SYSTEM_EXTERN volatile unsigned char *queue_in;
SYSTEM_EXTERN volatile unsigned char *queue_out;


SYSTEM_EXTERN volatile unsigned char zigbee_work_state;                                              ///< the state of zigbee module working 

SYSTEM_EXTERN volatile unsigned char rx_in;         	 ///< recieved a complete frame of data flag 
SYSTEM_EXTERN volatile unsigned char wakeup_flag;	    ///<  initiative wakeup flag 

SYSTEM_EXTERN unsigned char record_report_time_flag;	///<  0x00：using gateway time；0x01：using mcu local time

SYSTEM_EXTERN unsigned char timestamp[4];	
SYSTEM_EXTERN _calendar_obj _time;	

/**
* @brief padding a byte in send buf base on dest
* @param[in] {dest} the location of padding byte 
* @param[in] {byte} padding byte 
* @return  sum of frame after this operation 
*/
unsigned short set_zigbee_uart_byte(unsigned short dest, unsigned char byte);

/**
* @brief padding buf in send buf base on dest
* @param[in] {dest} the location of padding 
* @param[in] {src}  the head address of padding buf
* @param[in] {len}  the length of padding buf
* @return  sum of frame after this operation 
*/
unsigned short set_zigbee_uart_buffer(unsigned short dest, unsigned char *src, unsigned short len);

/**
* @brief send len bytes data
* @param[in] {in} the head address of send data
* @param[in] {len}  the length of send data
* @return  void
*/
void zigbee_uart_write_data(unsigned char *in, unsigned short len);

/**
* @brief calculate the sum of the array
* @param[in] {pack} the head address of array
* @param[in] {pack_len}  the length of  array
* @return  sum
*/
unsigned char get_check_sum(unsigned char *pack, unsigned short pack_len);

/**
* @brief send a frame data 
* @param[in] {fr_cmd} frame cmd id
* @param[in] {len}    len of frame data 
* @return  void
*/
void zigbee_uart_write_frame(unsigned char fr_cmd, unsigned short len);

/**
* @brief handle received frame from zigbee module baseon frame cmd id
* @param[in] {void}
* @return  result of handle
*/
int data_handle(unsigned short offset);

/**
* @brief mcu send a cmd which used to making zigbee module leave network
* @param[in] {void}
* @return  void
*/
void mcu_exit_zigbee(void);

/**
* @brief mcu send a cmd which used to making zigbee module restart jion network
* @param[in] {void}
* @return  void
*/
void mcu_join_zigbee(void);

/**
* @brief mcu send a cmd which used to getting zigbee network state 
* @param[in] {void}
* @return  void
*/
void mcu_get_zigbee_state(void);


#ifdef SUPPORT_MCU_RTC_CHECK
/**
* @brief mcu send a cmd which used to getting timestamp
* @param[in] {void}
* @return void
*/
void mcu_get_system_time(void);

/**
* @brief calculta time from frame 
* @param[in] {void}
* @return  void
*/
void zigbee_timestamp_to_time(void);
#endif



/**
* @brief get mcu pid data dan saved in current_mcu_pid
* @param[in] {void}
* @return  result of handle
*/
void current_mcu_fw_pid(void);

#ifdef SET_ZIGBEE_NWK_PARAMETER
/**
* @brief mcu can set zigbee nwk parameter
* @param[in] {parameter_t *} 
* @param
* @return void
*/
 void mcu_set_zigbee_nwk_parameter(nwk_parameter_t *Pparameter);
#endif

#ifdef BROADCAST_DATA_SEND
/**
* @brief mcu can send a broadcast data in zigbee network
* @param[in] {buf} array of buf which to be sended
* @param[in] {buf_len} send buf length
* @return void
*/
void mcu_send_broadcast_data(unsigned char buf[], unsigned char buf_len);
#endif

/**
* @brief mcu version string to char
* @param[in] {void}
* @return  result of version
*/
unsigned char get_current_mcu_fw_ver(void);

#ifdef SUPPORT_MCU_OTA 
/**
* @brief mcu ota offset requset 
* @param[in] {packet_offset}  packet offset 
* @return  viod
*/
void mcu_ota_fw_request();

/**
* @brief mcu ota result report 
* @param[in] {status} result of mcu ota
* @return  void
*/
void mcu_ota_result_report(unsigned char status);
#endif


/**
* @brief compare str1 and str2
* @param[in] {str1} str1 
* @param[in] {str2} str2
* @return  equal return 0 else return -1
*/
int strcmp_barry(unsigned char *str1,unsigned char *str2);

/**
* @brief translate assic to hex
* @param[in] {assic_num} assic number
* @return hex data
*/
char assic_to_hex(unsigned char assic_num);

#ifdef __cplusplus
}
#endif
#endif
  
  
