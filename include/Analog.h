/**
 * @file Analog.h
 * @author Daniel Enrique Zamora Sifredo (dezamora98@gmail.com)
 * 
 * @brief this file is used to generate prototypes of the functions
 * required in the SmartPlugModbus analog tasks.
 * 
 * @version 0.1
 * @date 2023-08-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef ANALOG_H
#define ANALOG_H

#include <avr/io.h>

/// @brief defines the ordered path of ADC readings
const static uint8_t Analog_Chanel[] = {5, 4, 0, 1, 2, 3, 6, 7, 8};

/// @brief analog channel iterator
volatile static uint8_t Analog_Iterator = 0;

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
