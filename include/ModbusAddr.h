#ifndef MODBUSADDR_H
#define MODBUSADDR_H

#include "mb.h"
#include <avr/io.h>
#include <stdbool.h>

enum Addr_Coil
{
    Reset = 0,
    Plug_0,
    Plug_1,
    Plug_2,
    Plug_3,
    Plug_4,
    Plug_5,
    Size_CoilArray
};

enum Addr_HoldingReg
{
    Param_PlugOverCurrent = 0,      //
    Param_PlugLowCurrent,
    Param_OverVoltage,
    Param_LowVoltage,
    Param_TimeoutPlugLowCurrent,
    Param_TimeoutPlugOverCurrent,
    Param_TimeoutLowVoltage,
    Param_TimeoutOverVoltage,
    Param_HighTemperature,
    Param_SystemOverCurrent,
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
    st_Off,
    st_On,
    st_OverCurrent,
    st_LowCurrent,
    st_OverVoltage,
    st_LowVoltage,
};

volatile bool Coil[Size_CoilArray];
volatile uint16_t HoldingReg[Size_HoldingArray];
volatile uint16_t InputReg[Size_InputArray];
#define SlaveID 0x01

eMBErrorCode eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress,
                             USHORT usNRegs, eMBRegisterMode eMode)
{

    if ((usAddress + usNRegs) > (sizeof(HoldingReg) / sizeof(HoldingReg[0])))
    {
        return MB_ENOREG;
    }

    for (uint8_t i = 0; i < usNRegs; ++i)
    {

        if (eMode == MB_REG_READ)
        {

            pucRegBuffer[i * 2 + 0] = (uint8_t)(HoldingReg[i + usAddress] >> 8);
            pucRegBuffer[i * 2 + 1] = (uint8_t)(HoldingReg[i + usAddress] & 0xFF);
        }
        else if (eMode == MB_REG_WRITE)
        {

            HoldingReg[i + usAddress] = (pucRegBuffer[i * 2 + 0] << 8) | pucRegBuffer[i * 2 + 1];
        }
    }
    return MB_ENOERR;
}

eMBErrorCode eMBRegInputCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs)
{
    if ((usAddress + usNRegs) > (sizeof(InputReg) / sizeof(InputReg[0])))
    {
        return MB_ENOREG;
    }
    for(uint8_t i = 0; i < usNRegs; ++i)
    {
        pucRegBuffer[i * 2 + 0] = (uint8_t)(InputReg[i + usAddress] >> 8);
        pucRegBuffer[i * 2 + 1] = (uint8_t)(InputReg[i + usAddress] & 0xFF);
    }
    return MB_ENOERR;
}

eMBErrorCode eMBRegCoilsCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNCoils,
                           eMBRegisterMode eMode)
{
    return MB_ENOERR;
}

eMBErrorCode eMBRegDiscreteCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNDiscrete)
{
    return MB_ENOREG;
}

#endif // !MODBUSADDR_H
