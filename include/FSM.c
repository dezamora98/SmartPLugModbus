#include <stdbool.h>
#include <avr/wdt.h>
#include "FSM.h"
#include "Analog.h"
#include "GPIO.h"
#include "ModbusSlave/Modbus.h"
#include "EEPROM_Param.h"
#include <string.h>

inline void ResetCheck(void)
{
    if ((Coil.Array[InitAddr_Coil] & (1 << (ADDR_Reset))) != 0)
    {
        Coil.Array[InitAddr_Coil] = 0;
        if (!ParamCheck())
        {
            UpdatePersistentParam();
        }
        wdt_enable(WDTO_15MS);
        while (true)
            ;
    }
}

void setProtect(state st)
{
    RELAY_PORT = ALL_OFF;
    memset(InputReg.PlugState, st, sizeof(InputReg.PlugState));
    InputReg.SystemState = st;
}


void FSM_init(void)
{
    while (true)
    {
        switch (FSM_State)
        {
        case ST_Standby:
            GPIOUpdate();
            AnalogCheck();
            break;

        case ST_ModbusPull:
            ModbusPoll();
            break;

        case ST_Protect_OverVoltage:
            setProtect(st_OverVoltage);
            break;

        case ST_Protect_LowVoltage:
            setProtect(st_LowVoltage);
            break;

        case ST_Protect_CriticalTem:
            setProtect(st_HighTemterature);
            break;

        case ST_Protect_SystemOverCurrent:
            setProtect(st_SystemOverCurrent);
            break;

        default:
            setProtect(st_UndefinedError);
            break;
        }
        ResetCheck();
    }
}
