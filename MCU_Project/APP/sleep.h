/**
* @file  sleep.h
* @author hequan
* @date 2021.01.29
* http://www.hqshome.cn
*/
#ifndef SLEEP_H
#define SLEEP_H

#include "main.h"

typedef enum
{
    ONLY_EXIT,
    EXIT_RTC
} SLEEP_MODEL_T;


void Sleep_setClock(RTC_TimeTypeDef time, APP_T *data);
void Sleep_mcu();

#endif