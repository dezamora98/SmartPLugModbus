#ifndef GPIO_H
#define GPIO_H

#include <avr/io.h>

#define MODBUS_RX PD0
#define MODBUS_TX PD1
#define MODBUS_RTS PD2
#define SOFT_MOSI PD3
#define SOFT_MISO PD4
#define SOFT_SCK PD5
#define SOFT_SS0 PD6
#define SOFT_SS1 PD7

#define RELAY_PORT PORTB
#define RELAY_PORT_DDR DDRB
#define ANALOG_PORT PORTC
#define ANALOG_PORT_DDR DDRC
#define COM_PORT PORTD
#define COM_PORT_DDR DDRD
#define ALL_OFF 0xff
#define ALL_ON 0x00
#define ALL_IN 0x00
#define ALL_OUT 0xff
#define RELAY_ON(n) RELAY_PORT &= ~(1 << n)
#define RELAY_OFF(n) RELAY_PORT |= 1 << n

/**
 * @brief GPIO configuration
 *
 **/
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

#endif // !GPIO_H