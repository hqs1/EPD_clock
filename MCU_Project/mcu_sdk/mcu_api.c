/**
* @file  mcu_api.c
* @brief this file contains basic fuctions and DP data report fucntion which is base on DP type
* @author luchao
* @date 2020.03.13
* @par email:
* luchao@tuya.com
* @copyright HANGZHOU TUYA INFORMATION TECHNOLOGY CO.,LTD
* @par company
* http://www.tuya.com
*/

#include "zigbee.h"

/**
* @brief hex translate to bcd 
* @param[in] {Value_H} higher bits data 
* @param[in] {Value_L} lower bits data
* @return  bcd type data
*/
unsigned char hex_to_bcd(unsigned char Value_H, unsigned char Value_L)
{
  unsigned char bcd_value;

  if ((Value_H >= '0') && (Value_H <= '9'))
  {
    Value_H -= '0';
  }
  else if ((Value_H >= 'A') && (Value_H <= 'F'))
  {
    Value_H = Value_H - 'A' + 10;
  }
  else if ((Value_H >= 'a') && (Value_H <= 'f'))
  {
    Value_H = Value_H - 'a' + 10;
  }

  bcd_value = Value_H & 0x0f;
  bcd_value <<= 4;

  if ((Value_L >= '0') && (Value_L <= '9'))
  {
    Value_L -= '0';
  }
  else if ((Value_L >= 'A') && (Value_L <= 'F'))
  {
    Value_L = Value_L - 'a' + 10;
  }
  else if ((Value_L >= 'a') && (Value_L <= 'f'))
  {
    Value_L = Value_L - 'a' + 10;
  }

  bcd_value |= Value_L & 0x0f;

  return bcd_value;
}

/**
* @brief get string len 
* @param[in] {str} higher bits data 
* @return string len 
*/
unsigned long my_strlen(unsigned char *str)
{
  unsigned long len = 0;
  if (str == NULL)
  {
    return NULL;
  }

  for (len = 0; *str++ != '\0';)
  {
    len++;
  }

  return len;
}

/**
* @brief  assign ch to the first count bytes of the memory address src
* @param[in] {src}    srouce address
* @param[in] {ch}     set value 
* @param[in] {count}  length of set address 
* @return void
*/
void *my_memset(void *src, unsigned char ch, unsigned short count)
{
  unsigned char *tmp = (unsigned char *)src;
  if (src == NULL)
  {
    return NULL;
  }

  while (count--)
  {
    *tmp++ = ch;
  }
  return src;
}

/**
* @brief copy count bytes data from src to dest
* @param[in] {src}    srouce address
* @param[in] {dest}   destination address
* @param[in] {count}  length of copy data
* @return void
*/
void *my_memcpy(void *dest, const void *src, unsigned short count)
{
  unsigned char *pdest = (unsigned char *)dest;
  const unsigned char *psrc = (const unsigned char *)src;
  unsigned short i;

  if (dest == NULL || src == NULL)
  {
    return NULL;
  }

  if ((pdest <= psrc) || (pdest > psrc + count))
  {
    for (i = 0; i < count; i++)
    {
      pdest[i] = psrc[i];
    }
  }
  else
  {
    for (i = count; i > 0; i--)
    {
      pdest[i - 1] = psrc[i - 1];
    }
  }

  return dest;
}

/**
* @brief copy string src to string dest
* @param[in] {src}    srouce address
* @param[in] {dest}   destination address
* @return the tail of destination 
*/
char *my_strcpy(char *dest, const char *src)
{
  char *p = dest;
  while (*src != '\0')
  {
    *dest++ = *src++;
  }
  *dest = '\0';
  return p;
}

/**
* @brief compare string s1 with string s2
* @param[in] {s1}  srouce address
* @param[in] {s2}  destination address
* @return compare result
*/
int my_strcmp(char *s1, char *s2)
{
  while (*s1 && *s2 && *s1 == *s2)
  {
    s1++;
    s2++;
  }
  return *s1 - *s2;
}

/**
* @brief int translate to byte
* @param[in] {number} int data 
* @param[out] {value} the result array     
* @return void
*/
void int_to_byte(unsigned long number, unsigned char value[4])
{
  value[0] = number >> 24;
  value[1] = number >> 16;
  value[2] = number >> 8;
  value[3] = number & 0xff;
}

