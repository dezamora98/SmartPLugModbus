#ifndef MODBUS_STRUCT_H
#define MODBUS_STRUCT_H
#include "ModbusAddr.h"
#include <stdint.h>

/// @brief de
typedef struct
{
    uint8_t Array[(SIZE_Coil + 7) / 8];
} t_Coil;

/// @brief de
typedef struct
{
    union
    {
        uint16_t Array[SIZE_HoldingReg];
        /**
         * @brief user arrays or variables,must be defined in the same order of the addresse
         */
        struct
        {
            uint16_t PlugOverCurrent;
            uint16_t PlugLowCurrent;
            uint16_t OverVoltage;
            uint16_t LowVoltage;
            uint16_t HighTemperature;
            uint16_t SystemOverCurrent;
            uint16_t SlaveID;
            uint16_t TimeoutPlugLowCurrent;
            uint16_t TimeoutPlugOverCurrent;
            uint16_t TimeoutLowVoltage;
            uint16_t TimeoutOverVoltage;
        };
    };
}t_HoldingReg;

/// @brief de
typedef struct 
{
    union
    {
        uint16_t Array[SIZE_InputReg];
        /**
         * @brief user arrays or variables,must be defined in the same order of the addresse
         */
        struct
        {
            uint16_t PlugState[ADDR_Reg_PlugState_5-ADDR_Reg_PlugState_0 + 1];
            union
            {
                uint16_t AnalogReg[ADDR_Reg_TempMCU-ADDR_Reg_PlugVoltage + 1];
                struct
                {
                    uint16_t PlugVoltage;
                    uint16_t BoardCurrent;
                    uint16_t PlugCurrent[ADDR_Reg_PlugCurrent_5 - ADDR_Reg_PlugCurrent_0];
                    uint16_t TempMCU;
                };
            };
        };
    };
}t_InputReg;

#endif // !MODBUS_STRUCT_H
