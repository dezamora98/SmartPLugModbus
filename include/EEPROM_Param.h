#ifndef EEPLROM_PARAM_H
#define EEPLROM_PARAM_H

#include <ctype.h>
#include <stdbool.h>
#include <avr/eeprom.h>

bool ParamCheck(void);
#define UpdatePersistentParam() eeprom_write_block((void *)HoldingReg.Array, (void *)0, SIZE_HoldingReg * sizeof(uint16_t))
#define UpdateVolatileParam()  eeprom_read_block((void *)HoldingReg.Array, (void *)0, SIZE_HoldingReg * sizeof(uint16_t))

#endif // !EEPLROM_PARAM_H