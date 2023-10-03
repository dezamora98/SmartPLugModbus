/**
 * @file main.c
 * @author Daniel Enrique Zamora Sifredo (dezamora@gmail.com)
 * @brief SmartPlugModbus firmware main file
 * @version 0.1
 * @date 2023-08-27
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>

#include "FSM.h"
#include "ModbusSlave/Modbus.h"
#include "Analog.h"
#include "GPIO.h"
#include "EEPROM_Param.h"
#include "Timer.h"

int main(void)
{
    CLKPR = 00;
    wdt_disable();
    GPIOInit();
    AnalogInit();
    // TimerInit();
    UpdateVolatileParam();
    if (Check_ID())
    {
        HoldingReg.SlaveID = 1;
        UpdatePersistentParam();
    }

    eMBInit(MB_RTU, HoldingReg.SlaveID, 0, 38400, MB_PAR_NONE);
    eMBEnable();
    FSM_init();
}
