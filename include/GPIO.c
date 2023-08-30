#include "GPIO.h"
#include "ModbusSlave/Modbus.h"

inline void GPIOInit(void)
{
    RELAY_PORT = ALL_OFF; // relay init OFF

    ANALOG_PORT_DDR = ALL_IN; // analog imput;
    RELAY_PORT_DDR = ALL_OUT; // ouput relay;

    // set up the output pins on the communication port
    COM_PORT = 1 << MODBUS_TX | 1 << MODBUS_RTS |
               1 << SOFT_MOSI | 1 << SOFT_SCK |
               1 << SOFT_SS0 | 1 << SOFT_SS1;
    // set up the output pins on the communication port
    COM_PORT_DDR = 1 << MODBUS_TX | 1 << MODBUS_RTS |
                   1 << SOFT_MOSI | 1 << SOFT_SCK |
                   1 << SOFT_SS0 | 1 << SOFT_SS1;
}

void GPIOUpdate(void)
{
    uint8_t coils = ~(Coil.Array[InitAddr_Coil]) & 0x3f;
    uint8_t relay = (RELAY_PORT & 0x3f);
    uint8_t i = 0;

    while (relay != coils)
    {
        if ((relay & (1 << i)) != (coils & (1 << i)))
        {
            if(coils & (1 << i))
            {
                if(InputReg.PlugState[i]==st_On)
                {
                    InputReg.PlugState[i] = st_Off;
                }
                relay |= (1<<i);
            }
            else
            {
                InputReg.PlugState[i] = st_On;
                relay &= ~(1<<i);
            }
        }
        ++i;
    }
    RELAY_PORT = relay;
}