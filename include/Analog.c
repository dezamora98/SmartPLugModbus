#include <avr/interrupt.h>
#include "ModbusSlave/Modbus.h"
#include "Analog.h"
#include "GPIO.h"

ISR(ADC_vect)
{
    /*saving the channel reading in the corresponding register in MODBUS.*/
    InputReg.AnalogReg[Analog_Iterator] = ADC;

    if (++Analog_Iterator == sizeof(InputReg.AnalogReg) / sizeof(uint16_t))
    {
        Analog_Iterator = 0;
    }

    /*prepares the ADC for the next conversion, if the next channel
     is from the temperature sensor the reference must be set to 1.1V
     otherwise AREF is still used.*/
    if (Analog_Chanel[Analog_Iterator] == 8)
    {
        ADMUX = (1 << REFS0) | (1 << REFS1) | Analog_Chanel[Analog_Iterator];
    }
    else
    {
        /**
         * @brief using a 5v or 3.3v reference greatly affects the accuracy of
         * the adc when changing the reference for the integrated temperature sensor.
         */
#ifdef INT_REF_11
        ADMUX = (1 << REFS0) | (1 << REFS1) | Analog_Chanel[Analog_Iterator];
#else
        ADMUX = (1 << REFS0) | Analog_Chanel[Analog_Iterator];
#endif // !INT_REF_11
    }

    /* start new conversion */
    ADCSRA |= (1 << ADSC);
}

void AnalogInit(void)
{
    /* Disable the digital inputs to reduce power
       consumption and noise */
    DIDR0 = 0xff;

    Analog_Iterator = 0;
    ADMUX = (1 << REFS0)  | Analog_Chanel[Analog_Iterator];

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
    sei();
}

void AnalogCheck(void)
{
    // checking voltage level (OverVoltage)
    if (InputReg.PlugVoltage > HoldingReg.OverVoltage)
    {
        /*include system timeout protect*/
        /*-> here <-*/
        /* for testing, the timeout protection will be ignored */
        RELAY_PORT = ALL_OFF;
        FSM_State = ST_Protect_OverVoltage;
        return;
    }

    // checking voltage level (LowVoltage)
    if (InputReg.PlugVoltage < HoldingReg.LowVoltage)
    {
        /*include system timeout protect*/
        /*-> here <-*/
        /* for testing, the timeout protection will be ignored */
        RELAY_PORT = ALL_OFF;
        FSM_State = ST_Protect_LowVoltage;
        return;
    }

    // checking CPU temperature
    if (InputReg.TempMCU > HoldingReg.HighTemperature)
    {
        /*include system timeout protect*/
        /*-> here <-*/
        /* for testing, the timeout protection will be ignored */
        RELAY_PORT = ALL_OFF;
        FSM_State = ST_Protect_CriticalTem;
        return;
    }

    // checking of current level on main board
    if (InputReg.BoardCurrent > HoldingReg.SystemOverCurrent)
    {
        /*include system timeout protect*/
        /*-> here <-*/
        /* for testing, the timeout protection will be ignored */
        RELAY_PORT = ALL_OFF;
        FSM_State = ST_Protect_SystemOverCurrent;
        return;
    }

    // checking the current in the plugs
    for (uint8_t i = 0; i < sizeof(InputReg.PlugCurrent) / sizeof(uint16_t); ++i)
    {
        if (InputReg.PlugCurrent[i] >= HoldingReg.PlugOverCurrent)
        {
            /*include system timeout protect*/
            /*-> here <-*/
            /* for testing, the timeout protection will be ignored */
            RELAY_OFF(i);
            InputReg.PlugState[i] = st_OverCurrent;
            Coil.Array[0] &= ~(1 << (i + ADDR_Plug_0));
        }
        else if (InputReg.PlugCurrent[i] <= HoldingReg.PlugLowCurrent)
        {
            /*include system timeout protect*/
            /*-> here <-*/
            /* for testing, the timeout protection will be ignored */
            RELAY_OFF(i);
            InputReg.PlugState[i] = st_LowCurrent;
            Coil.Array[0] &= ~(1 << (i + ADDR_Plug_0));
        }
    }
}