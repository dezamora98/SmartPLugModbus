/**
 * @file ModbusStruct.h
 * @author Daniel Enrique Zamora Sifredo (dezamora98@gmail.com)
 *
 * @brief this header is used for the user to define the data
 * structures of his variables in the modbus slave according
 * to the addresses set in ModbusAddr.h
 *
 * @version 0.1
 * @date 2023-08-27
 *
 * @copyright Copyright (c) 2023
 *
 * @example this example shows the basic way of forming the
 * data structures of the slave's Modbus stack:
 * @code
 *
 * typedef struct
 * {
 *     uint8_t Array[(SIZE_Coil + 7) / 8];
 * } t_Coil;
 *
 * typedef struct
 * {
 *     uint8_t Array[(SIZE_DiscreteInput + 7) / 8];
 * } t_DiscreteInput;
 *
 *
 * typedef struct
 * {
 *     union
 *     {
 *         uint16_t Array[SIZE_HoldingReg];
 *         // variables contained in the array in an ordered form
 *     };
 * } t_HoldingReg;
 *
 * typedef struct
 * {
 *     union
 *     {
 *         uint16_t Array[SIZE_InputReg];
 *         // variables contained in the array in an ordered form
 *     };
 * } t_InputReg;
 *
 * @endcond
 */

#ifndef MODBUS_STRUCT_H
#define MODBUS_STRUCT_H

#include "ModbusAddr.h"
#include <stdint.h>

/// @brief this structure is used to define the modbus slave coils.
#pragma pack(push, 1)
typedef struct
{
    union
    {
        uint8_t Array[(SIZE_Coil + 7) / 8];
        struct
        {
            uint8_t Plug_0 : 1;
            uint8_t Plug_1 : 1;
            uint8_t Plug_2 : 1;
            uint8_t Plug_3 : 1;
            uint8_t Plug_4 : 1;
            uint8_t Plug_5 : 1;
            uint8_t Reset : 1;
        };
    };
} t_Coil;
#pragma pack(pop)
/// @brief this structure is used to define the modbus slave holding registers.
#pragma pack(push, 1)
typedef struct
{
    union
    {
        uint16_t Array[SIZE_HoldingReg];
        ///@brief user arrays or variables,must be defined in the same order of the addresse
        struct
        {
            /// @brief  maximum current parameter on the plugs
            uint16_t PlugOverCurrent;
            /// @brief  minimum current parameter in the plugs
            uint16_t PlugLowCurrent;
            /// @brief  maximum line voltage parameter
            uint16_t OverVoltage;
            /// @brief  minimum line voltage parameter
            uint16_t LowVoltage;
            /// @brief  maximum mcu temperature parameter
            uint16_t HighTemperature;
            /// @brief  maximum current parameter on the main board
            uint16_t SystemOverCurrent;
            /// @brief  to change the slave id
            uint16_t SlaveID;
            /// @brief timeout for low current protect action
            uint16_t TimeoutPlugLowCurrent;
            /// @brief timeout for overcurrent protect action
            uint16_t TimeoutPlugOverCurrent;
            /// @brief timeout for low voltage protect action
            uint16_t TimeoutLowVoltage;
            /// @brief timeout for overvoltage protect action
            uint16_t TimeoutOverVoltage;
            /// @brief timeout for high temperature protect action
            uint16_t TimeoutHighTemperature;
        };
    };
} t_HoldingReg;
#pragma pack(pop)

/// @brief this structure is used to define the modbus slave input registers.
#pragma pack(push, 1)
typedef struct
{
    union
    {
        uint16_t Array[SIZE_InputReg];
        ///@brief user arrays or variables,must be defined in the same order of the addresse
        struct
        {
            uint16_t SystemState;
            ///@brief Array for the status of all plugs
            uint16_t PlugState[ADDR_Reg_PlugState_5 - ADDR_Reg_PlugState_0 + 1];
            union
            {
                ///@brief Array for all analog registers
                uint16_t AnalogReg[ADDR_Reg_TempMCU - ADDR_Reg_PlugCurrent_0 + 1];
                struct
                {
                    /// @brief Array for current consumption of all plugs
                    uint16_t PlugCurrent[ADDR_Reg_PlugCurrent_5 - ADDR_Reg_PlugCurrent_0];
                    /// @brief Line voltage
                    uint16_t Voltage;
                    /// @brief Power consumption of the main board
                    uint16_t BoardCurrent;
                    /// @brief MCU temperature
                    uint16_t TempMCU;
                };
            };
        };
    };
} t_InputReg;
#pragma pack(pop)
/*****************************************************************************************************/

#endif // !MODBUS_STRUCT_H
