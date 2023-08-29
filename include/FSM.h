#ifndef FSM_H
#define FSM_H

#include <avr/io.h>

typedef enum
{
    ST_Standby = 0,
    ST_ModbusPull,
    ST_Protect_CriticalTem,
    ST_Protect_OverVoltage,
    ST_Protect_LowVoltage,
    ST_Protect_SystemOverCurrent
}state;

volatile state FSM_State;
volatile state FSM_LastState;

void ResetCheck(void);
void setProtect(state st);
void FSM_init(void);

#endif