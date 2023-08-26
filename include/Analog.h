#ifndef ANALOG_H
#define ANALOG_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include "ModbusAddr.h"

#define INIT_AN_REG Reg_PlugVoltage
static const uint8_t Analog_Chanel[] = {5, 4, 0, 1, 2, 3, 6, 7, 8}; // defines the ordered path of ADC readings
static volatile uint8_t Analog_Iterator = 0;                        // analog channel iterator
#define size_ACH sizeof(Analog_Chanel) / sizeof(Analog_Chanel[0])   // number of analog channels
#define getAddrReg_ACH(i) INIT_AN_REG + i                           // get analog channel register address

ISR(ADC_vect)
{
    /*saving the channel reading in the corresponding register in MODBUS.*/
    InputReg[getAddrReg_ACH(Analog_Iterator)] = ADC;

    if (++Analog_Iterator == size_ACH)
    {
        Analog_Iterator = 0;
    }

    /*prepares the ADC for the next conversion, if the next channel
     is from the temperature sensor the reference must be set to 1.1V
     otherwise AREF is still used.*/
    if ((INIT_AN_REG + Analog_Iterator) == Reg_TempMCU)
    {
        ADMUX = 1 << REFS0 | 1 << REFS1 | Analog_Chanel[Analog_Iterator];
    }
    else
    {
        ADMUX = 1 << REFS0 | Analog_Chanel[Analog_Iterator];
    }

    /* start new conversion */
    ADCSRA |= 1 << ADSC;
}

/**
 * @brief Initializes analog tasks:
 *
 * This function sets the ADC to 125kHz according
 * to the CPU working frequency (F_CPU). This guarantees
 * an ADC response time of approximately 100uS; therefore,
 * if 10 consecutive channels are polled, an update time of
 * approximately 1ms is achieved.
 *
 **/

inline void AnalogInit(void)
{
    /* Disable the digital inputs to reduce power
       consumption and noise */
    DIDR0 = 0xff;

    Analog_Iterator = 0;
    ADMUX = 1 << REFS0 | Analog_Chanel[Analog_Iterator];

/* Set the ADC prescaler for 125 kHz */
#if F_CPU == 16000000UL
    ADCSRA = 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0 |
             1 << ADEN | 1 << ADIE;
#elif F_CPU >= 8000000UL
    ADCSRA = 1 << ADPS2 | 1 << ADPS1 | 1 << ADEN | 1 << ADIE;
#elif F_CPU == 4000000UL
    ADCSRA = 1 << ADPS2 | 1 << ADEN | 1 << ADIE;
#elif F_CPU == 2000000UL
    ADCSRA = 1 << ADPS1 | 1 << ADPS0 | 1 << ADEN | 1 << ADIE;
#elif F_CPU == 1000000UL
    ADCSRA = 1 << ADPS1 | 1 << ADEN | 1 << ADIE;
#else
    ADCSRA = 1 << ADEN | 1 << ADIE;
#endif

    /* start init conversion */
    ADCSRA |= 1 << ADSC;
}



#endif // !ANALOG_H
