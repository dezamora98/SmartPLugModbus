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
#include <avr/delay.h>

/* ------------------------------ includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "FSM.h"
#include "Analog.h"
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

        default:
            break;
        }
    }
}
