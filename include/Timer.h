#ifndef TIMER_H
#define TIMER_H

#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>
#include "ModbusSlave/Modbus.h"

typedef struct
{
    bool EN;
    uint16_t tic;
} t_tic;

typedef struct
{
    union 
    {
        t_tic Array[ADDR_Reg_TempMCU-ADDR_Reg_PlugCurrent_0];
        struct
        {
            t_tic Voltage;
            t_tic BoardCurrent;
            t_tic PlugCurrent[ADDR_Reg_PlugCurrent_5 - ADDR_Reg_PlugCurrent_0];
            t_tic TempMCU;
        };
    };
} t_SPMEvent;

typedef enum
{
    micro,
    mili,
    seg
} t_timeType;

void TimerInit();

void TimerTic(t_tic *event, uint16_t timeOut, t_timeType type);
bool TimerToc(t_tic *event, uint16_t timeOut, t_timeType type);


void CheckTimerEvent(t_SPMEvent *SPM);

#define TimerEnable() TIMSK0 = (1 << OCIE0A)
#define TimerDisable() TIMSK0 = 0

#endif // !TIMER_H