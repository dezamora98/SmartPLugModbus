#ifndef MODBUSADDR_H
#define MODBUSADDR_H

#include "mb.h"
#include <avr/io.h>
#include <stdbool.h>

enum class Addr_OutCoil
{
    Reset = 0,
    Plug_0,
    Plug_1,
    Plug_2,
    Plug_3,
    Plug_4,
    Plug_5,
    Plug_6,
    Plug_7,
};

enum class Addr_OutReg
{
    Param_OverCurrent = 0,
    Param_LowCurrent,
    Param_OverVoltage,
    Param_LowVoltage,
};

enum class Addr_InReg
{
    PlugVoltage = 0,
    PlugState_0,
    PlugState_1,
    PlugState_2,
    PlugState_3,
    PlugState_4,
    PlugState_5,
    PlugState_6,
    PlugState_7,
    PlugCurrent_0,
    PlugCurrent_1,
    PlugCurrent_2,
    PlugCurrent_3,
    PlugCurrent_4,
    PlugCurrent_5,
    PlugCurrent_6,
    PlugCurrent_7,
};

enum class PlugState
{
    Off,
    On,
    OverCurrent,
    LowCurrent,
    OverVoltage,
    LowVoltage,
};

static bool OutCoil[9];
static uint16_t OutReg[4];
static uint16_t InReg[16];

eMBErrorCode eMDStatus;
#define SlaveID 0x01

eMBErrorCode eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress,
                             USHORT usNRegs, eMBRegisterMode eMode)
{
    PORTC = 1 << PORTC0;

    if (eMode == MB_REG_READ)
    {
        if ((usAddress + usNRegs - 1) > (sizeof(OutReg) / sizeof(OutReg[0])))
        {
            return MB_ENOREG;
        }
    }
    else
    {
        return MB_EINVAL;
    }

    for (uint8_t i = 0; i < usNRegs; i++)
    {

        if (eMode == MB_REG_READ)
        {

            pucRegBuffer[i * 2 + 0] = (uint8_t)(OutReg[i + usAddress] >> 8);
            pucRegBuffer[i * 2 + 1] = (uint8_t)(OutReg[i + usAddress] & 0xFF);
        }
        else if (eMode == MB_REG_WRITE)
        {

            OutReg[i + usAddress] = (pucRegBuffer[i * 2 + 0] << 8) | pucRegBuffer[i * 2 + 1];
        }
    }
    return MB_ENOERR;
}

eMBErrorCode eMBRegCoilsCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNCoils,
                           eMBRegisterMode eMode)
{
    return MB_ENOERR;
}

eMBErrorCode eMBRegInputCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs)
{
    return MB_ENOREG;
}

eMBErrorCode eMBRegDiscreteCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNDiscrete)
{
    return MB_ENOREG;
}

#endif // !MODBUSADDR_H