/**
* @brief byte data translate to int 
* @param[in] {value}  the byte array 
* @return result of int data 
*/
unsigned long byte_to_int(const unsigned char value[4])
{
  unsigned long nubmer = 0;

  nubmer = (unsigned long)value[0];
  nubmer <<= 8;
  nubmer |= (unsigned long)value[1];
  nubmer <<= 8;
  nubmer |= (unsigned long)value[2];
  nubmer <<= 8;
  nubmer |= (unsigned long)value[3];

  return nubmer;
}

/**
* @brief report bool type DP data to zigbee module 
* @param[in]  {dpid}  dp id
* @param[in]  {value} Data contents of dp 
* @return send result 
*/
unsigned char mcu_dp_bool_update(unsigned char dpid, unsigned char value)
{
  unsigned short length = 0;

  length = set_zigbee_uart_byte(length, dpid);
  length = set_zigbee_uart_byte(length, DP_TYPE_BOOL);

  length = set_zigbee_uart_byte(length, 0);
  length = set_zigbee_uart_byte(length, 1);

  if (value == FALSE)
  {
    length = set_zigbee_uart_byte(length, FALSE);
  }
  else
  {
    length = set_zigbee_uart_byte(length, 1);
  }

  zigbee_uart_write_frame(DATA_REPORT_CMD, length);

  return SUCCESS;
}

/**
* @brief report enum type DP data to zigbee module 
* @param[in]  {dpid}  dp id
* @param[in]  {value} Data contents of dp 
* @return send result 
*/
unsigned char mcu_dp_enum_update(unsigned char dpid, unsigned char value)
{
  unsigned short length = 0;

  length = set_zigbee_uart_byte(length, dpid);
  length = set_zigbee_uart_byte(length, DP_TYPE_ENUM);
  //
  length = set_zigbee_uart_byte(length, 0);
  length = set_zigbee_uart_byte(length, 1);
  //
  length = set_zigbee_uart_byte(length, value);

  zigbee_uart_write_frame(DATA_REPORT_CMD, length);

  return SUCCESS;
}

/**
* @brief report bitmap type DP data to zigbee module 
* @param[in]  {dpid}  dp id
* @param[in]  {value} Data contents of dp 
* @return send result 
*/
unsigned char mcu_dp_bitmap_update(unsigned char dpid, unsigned long value)
{
  unsigned short length = 0;

  length = set_zigbee_uart_byte(length, dpid);
  length = set_zigbee_uart_byte(length, DP_TYPE_BITMAP);
  //
  length = set_zigbee_uart_byte(length, 0);

  if ((value | 0xff) == 0xff)
  {
    length = set_zigbee_uart_byte(length, 1);
    length = set_zigbee_uart_byte(length, value);
  }
  else if ((value | 0xffff) == 0xffff)
  {
    length = set_zigbee_uart_byte(length, 2);
    length = set_zigbee_uart_byte(length, value >> 8);
    length = set_zigbee_uart_byte(length, value & 0xff);
  }
  else
  {
    length = set_zigbee_uart_byte(length, 4);
    length = set_zigbee_uart_byte(length, value >> 24);
    length = set_zigbee_uart_byte(length, value >> 16);
    length = set_zigbee_uart_byte(length, value >> 8);
    length = set_zigbee_uart_byte(length, value & 0xff);
  }

  zigbee_uart_write_frame(DATA_REPORT_CMD, length);

  return SUCCESS;
}

/**
* @brief report fault type DP data to zigbee module 
* @param[in]  {dpid}  dp id
* @param[in]  {value} Data contents of dp 
* @return send result 
*/
unsigned char mcu_dp_fault_update(unsigned char dpid, unsigned long value)
{
  unsigned short length = 0;

  length = set_zigbee_uart_byte(length, dpid);
  length = set_zigbee_uart_byte(length, DP_TYPE_BITMAP);
  //
  length = set_zigbee_uart_byte(length, 0);

  if ((value | 0xff) == 0xff)
  {
    length = set_zigbee_uart_byte(length, 1);
    length = set_zigbee_uart_byte(length, value);
  }
  else if ((value | 0xffff) == 0xffff)
  {
    length = set_zigbee_uart_byte(length, 2);
    length = set_zigbee_uart_byte(length, value >> 8);
    length = set_zigbee_uart_byte(length, value & 0xff);
  }
  else
  {
    length = set_zigbee_uart_byte(length, 4);
    length = set_zigbee_uart_byte(length, value >> 24);
    length = set_zigbee_uart_byte(length, value >> 16);
    length = set_zigbee_uart_byte(length, value >> 8);
    length = set_zigbee_uart_byte(length, value & 0xff);
  }

  zigbee_uart_write_frame(DATA_REPORT_CMD, length);

  return SUCCESS;
}

