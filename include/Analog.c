#include "Analog.h"

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
    uint8_t plug;

    for (uint8_t i = 0; i < size_ACH; ++i)
    {
        // checking voltage level and current in system
        if (getAddrReg_ACH(i) == Reg_PlugVoltage)
        {
            if (InputReg[Reg_PlugVoltage] > HoldingReg[Param_OverVoltage])
            {
                FSM_State = ST_Protect_OverVoltage;
                break;
            }
            else if (InputReg[Reg_BoardCurrent] > HoldingReg[Param_LowVoltage])
            {
                FSM_State = ST_Protect_LowVoltage;
                break;
            }
        }
        // checking the current in the plugs
        else if (getAddrReg_ACH(i) >= Reg_PlugCurrent_0 && getAddrReg_ACH(i) <= Reg_PlugCurrent_5)
        {
            if (InputReg[getAddrReg_ACH(i)] >= HoldingReg[Param_PlugOverCurrent])
            {
                /*include system timeout protect*/
                /*-> here <-*/
                /* for testing, the timeout protection will be ignored */
                plug = Reg_PlugCurrent_0 - getAddrReg_ACH(i);
                RELAY_OFF(plug);
                InputReg[plug] = st_OverCurrent;
                Coil[0] &= ~(1 << (plug + Plug_0));
            }
            else if (InputReg[getAddrReg_ACH(i)] <= HoldingReg[Param_PlugLowCurrent])
            {
                /*include system timeout protect*/
                /*-> here <-*/
                /* for testing, the timeout protection will be ignored */
                plug = Reg_PlugCurrent_0 - getAddrReg_ACH(i);
                RELAY_OFF(plug);
                InputReg[plug] = st_LowCurrent;
                Coil[0] &= ~(1 << (plug + Plug_0));
            }
        }
        // checking CPU temperature
        else if (getAddrReg_ACH(i) == Reg_PlugVoltage)
        {
            if (InputReg[Reg_TempMCU] >= Param_HighTemperature)
            {
                FSM_State = ST_Protect_CriticalTem;
                break;
            }
        }
        // checking of current level on main board
        else if (getAddrReg_ACH(i) == Reg_BoardCurrent)
        {
            if (InputReg[Reg_BoardCurrent] >= Param_SystemOverCurrent)
            {
                FSM_State = ST_Protect_SystemOverCurrent;
                break;
            }
        }
    }
}