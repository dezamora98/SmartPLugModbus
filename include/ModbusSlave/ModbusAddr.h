/**
 * @file ModbusAddr.h
 * @author Daniel Enrique Zamora Sifredo (dezamora98@gmail.com)
 *
 * @brief this header file defines the addresses of the data
 * types in the registers of the modbus slave
 *
 * @version 0.1
 * @date 2023-08-27
 *
 * @copyright Copyright (c) 2023
 */

#ifndef MODBUS_ADDR_H

#include <ctype.h>


#define sizeof_array(array) sizeof(array) / sizeof(array[0])

/**
 * @brief Enumeration to define the coil addresses
 */
typedef enum
{
    /**
     * @brief this field defines the start of the addresses,
     * it must not be changed in position
     */
    InitAddr_Coil = 0,

    /*User definitions*/
    ADDR_Plug_0 = InitAddr_Coil,
    ADDR_Plug_1,
    ADDR_Plug_2,
    ADDR_Plug_3,
    ADDR_Plug_4,
    ADDR_Plug_5,
    ADDR_Reset,
    /*End User definitions*/

    /**
     * @brief this field defines the end of the addresses,
     * it must not be changed in position@brief
     */
    SIZE_Coil
} AddrCoil;

/**
 * @brief Enumeration to define the coil addresses
 */
typedef enum
{
    /**
     * @brief this field defines the start of the addresses,
     * it must not be changed in position
     */
    InitAddr_DiscretImput = 0,

    /*User definitions*/
    /*End User definitions*/

    /**
     * @brief this field defines the end of the addresses,
     * it must not be changed in position@brief
     */
    SIZE_DiscretImput
} AddrDiscretImput;

/**
 * @brief Enumeration to define the holding registers addresses
 */
typedef enum
{
    /**
     * @brief this field defines the start of the addresses,
     * it must not be changed in position
     */
    InitAddr_HoldingReg = 0,

    /*User definitions*/
    ADDR_Param_PlugOverCurrent = InitAddr_HoldingReg,
    ADDR_Param_PlugLowCurrent,
    ADDR_Param_OverVoltage,
    ADDR_Param_LowVoltage,

    ADDR_Param_HighTemperature,
    ADDR_Param_SystemOverCurrent,
    ADDR_Param_SlaveID,

    ADDR_Param_TimeoutPlugLowCurrent,
    ADDR_Param_TimeoutPlugOverCurrent,
    ADDR_Param_TimeoutLowVoltage,
    ADDR_Param_TimeoutOverVoltage,
    ADDR_Param_TimeoutHighTemperature,
    /*End User definitions*/

    /**
     * @brief this field defines the end of the addresses,
     * it must not be changed in position@brief
     */
    SIZE_HoldingReg
} AddrHoldingReg;

/**
 * @brief Enumeration to define the input registers addresses
 */
typedef enum
{
    /**
     * @brief this field defines the start of the addresses,
     * it must not be changed in position
     */
    InitAddr_InputReg = 0,

    /*User definitions*/
    ADDR_Reg_SystemState = InitAddr_InputReg,
    ADDR_Reg_PlugState_0,
    ADDR_Reg_PlugState_1,
    ADDR_Reg_PlugState_2,
    ADDR_Reg_PlugState_3,
    ADDR_Reg_PlugState_4,
    ADDR_Reg_PlugState_5,

    // analog registers
    ADDR_Reg_PlugCurrent_0,
    ADDR_Reg_PlugCurrent_1,
    ADDR_Reg_PlugCurrent_2,
    ADDR_Reg_PlugCurrent_3,
    ADDR_Reg_PlugCurrent_4,
    ADDR_Reg_PlugCurrent_5,
    ADDR_Reg_Voltage,
    ADDR_Reg_BoardCurrent,
    ADDR_Reg_TempMCU,
    /*End User definitions*/

    /**
     * @brief this field defines the end of the addresses,
     * it must not be changed in position.
     */
    SIZE_InputReg
} AddrInputReg;

/*---------------------------------------------------------------------------------------*/

/**
 * @brief other useful definitions, in this case an enumeration
 *  with the possible states of the status records
 */
typedef enum
{
    st_Off = 0,
    st_On,
    st_OverCurrent,
    st_LowCurrent,
    st_OverVoltage,
    st_LowVoltage,
    st_SystemOverCurrent,
    st_HighTemterature,
    st_UndefinedError
} PlugState;

#endif // !MODBUS_ADDR_H
