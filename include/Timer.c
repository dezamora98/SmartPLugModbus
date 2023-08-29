#include "Timer.h"
#include <avr/interrupt.h>
#include <util/eu_dst.h>

volatile static uint16_t vMicT;
volatile static uint16_t vMic;
volatile static uint16_t vMil;

ISR(TIMER0_COMPA_vect)
{
    vMicT += 100;
    if (++vMic == 1000)
    {
        ++vMil;
    }
}

void TimerInit(void)
{
    // Clear count register
    TCNT0 = 0;
    // Comparer
    OCR0A = 200;
    // Normal mode
    TCCR0A = 0;
    // preescaller -> clk/8 -> 2MHz -> 1/0.5uS
    TCCR0B = (1 << CS01);
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
        event->tic = vMil + timeOut;
        break;
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
        if (event->tic <= vMil)
            return false;
    }
    return true;
}

void CheckTimerEvent(t_SPMEvent *SPM)
{
    for (uint8_t i = 0; i < sizeof_array(SPM->Array); ++i)
    {
        if(SPM->Array[i].EN)
            return;
    }
    TimerDisable();
}