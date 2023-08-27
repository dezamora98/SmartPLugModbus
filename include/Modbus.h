#ifndef MODBUSADDR_H
#define MODBUSADDR_H

#include "mb.h"

enum Addr_Coil
{
    Plug_0 = 0,
    Plug_1,
    Plug_2,
    Plug_3,
    Plug_4,
    Plug_5,

    Reset,

    Size_CoilArray
};

enum Addr_HoldingReg
{
    Param_PlugOverCurrent = 0, //
    Param_PlugLowCurrent,
    Param_OverVoltage,
    Param_LowVoltage,

    Param_HighTemperature,
    Param_SystemOverCurrent,
    Param_SlaveID,

    Param_TimeoutPlugLowCurrent,
    Param_TimeoutPlugOverCurrent,
    Param_TimeoutLowVoltage,
    Param_TimeoutOverVoltage,

    Size_HoldingArray
};

enum Addr_InputReg
{
    Reg_PlugState_0 = 0,
    Reg_PlugState_1,
    Reg_PlugState_2,
    Reg_PlugState_3,
    Reg_PlugState_4,
    Reg_PlugState_5,

    // analog registers
    Reg_PlugVoltage,
    Reg_BoardCurrent,
    Reg_PlugCurrent_0,
    Reg_PlugCurrent_1,
    Reg_PlugCurrent_2,
    Reg_PlugCurrent_3,
    Reg_PlugCurrent_4,
    Reg_PlugCurrent_5,
    Reg_TempMCU,

    Size_InputArray
};

enum PlugState
{
    st_Off = 0,
    st_On,
    st_OverCurrent,
    st_LowCurrent,
    st_OverVoltage,
    st_LowVoltage,
};

/**
 * the expression @code Size_CoilArray + 7) / 8 ensures rounding for the required number of bytes of the array
 **/
// volatile uint8_t Coil[(Size_CoilArray + 7) / 8];
volatile uint8_t Coil;
volatile uint16_t HoldingReg[Size_HoldingArray];
volatile uint16_t InputReg[Size_InputArray];

#define PlugOverCurrent HoldingReg[Param_PlugOverCurrent]
#define PlugLowCurrent HoldingReg[Param_PlugLowCurrent]
#define OverVoltage HoldingReg[Param_OverVoltage]
#define LowVoltage HoldingReg[Param_LowVoltage]
#define TimeoutPlugLowCurrent HoldingReg[Param_TimeoutPlugLowCurrent]
#define TimeoutPlugOverCurrent HoldingReg[Param_TimeoutPlugOverCurrent]
#define TimeoutLowVoltage HoldingReg[Param_TimeoutLowVoltage]
#define TimeoutOverVoltage HoldingReg[Param_TimeoutOverVoltage]
#define HighTemperature HoldingReg[Param_HighTemperature]
#define SystemOverCurrent HoldingReg[Param_SystemOverCurrent]
//#define SlaveID HoldingReg[Param_SlaveID]

#define ArrayAnalogReg (InputReg + Reg_PlugVoltage)
#define size_ArrayAnalogReg (Reg_PlugCurrent_5 - Reg_PlugVoltage + 1)

#define ArrayCurrentPlug (InputReg + Reg_PlugCurrent_0)
#define size_ArrayCurrentPlug (Reg_PlugCurrent_5 - Reg_PlugCurrent_0 + 1)

#define ArrayPlugState (InputReg + Reg_PlugState_0)
#define size_ArrayPlugState (Reg_PlugState_5 - Reg_PlugState_0 + 1)

#define PlugVoltage InputReg[Reg_PlugVoltage]
#define BoardCurrent InputReg[Reg_BoardCurrent]
#define TempMCU InputReg[Reg_TempMCU]
#define BoardCurrent InputReg[Reg_BoardCurrent]

#define SlaveID 0x01

void ModbusPoll(void);

eMBErrorCode eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress,
                             USHORT usNRegs, eMBRegisterMode eMode);

eMBErrorCode eMBRegInputCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs);

eMBErrorCode eMBRegCoilsCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNCoils,
                           eMBRegisterMode eMode);

eMBErrorCode eMBRegDiscreteCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNDiscrete);

#endif // !MODBUSADDR_H
