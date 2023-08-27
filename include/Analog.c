#include <avr/interrupt.h>
#include "Modbus.h"
#include "Analog.h"
#include "GPIO.h"

ISR(ADC_vect)
{
    /*saving the channel reading in the corresponding register in MODBUS.*/
    ArrayAnalogReg[Analog_Iterator] = ADC;

    if (++Analog_Iterator == size_ACH)
    {
        Analog_Iterator = 0;
    }

    /*prepares the ADC for the next conversion, if the next channel
     is from the temperature sensor the reference must be set to 1.1V
     otherwise AREF is still used.*/
    if ((Reg_PlugVoltage + Analog_Iterator) == Reg_TempMCU)
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

void AnalogInit(void)
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

void AnalogCheck(void)
{
    // checking voltage level (OverVoltage)
    if (PlugVoltage > OverVoltage)
    {
        /*include system timeout protect*/
        /*-> here <-*/
        /* for testing, the timeout protection will be ignored */
        RELAY_PORT = ALL_OFF;
        FSM_State = ST_Protect_OverVoltage;
        return;
    }

    // checking voltage level (LowVoltage)
    if (PlugVoltage < LowVoltage)
    {
        /*include system timeout protect*/
        /*-> here <-*/
        /* for testing, the timeout protection will be ignored */
        RELAY_PORT = ALL_OFF;
        FSM_State = ST_Protect_LowVoltage;
        return;
    }

    // checking CPU temperature
    if (TempMCU > HighTemperature)
    {
        /*include system timeout protect*/
        /*-> here <-*/
        /* for testing, the timeout protection will be ignored */
        RELAY_PORT = ALL_OFF;
        FSM_State = ST_Protect_CriticalTem;
        return;
    }

    // checking of current level on main board
    if (BoardCurrent > SystemOverCurrent)
    {
        /*include system timeout protect*/
        /*-> here <-*/
        /* for testing, the timeout protection will be ignored */
        RELAY_PORT = ALL_OFF;
        FSM_State = ST_Protect_SystemOverCurrent;
        return;
    }

    // checking the current in the plugs
    for (uint8_t i = 0; i < size_ArrayCurrentPlug; ++i)
    {
        if (ArrayCurrentPlug[i] >= PlugOverCurrent)
        {
            /*include system timeout protect*/
            /*-> here <-*/
            /* for testing, the timeout protection will be ignored */
            RELAY_OFF(i);
            ArrayPlugState[i] = st_OverCurrent;
            Coil &= ~(1 << (i + Plug_0));
        }
        else if (ArrayCurrentPlug[i] <= PlugLowCurrent)
        {
            /*include system timeout protect*/
            /*-> here <-*/
            /* for testing, the timeout protection will be ignored */
            RELAY_OFF(i);
            ArrayPlugState[i] = st_LowCurrent;
            Coil &= ~(1 << (i + Plug_0));
        }
    }
}