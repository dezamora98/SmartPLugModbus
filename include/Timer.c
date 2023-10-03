#include "Timer.h"
#include <avr/interrupt.h>

volatile static uint16_t vMicT;
volatile static uint16_t vMic;
volatile static uint16_t vMilT;
volatile static uint16_t vMil;
volatile static uint16_t vSegT;

ISR(TIMER0_COMPA_vect)
{
    vMicT += 100;
    vMic += 100;
    if (++vMic == 1000)
    {
        ++vMilT;
        ++vMil;
        vMic = 0;
    }
    if (++vMil == 1000)
    {
        ++vSegT;
    }
}

void TimerInit(void)
{

    // Configura el temporizador para que se desborde cada 100us
    TCCR0A |= (1 << WGM01);  // Modo CTC
    TCCR0B |= (1 << CS01);   // Preescalador 8
    OCR0A = 99;              // Valor de comparación para obtener una interrupción cada 100us
    TIMSK0 |= (1 << OCIE0A); // Habilita la interrupción de comparación A
}

void TimerTic(t_tic *event, uint16_t timeOut, t_timeType type)
{
    event->EN = true;
    switch ((uint8_t)type)
    {
    case micro:
        event->tic = vMicT + timeOut;
        break;
    case mili:
        event->tic = vMilT + timeOut;
        break;
    case seg:
        event->tic = vSegT + timeOut;
    }
    // enable comparator A interrupt
    TimerEnable();
}

bool TimerToc(t_tic *event, uint16_t timeOut, t_timeType type)
{
    switch ((uint8_t)type)
    {
    case micro:
        if (event->tic <= vMic)
            return false;
    case mili:
        if (event->tic <= vMilT)
            return false;
    case seg:
        if (event->tic <= vSegT)
            return false;
    }
    return true;
}

void CheckTimerEvent(t_SPMEvent *SPM)
{
    for (uint8_t i = 0; i < sizeof_array(SPM->Array); ++i)
    {
        if (SPM->Array[i].EN)
            return;
    }
    TimerDisable();
}