/**
* @brief report raw type DP data to zigbee module 
* @param[in]  {dpid}  dp id
* @param[in]  {value} Data contents of dp 
* @param[in]  {len} length of Data contents  
* @return send result 
*/
unsigned char mcu_dp_raw_update(unsigned char dpid, const unsigned char value[], unsigned short len)
{
  unsigned short length = 0;

  length = set_zigbee_uart_byte(length, dpid);
  length = set_zigbee_uart_byte(length, DP_TYPE_RAW);
  //
  length = set_zigbee_uart_byte(length, len / 0x100);
  length = set_zigbee_uart_byte(length, len % 0x100);
  //
  length = set_zigbee_uart_buffer(length, (unsigned char *)value, len);

  zigbee_uart_write_frame(DATA_REPORT_CMD, length);

  return SUCCESS;
}

/**
* @brief report string type DP data to zigbee module 
* @param[in]  {dpid}  dp id
* @param[in]  {value} Data contents of dp 
* @param[in]  {len} length of Data contents  
* @return send result 
*/
unsigned char mcu_dp_string_update(unsigned char dpid, const unsigned char value[], unsigned short len)
{
  unsigned short length = 0;

  length = set_zigbee_uart_byte(length, dpid);
  length = set_zigbee_uart_byte(length, DP_TYPE_STRING);
  //
  length = set_zigbee_uart_byte(length, len / 0x100);
  length = set_zigbee_uart_byte(length, len % 0x100);
  //
  length = set_zigbee_uart_buffer(length, (unsigned char *)value, len);

  zigbee_uart_write_frame(DATA_REPORT_CMD, length);

  return SUCCESS;
}

/**
* @brief report raw type DP data to zigbee module 
* @param[in]  {dpid}  dp id
* @param[in]  {value} Data contents of dp 
* @return send result 
*/
unsigned char mcu_dp_value_update(unsigned char dpid, unsigned long value)
{
  unsigned short length = 0;

  length = set_zigbee_uart_byte(length, dpid);
  length = set_zigbee_uart_byte(length, DP_TYPE_VALUE);
  //
  length = set_zigbee_uart_byte(length, 0);
  length = set_zigbee_uart_byte(length, 4);
  //
  length = set_zigbee_uart_byte(length, value >> 24);
  length = set_zigbee_uart_byte(length, value >> 16);
  length = set_zigbee_uart_byte(length, value >> 8);
  length = set_zigbee_uart_byte(length, value & 0xff);

  zigbee_uart_write_frame(DATA_REPORT_CMD, length);

  return SUCCESS;
}

/**
* @brief mcu get bool type value from zigbee translate 
* @param[in]  {value}  data of dp
* @param[in]  {len} Data length
* @return  true or false
*/
unsigned char mcu_get_dp_download_bool(const unsigned char value[], unsigned short len)
{
  return (value[0]);
}

/**
* @brief mcu get enum type value from zigbee translate 
* @param[in]  {value}  data of dp
* @param[in]  {len} Data length
* @return  enum data
*/
unsigned char mcu_get_dp_download_enum(const unsigned char value[], unsigned short len)
{
  return (value[0]);
}

/**
* @brief mcu get value type value from zigbee translate 
* @param[in]  {value}  data of dp
* @param[in]  {len} Data length
* @return  value data
*/
unsigned long mcu_get_dp_download_value(const unsigned char value[], unsigned short len)
{
  return (byte_to_int(value));
}

/**
* @brief mcu start zigbee module test 
* @param[in]  {channel} test channel usually 11
* @return void
*/
void mcu_start_zigbee_test(unsigned char channel)
{
  zigbee_uart_tx_buf[DATA_START + 7] = channel;
  zigbee_uart_write_frame(ZIGBEE_RF_TEST_CMD, 1);
}

/**
* @brief mcu reset zigbee module 
* @param[in]  {void} 
* @return void
*/
void mcu_reset_zigbee(void)
{
  unsigned short length = 0;

  length = set_zigbee_uart_byte(length, 0);
  zigbee_uart_write_frame(ZIGBEE_CFG_CMD, length);
}

