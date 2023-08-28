#include "Timer.h"
#include <avr/interrupt.h>

ISR(TIMER0_COMPA_vect)
{
    ++vTime;
}

void TimerInit()
{
    TCCR0A |= (1 << WGM01);
    TCCR0B &= ~(1 << WGM02);

    // Configura el valor del registro OCR0A
    OCR0A = ((F_CPU / 64 / 1000) - 1);

    // Habilita la interrupción por comparación del Timer0
    TIMSK0 |= (1 << OCIE0A);

    // Configura el preescalador del Timer0
    TCCR0B |= (1 << CS01) | (1 << CS00);
    sei();
}