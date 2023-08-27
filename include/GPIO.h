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
 * @brief this function initializes the system GPIOs
 **/
void GPIOInit(void);

/**
 * @brief this function consists of updating the relay 
 * outputs according to the values of the coils.
 **/
void GPIOUpdate(void);
#endif // !GPIO_H