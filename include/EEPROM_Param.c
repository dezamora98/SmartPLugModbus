#include "EEPROM_Param.h"
#include "ModbusSlave/Modbus.h"
#include <string.h>

bool ParamCheck(void)
{
    uint16_t temp[SIZE_HoldingReg];
    eeprom_read_block((void *)temp, (void *)0, SIZE_HoldingReg * sizeof(uint16_t));
    return memcmp(temp, HoldingReg.Array, SIZE_HoldingReg * sizeof(uint16_t)) == 0;
}