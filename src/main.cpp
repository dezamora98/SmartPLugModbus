#include <avr/io.h>
#include "ModbusAddr.h"


int main(void)
{
    DDRB |= 0xFF;
    DDRC |= 0xFF;
    eMDStatus = eMBInit(MB_RTU,SlaveID,0,9600,MB_PAR_NONE);
    eMDStatus = eMBEnable();

    for(;;)
    {
        eMBPoll();
    }
    return 0;
}