/**
* @brief mcu start zigbee module 
* @param[in]  {void} 
* @return void
*/
void mcu_network_start(void)
{
  unsigned short length = 0;

  length = set_zigbee_uart_byte(length, 1);
  zigbee_uart_write_frame(ZIGBEE_CFG_CMD, length);
}

/**
* @brief copy receive data from uart receive interrupt
* @param[in]  {value} Data received from interrupt
* @return void 
*/
void uart_receive_input(unsigned char value)
{
  if (1 == queue_out - queue_in)
  {
    //串口接收缓存已满
  }
  else if ((queue_in > queue_out) && ((queue_in - queue_out) >= sizeof(zigbee_queue_buf)))
  {
    //串口接收缓存已满
  }
  else
  {
    //串口接收缓存未满
    if (queue_in >= (unsigned char *)(zigbee_queue_buf + sizeof(zigbee_queue_buf)))
    {
      queue_in = (unsigned char *)(zigbee_queue_buf);
    }
    *queue_in++ = value;
  }
}

/**
* @brief init paramter and set rx_buf with zero
* @param[in]  {void}
* @return void 
*/
void zigbee_protocol_init(void)
{
  queue_in = (unsigned char *)zigbee_queue_buf;
  queue_out = (unsigned char *)zigbee_queue_buf;
}

/**
* @brief read byte from zigbee_queue_buf array
* @param[in]  {void}
* @return read byte value
*/
unsigned char Queue_Read_Byte(void)
{
  unsigned char value;

  if (queue_out != queue_in)
  {
    //有数据
    if (queue_out >= (unsigned char *)(zigbee_queue_buf + sizeof(zigbee_queue_buf)))
    {
      //数据已经到末尾
      queue_out = (unsigned char *)(zigbee_queue_buf);
    }

    value = *queue_out++;
  }

  return value;
}

/**
 * @brief  check rx buffer is empty or not
 * @param  Null
 * @return the queue state
 */
unsigned char with_data_rxbuff(void)
{
  if (queue_out != queue_in)
    return 1;
  else
    return 0;
}

/**
* @brief uart receive data handle, call this function at mian loop 
* @param[in]  {void}
* @return void 
*/
void zigbee_uart_service(void)
{
  static unsigned short rx_in = 0;
  unsigned short offset = 0;
  unsigned short rx_value_len = 0;

  while ((rx_in < sizeof(zigbee_uart_rx_buf)) && (with_data_rxbuff() > 0))
  {
    zigbee_uart_rx_buf[rx_in++] = Queue_Read_Byte();
  }

  if (rx_in < PROTOCOL_HEAD)
    return;

  while ((rx_in - offset) >= PROTOCOL_HEAD)
  {
    if (zigbee_uart_rx_buf[offset + HEAD_FIRST] != FIRST_FRAME_HEAD)
    {
      offset++;
      continue;
    }

    if (zigbee_uart_rx_buf[offset + HEAD_SECOND] != SECOND_FRAME_HEAD)
    {
      offset++;
      continue;
    }

    if (zigbee_uart_rx_buf[offset + PROTOCOL_VERSION] != SERIAL_PROTOCOL_VER) /*version 2 is more than two byte  sequence number than version 1*/
    {
      offset += 2;
      continue;
    }

    rx_value_len = zigbee_uart_rx_buf[offset + LENGTH_HIGH] * 0x100;
    rx_value_len += (zigbee_uart_rx_buf[offset + LENGTH_LOW] + PROTOCOL_HEAD);
    if (rx_value_len > sizeof(zigbee_uart_rx_buf))
    {
      offset += 3;
      continue;
    }

    if ((rx_in - offset) < rx_value_len)
    {
      break;
    }

    if (get_check_sum((unsigned char *)zigbee_uart_rx_buf + offset, rx_value_len - 1) != zigbee_uart_rx_buf[offset + rx_value_len - 1])
    {
      offset += 3;
      continue;
    }

    data_handle(offset);
    offset += rx_value_len;
  }
  rx_in -= offset;
  if (rx_in > 0)
  {
    // if ((offset == 0) && (rx_in >= sizeof(zigbee_uart_rx_buf)))
    // {
    //   offset++;
    //   rx_in -= offset;
    // }
    my_memcpy((char *)zigbee_uart_rx_buf, (const char *)zigbee_uart_rx_buf + offset, rx_in);
  }
}