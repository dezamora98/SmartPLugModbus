/*
 * FreeModbus Libary: AVR Demo Application
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * Modfications Copyright (C) 2006 Michał.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: demo.c,v 1.7 2006/06/15 15:38:02 wolti Exp $
 */

/* ----------------------- AVR includes -------------------------------------*/
#include <avr/io.h>
#include <util/delay.h>

/* ------------------------------ includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "FSM.h"
#include "Analog.h"
#include "GPIO.h"
#include "ModbusAddr.h"

/* ----------------------- Defines ------------------------------------------*/

/* ----------------------- Static variables ---------------------------------*/

/* --------------------- function prototypes ---------------------------------*/
inline void FSM_init(void);

/* ----------------------- Start implementation -----------------------------*/
int main(void)
{

    eMBInit(MB_RTU, SlaveID, 0, 38400, MB_PAR_NONE);

    /* Enable the Modbus Protocol Stack. */
    eMBEnable();

    FSM_init();
}

inline void FSM_init(void)
{
    eMBEventType MB_eEvent;
    eMBErrorCode MB_ERROR;

    while (true)
    {
        switch (FSM_State)
        {
        case ST_Standby:
            for (uint8_t i = 0; i < size_ACH; ++i)
            {
                // checking voltage level
                if (getAddrReg_ACH(i) == Reg_PlugVoltage)
                {
                    if (InputReg[Reg_PlugVoltage] > HoldingReg[Param_OverVoltage])
                    {
                        FSM_State = ST_Protect_OverVoltage;
                        break;
                    }
                    else if (InputReg[Reg_BoardCurrent] > HoldingReg[Param_LowVoltage])
                    {
                        FSM_State = ST_Protect_LowVoltage;
                        break;
                    }
                }
                // checking the current in the plugs
                else if (getAddrReg_ACH(i) >= Reg_PlugCurrent_0 && getAddrReg_ACH(i) <= Reg_PlugCurrent_5)
                {
                    if (InputReg[getAddrReg_ACH(i)] >= HoldingReg[Param_PlugOverCurrent])
                    {
                        /*include system timeout protect*/
                        /*-> here <-*/
                        /* for testing, the timeout protection will be ignored */
                        uint8_t plug = Reg_PlugCurrent_0 - getAddrReg_ACH(i);
                        RELAY_OFF(plug);
                        InputReg[plug] = st_OverCurrent;
                        Coil[plug + 1] = false;
                    }
                    else if (InputReg[getAddrReg_ACH(i)] <= HoldingReg[Param_PlugLowCurrent])
                    {
                        /*include system timeout protect*/
                        /*-> here <-*/
                        /* for testing, the timeout protection will be ignored */
                        uint8_t plug = Reg_PlugCurrent_0 - getAddrReg_ACH(i);
                        RELAY_OFF(plug);
                        InputReg[plug] = st_LowCurrent;
                        Coil[plug + 1] = false;
                    }
                }
                // checking CPU temperature
                else if (getAddrReg_ACH(i) == Reg_PlugVoltage)
                {
                    if (InputReg[Reg_TempMCU] >= Param_HighTemperature)
                    {
                        FSM_State = ST_Protect_CriticalTem;
                        break;
                    }
                }
                // checking of current level on main board
                else if (getAddrReg_ACH(i) == Reg_BoardCurrent)
                {
                    if (InputReg[Reg_BoardCurrent] >= Param_SystemOverCurrent)
                    {
                        FSM_State = Param_SystemOverCurrent;
                        break;
                    }
                }
            }
            break;

        case ST_ModbusPull:
            do
            {
                MB_ERROR = eMBPoll(&MB_eEvent);
                _delay_us(10);
            } while (MB_ERROR == MB_ENOERR && MB_eEvent != EV_FRAME_SENT);
            MB_ERROR = MB_ENOERR;
            MB_eEvent = EV_READY;
            FSM_State = ST_Standby;
            break;

        case ST_Protect_OverVoltage:
            RELAY_PORT = ALL_OFF;
            break;

        case ST_Protect_LowVoltage:
            RELAY_PORT = ALL_OFF;
            break;

        case ST_Protect_CriticalTem:
            RELAY_PORT = ALL_OFF;
            break;
        
        case ST_Protect_SystemOverCurrent:
            RELAY_PORT = ALL_OFF;
            break;

        default:
            RELAY_PORT = ALL_OFF;
            /*System ERROR*/
            break;
        }
    }
}
