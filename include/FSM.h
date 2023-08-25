#ifndef FSM_H
#define FSM_H

#include <avr/io.h>

enum
{
    ST_Standby,
    ST_ModbusPull,
    ST_ModbusERROR,
    ST_CriticalTem,

};

/* ----------------------- Static variables ---------------------------------*/
volatile uint8_t FSM_State;

#endif