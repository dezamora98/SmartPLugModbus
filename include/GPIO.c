#include "GPIO.h"
#include "ModbusSlave/Modbus.h"

inline void GPIOInit(void)
{
    RELAY_PORT = ALL_OFF;     // relay init OFF

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
    RELAY_PORT = (Coil.Array[0]<<ADDR_Plug_0) & ((1<<ADDR_Reset)-1);
}