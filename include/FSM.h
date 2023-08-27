#ifndef FSM_H
#define FSM_H

#include <avr/io.h>

enum
{
    ST_Standby = 0,
    ST_ModbusPull,
    ST_Protect_CriticalTem,
    ST_Protect_OverVoltage,
    ST_Protect_LowVoltage,
    ST_Protect_SystemOverCurrent
};

volatile uint8_t FSM_State;
volatile uint8_t FSM_LastState;

void FSM_init(void);

#endif