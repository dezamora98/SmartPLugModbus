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


#include "FSM.h"
#include "ModbusSlave/Modbus.h"
#include "Analog.h"
#include "GPIO.h"
#include "EEPROM_Param.h"
#include <avr/wdt.h>


int main(void)
{
    wdt_disable();
    GPIOInit();
    AnalogInit();
    UpdateVolatileParam();
    eMBInit(MB_RTU, HoldingReg.SlaveID, 0, 38400, MB_PAR_NONE);
    eMBEnable();
    FSM_init();
}

