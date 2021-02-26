/**
* @file  protocol.c
* @brief this file contains protocol analysis and construct response function when received zigbee module send message
* @author luchao
* @date 2020.03.13
* @par email:
* luchao@tuya.com
* @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
* @par company
* http://www.tuya.com
*/

#include "zigbee.h"
#include "usart.h"
#include "main.h"
#include "ota_flash.h"
#include "main.h"
/******************************************************************************
                                移植须知:
1:MCU必须在while中直接调用mcu_api.c内的zigbee_uart_service()函数
2:程序正常初始化完成后,建议不进行关串口中断,如必须关中断,关中断时间必须短,关中断会引起串口数据包丢失
3:请勿在中断/定时器中断内调用上报函数
******************************************************************************/
/******************************************************************************
                              第一步:初始化
1:在需要使用到zigbee相关文件的文件中include "zigbee.h"
2:在MCU初始化中调用mcu_api.c文件中的zigbee_protocol_init()函数
3:将MCU串口单字节发送函数填入protocol.c文件中uart_transmit_output函数内,并删除#error
4:在MCU串口接收函数中调用mcu_api.c文件内的uart_receive_input函数,并将接收到的字节作为参数传入
5:单片机进入while循环后调用mcu_api.c文件内的zigbee_uart_service()函数
******************************************************************************/

/******************************************************************************
                        1:dp数据点序列类型对照表
          **此为自动生成代码,如在开发平台有相关修改请重新下载MCU_SDK**         
******************************************************************************/

///> dp data list, this will be generated by cloud platform
const DOWNLOAD_CMD_S download_cmd[] =
	{
		{DPID_TEMP_CURRENT, DP_TYPE_VALUE},
		{DPID_HUMIDITY_VALUE, DP_TYPE_VALUE},
		{DPID_BATTERY_STATE, DP_TYPE_ENUM},
		{DPID_BATTERY_PERCENTAGE, DP_TYPE_VALUE},
		{DPID_CHARGE_STATE, DP_TYPE_BOOL},
};

/******************************************************************************
                           2:串口单字节发送函数
请将MCU串口发送函数填入该函数内,并将接收到的数据作为参数传入串口发送函数
******************************************************************************/

/**
* @brief encapsulates a generic send function, developer should use their own function to completing this fuction 
* @param[in] {value} send signle data 
* @return  void
*/
void uart_transmit_output(unsigned char value)
{
	USART1_SendByte(value);
}

/******************************************************************************
                           第二步:实现具体用户函数
1:APP下发数据处理
2:数据上报处理
******************************************************************************/

/******************************************************************************
                            1:所有数据上报处理
当前函数处理全部数据上报(包括可下发/可上报和只上报)
  需要用户按照实际情况实现:
  1:需要实现可下发/可上报数据点上报
  2:需要实现只上报数据点上报
此函数为MCU内部必须调用
用户也可调用此函数实现全部数据上报
******************************************************************************/

/******************************************************************************
                                WARNING!!!    
                            2:所有数据上报处理
自动化代码模板函数,具体请用户自行实现数据处理
******************************************************************************/

///> this will realize by  cloud platform

#ifdef SUPPORT_MCU_RTC_CHECK
/**
* @brief mcu check local RTC time 
* @param[in] {time} timestamp
* @return  void
*/
void mcu_write_rtctime(unsigned char time[])
{
	/*
  time[0]~time[3]：standard time
  time[4]~time[7]: Local time
 */
	my_memcpy((void *)timestamp, &time[4], 4); //get timestamp
	zigbee_timestamp_to_time();
	APP_RTC_Set(_time.hour, _time.min, _time.sec, &app_data);
	/*
	year = _time.w_year;	//year
	month = _time.w_month;	//month
	date = _time.w_date;	//date
	hour = _time.hour + 8;	//hour(8:BeiJing time)
	min = _time.min;	//minute
	sec = _time.sec;	//second
*/
}
#endif

/**
* @brief Zigbee functional test feedback
* @param[in] {void} 
* @return  void
*/
void zigbee_test_result(void)
{
	//   #error "this test is makesure the rf fuction of zigbee module, if test pass or not should do something, mcu should realize"
	unsigned char rssi = zigbee_uart_rx_buf[DATA_START];

	if (rssi > 0x3C)
	{
		//test sucess the range of rssi is 0% ~ 100%
	}
	else
	{
		//test failure
	}
}

/******************************************************************************
                                WARNING!!!                     
以下函数用户请勿修改!!
******************************************************************************/

/**
* @brief this function will handle uart received frame data  
* @param[in] {dpid}   dp id
* @param[in] {value}  dp data 
* @param[in] {length} lenght of dp data 
* @return  handle result 
*/
unsigned char dp_download_handle(unsigned char dpid, const unsigned char value[], unsigned short length)
{
	/* only list of function, mcu need realize these fuction*/
	unsigned char ret;
	switch (dpid)
	{

	default:
		break;
	}
	return ret;
}

/**
* @brief get received cmd total number
* @param[in] {void}   
* @return  received cmd total number
*/
unsigned char get_download_cmd_total(void)
{
	return (sizeof(download_cmd) / sizeof(download_cmd[0]));
}

/**
* @brief received zigbee net_work state handle 
* @param[in] {zigbee_work_state}  zigbee current network state
* @return  void 
*/
void zigbee_work_state_event(unsigned char zigbee_work_state)
{
	unsigned short length = 0;

	length = set_zigbee_uart_byte(length, 0x10);
	zigbee_uart_write_frame(ZIGBEE_STATE_CMD, length);
	Network_Handler(zigbee_work_state, &app_data);
}

