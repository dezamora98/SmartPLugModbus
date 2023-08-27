#include <util/delay.h>
#include "FSM.h"
#include "mb.h"
#include "mbport.h"
#include "FSM.h"
#include "Modbus.h"
#include "Analog.h"
#include "GPIO.h"

void FSM_init(void)
{
    eMBEventType MB_eEvent;
    eMBErrorCode MB_ERROR;

    while (true)
    {
        switch (FSM_State)
        {
        case ST_Standby:
            AnalogCheck();
            RELAY_PORT = (Coil[0]<<Plug_0) & ((1<<Reset)-1);
            break;

        case ST_ModbusPull:
            MB_eEvent = EV_READY;
            do
            {
                MB_ERROR = eMBPoll(&MB_eEvent);
                _delay_us(1);
            } while (MB_ERROR == MB_ENOERR && MB_eEvent != EV_READY);
            FSM_State = FSM_LastState;
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
