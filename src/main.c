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
#include "Modbus.h"
#include "Analog.h"
#include "GPIO.h"

/* ----------------------- Defines ------------------------------------------*/

/* ----------------------- Static variables ---------------------------------*/

/* --------------------- function prototypes ---------------------------------*/

/* ----------------------- Start implementation -----------------------------*/
int main(void)
{
    GPIOInit();
    AnalogInit();
    eMBInit(MB_RTU, SlaveID, 0, 38400, MB_PAR_NONE);
    eMBEnable();
    FSM_init();
}