/**
* @brief received reset zigbee response 
* @param[in] {state} response state 
* @return  void 
*/
void mcu_reset_zigbee_event(unsigned char state)
{
	switch (state)
	{
	case RESET_ZIGBEE_OK:

		break;

	case RESET_ZIGBEE_ERROR:

		break;

	default:
		break;
	}
}

/**
* @brief check mcu version response
* @param[in] {void}
* @return  void 
*/
void response_mcu_ota_version_event(void)
{
	unsigned short length = 0;
	length = set_zigbee_uart_byte(length, get_current_mcu_fw_ver()); //current fw version
	zigbee_uart_write_frame(MCU_OTA_VERSION_CMD, length);
}

#ifdef SUPPORT_MCU_OTA
/**
* @brief mcu ota update notify response
* @param[in] {offset} data offset 
* @return  void 
*/
void response_mcu_ota_notify_event(unsigned char offset)
{
	unsigned char i = 0;
	unsigned short length = 0;

	current_mcu_fw_pid(); //current PID

	while (i < 8)
	{
		ota_fw_info.mcu_ota_pid[i] = zigbee_uart_rx_buf[offset + DATA_START + i]; //ota fw PID
		i++;
	}
	ota_fw_info.mcu_ota_ver = zigbee_uart_rx_buf[offset + DATA_START + 8]; //ota fw version
	ota_fw_info.mcu_ota_fw_size = zigbee_uart_rx_buf[offset + DATA_START + 9] << 24 |
								  zigbee_uart_rx_buf[offset + DATA_START + 10] << 16 |
								  zigbee_uart_rx_buf[offset + DATA_START + 11] << 8 |
								  zigbee_uart_rx_buf[offset + DATA_START + 12]; //ota fw size
	ota_fw_info.mcu_ota_checksum = zigbee_uart_rx_buf[offset + DATA_START + 13] << 24 |
								   zigbee_uart_rx_buf[offset + DATA_START + 14] << 16 |
								   zigbee_uart_rx_buf[offset + DATA_START + 15] << 8 |
								   zigbee_uart_rx_buf[offset + DATA_START + 16]; //ota fw checksum

	if ((ota_fw_info.mcu_ota_ver > get_current_mcu_fw_ver() && ota_fw_info.mcu_ota_fw_size > 0))
	{												 //check fw pid and fw version and fw size
		length = set_zigbee_uart_byte(length, 0x00); //OK
	}
	else
	{
		length = set_zigbee_uart_byte(length, 0x01); //error
	}
	ota_fw_info.mcu_current_offset = 0;
	zigbee_uart_write_frame(MCU_OTA_NOTIFY_CMD, length);
	//start receive fw
	app_data.state = MCU_OTA;
}

/**
* @brief mcu send ota data request 
* @param[in] {void}  
* @return  void 
*/
void mcu_ota_fw_request_event(unsigned char offset)
{
	unsigned int fw_offset = 0;
	unsigned char fw_data[FW_SINGLE_PACKET_SIZE] = {-1}; //
	unsigned char i = 0;

	if (zigbee_uart_rx_buf[offset + DATA_START] == 0x01) //status check
		return;
	while (i < 8)
	{
		if (current_mcu_pid[i] != zigbee_uart_rx_buf[offset + DATA_START + 1 + i]) //pid check
			return;
		i++;
	}
	if (ota_fw_info.mcu_ota_ver != zigbee_uart_rx_buf[offset + DATA_START + 9]) //version check
		return;

	i = 0;
	while (i < 4)
	{
		fw_offset |= (zigbee_uart_rx_buf[offset + DATA_START + 10 + i] << (24 - i * 8)); //offset
		i++;
	}
	i = 0;

	if (ota_fw_info.mcu_current_offset == fw_offset)
	{
		if ((ota_fw_info.mcu_ota_fw_size - fw_offset) / FW_SINGLE_PACKET_SIZE != 0)
		{
			while (i < FW_SINGLE_PACKET_SIZE)
			{
				fw_data[i] = zigbee_uart_rx_buf[offset + DATA_START + 14 + i]; //fw data
				i++;
			}
			ota_fw_info.mcu_current_offset += FW_SINGLE_PACKET_SIZE;
			OTA_Write_Flash(ota_fw_info.mcu_ota_fw_size, fw_offset, &fw_data[0], i);
			mcu_ota_fw_request(); //request next page fwdata
			app_data.ota.ota_timeout = HAL_GetTick();
		}
		else
		{
			i = 0;
			while (i < (ota_fw_info.mcu_ota_fw_size - fw_offset))
			{
				fw_data[i] = zigbee_uart_rx_buf[offset + DATA_START + 14 + i];
				i++;
			}
			OTA_Write_Flash(ota_fw_info.mcu_ota_fw_size, fw_offset, &fw_data[0], i);
			app_data.ota.state = OTA_SUCCESS;
		}
	}
	else
	{
		// ota request timeout, then restart ota request from  ota_fw_info.mcu_ota_fw_size
	}
}

void report_mcu_ota_result(unsigned char res)
{
	unsigned short length;
	length = set_zigbee_uart_byte(length, res);
	zigbee_uart_write_frame(MCU_OTA_NOTIFY_CMD, length);
}

/**
* @brief mcu ota data result notify
* @param[in] {void} 
* @return  void 
*/
void mcu_ota_result_event(unsigned char offset)
{
	unsigned char status = zigbee_uart_rx_buf[offset + DATA_START];

	if (status == 0x00)
	{
	}
	else if (status == 0x01)
	{
	}
}

#endif