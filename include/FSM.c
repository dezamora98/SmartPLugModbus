#include <stdbool.h>
#include <avr/wdt.h>
#include "FSM.h"
#include "Analog.h"
#include "GPIO.h"
#include "ModbusSlave/Modbus.h"
#include "EEPROM_Param.h"
#include <string.h>

void FSM_init(void)
{
    while (true)
    {
        switch (FSM_State)
        {
        case ST_Standby:
            // AnalogCheck();
            GPIOUpdate();

            if ((Coil.Array[0] & (1 << (ADDR_Reset))) != 0)
            {
                Coil.Array[0] = 0;
                if (!ParamCheck())
                {
                    UpdatePersistentParam();
                }
                wdt_enable(WDTO_15MS);
                while (true)
                    ;
            }
            break;

        case ST_ModbusPull:
            ModbusPoll();
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
