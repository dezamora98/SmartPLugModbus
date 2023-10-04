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
    for (uint8_t i = InitAddr_Coil; i <= ADDR_Plug_5; ++i)
    {
        if ((Coil.Array[InitAddr_Coil] & (1 << i)) != 0)
        {
            if (InputReg.PlugState[i] == st_Off)
            {
                InputReg.PlugState[i] = st_On;
                RELAY_ON(i);
            }
        }
        else
        {
            if (InputReg.PlugState[i] == st_On)
            {
                InputReg.PlugState[i] = st_Off;
            }
            RELAY_OFF(i);
        }
    }
}