#ifndef ANALOG_H
#define ANALOG_H

#include "Modbus.h"

const static uint8_t Analog_Chanel[] = {5, 4, 0, 1, 2, 3, 6, 7, 8}; // defines the ordered path of ADC readings
volatile static uint8_t Analog_Iterator = 0;                        // analog channel iterator
#define size_ACH sizeof(Analog_Chanel) / sizeof(Analog_Chanel[0])   // number of analog channels

/**
 * @brief Initializes analog tasks:
 * This function sets the ADC to 125kHz according
 * to the CPU working frequency (F_CPU). This guarantees
 * an ADC response time of approximately 100uS; therefore,
 * if 10 consecutive channels are polled, an update time of
 * approximately 1ms is achieved.
 */
void AnalogInit(void);

/**
 * @brief this function checks all analog variables of the
 * system and, if any of them exceeds the threshold defined
 * in the parameters (HoldingRegister), it changes the status
 * variable of the FSM.
 */
void AnalogCheck(void);

#endif // !ANALOG_H
