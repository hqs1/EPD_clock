/**
* @file   mcu_api.h
* @brief  declaration of fuction in mcu_api.c
* @author luchao
* @date 2020.03.13
* @par email:
* luchao@tuya.com
* @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
* @par company
* http://www.tuya.com
*/

#ifndef __MCU_API_H_
#define __MCU_API_H_

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef MCU_API_GLOBAL
  #define MCU_API_EXTERN
#else
  #define MCU_API_EXTERN   extern
#endif

/**
* @brief hex translate to bcd 
* @param[in] {Value_H} higher bits data 
* @param[in] {Value_L} lower bits data
* @return  bcd type data
*/
unsigned char hex_to_bcd(unsigned char Value_H,unsigned char Value_L);

/**
* @brief get string len 
* @param[in] {str} higher bits data 
* @return string len 
*/
unsigned long my_strlen(unsigned char *str);

/**
* @brief  assign ch to the first count bytes of the memory address src
* @param[in] {src}    srouce address
* @param[in] {ch}     set value 
* @param[in] {count}  length of set address 
* @return void
*/
void *my_memset(void *src,unsigned char ch,unsigned short count);

/**
* @brief copy count bytes data from src to dest
* @param[in] {src}    srouce address
* @param[in] {dest}   destination address
* @param[in] {count}  length of copy data
* @return void
*/
void *my_memcpy(void *dest, const void *src, unsigned short count);

/**
* @brief copy string src to string dest
* @param[in] {src}    srouce address
* @param[in] {dest}   destination address
* @return the tail of destination 
*/
char *my_strcpy(char *dest, const char *src);

/**
* @brief compare string s1 with string s2
* @param[in] {s1}  srouce address
* @param[in] {s2}  destination address
* @return compare result
*/
int my_strcmp(char *s1 , char *s2);

/**
* @brief int translate to byte
* @param[in] {number} int data 
* @param[out] {value} the result array     
* @return void
*/
void int_to_byte(unsigned long number,unsigned char value[4]);

/**
* @brief byte data translate to int 
* @param[in] {value}  the byte array 
* @return result of int data 
*/
unsigned long byte_to_int(const unsigned char value[4]);

/**
* @brief report bool type DP data to zigbee module 
* @param[in]  {dpid}  dp id
* @param[in]  {value} Data contents of dp 
* @return send result 
*/
unsigned char mcu_dp_bool_update(unsigned char dpid,unsigned char value);


/**
* @brief report enum type DP data to zigbee module 
* @param[in]  {dpid}  dp id
* @param[in]  {value} Data contents of dp 
* @return send result 
*/
unsigned char mcu_dp_enum_update(unsigned char dpid,unsigned char value);


/**
* @brief report bitmap type DP data to zigbee module 
* @param[in]  {dpid}  dp id
* @param[in]  {value} Data contents of dp 
* @return send result 
*/
unsigned char mcu_dp_bitmap_update(unsigned char dpid,unsigned long value);

/**
* @brief report fault type DP data to zigbee module 
* @param[in]  {dpid}  dp id
* @param[in]  {value} Data contents of dp 
* @return send result 
*/
unsigned char mcu_dp_fault_update(unsigned char dpid,unsigned long value);

/**
* @brief report raw type DP data to zigbee module 
* @param[in]  {dpid}  dp id
* @param[in]  {value} Data contents of dp 
* @param[in]  {len} length of Data contents  
* @return send result 
*/
unsigned char mcu_dp_raw_update(unsigned char dpid,const unsigned char value[],unsigned short len);

/**
* @brief report string type DP data to zigbee module 
* @param[in]  {dpid}  dp id
* @param[in]  {value} Data contents of dp 
* @param[in]  {len} length of Data contents  
* @return send result 
*/
unsigned char mcu_dp_string_update(unsigned char dpid,const unsigned char value[],unsigned short len);

/**
* @brief report raw type DP data to zigbee module 
* @param[in]  {dpid}  dp id
* @param[in]  {value} Data contents of dp 
* @return send result 
*/
unsigned char mcu_dp_value_update(unsigned char dpid,unsigned long value);

/**
* @brief mcu get bool type value from zigbee translate 
* @param[in]  {value}  data of dp
* @param[in]  {len} Data length
* @return  true or false
*/
unsigned char mcu_get_dp_download_bool(const unsigned char value[],unsigned short len);

/**
* @brief mcu get enum type value from zigbee translate 
* @param[in]  {value}  data of dp
* @param[in]  {len} Data length
* @return  enum data
*/
unsigned char mcu_get_dp_download_enum(const unsigned char value[],unsigned short len);

/**
* @brief mcu get value type value from zigbee translate 
* @param[in]  {value}  data of dp
* @param[in]  {len} Data length
* @return  value data
*/
unsigned long mcu_get_dp_download_value(const unsigned char value[],unsigned short len);

/**
* @brief mcu reset zigbee module 
* @param[in]  {void} 
* @return void
*/
void mcu_reset_zigbee(void);

/**
* @brief mcu start zigbee module 
* @param[in]  {void} 
* @return void
*/
void mcu_network_start(void);


/**
* @brief mcu start zigbee module test 
* @param[in]  {channel} test channel usually 11
* @return void
*/
void mcu_start_zigbee_test(unsigned char channel);

/**
* @brief copy receive data from uart receive interrupt
* @param[in]  {value} Data received from interrupt
* @return void 
*/
void uart_receive_input(unsigned char value);

/**
* @brief init paramter and set rx_buf with zero
* @param[in]  {void}
* @return void 
*/
void zigbee_protocol_init(void);

/**
 * @brief  check rx buffer is empty or not
 * @param  Null
 * @return the queue state
 */
unsigned char with_data_rxbuff(void);

/**
* @brief read byte from zigbee_queue_buf array
* @param[in]  {void}
* @return read byte value
*/
unsigned char Queue_Read_Byte(void);

/**
* @brief uart receive data handle, call this function at mian loop 
* @param[in]  {void}
* @return void 
*/
void zigbee_uart_service(void);

#ifdef __cplusplus
}
#endif
#endif
