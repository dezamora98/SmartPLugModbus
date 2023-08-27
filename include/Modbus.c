#include "Modbus.h"


eMBErrorCode eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress,
                             USHORT usNRegs, eMBRegisterMode eMode)
{

    if ((usAddress + usNRegs) > Size_HoldingArray)
    {
        return MB_ENOREG;
    }

    switch (eMode)
    {
    case MB_REG_READ:
        for (uint8_t i = 0; i < usNRegs; ++i)
        {
            pucRegBuffer[i * 2 + 0] = (uint8_t)(HoldingReg[i + usAddress] >> 8);
            pucRegBuffer[i * 2 + 1] = (uint8_t)(HoldingReg[i + usAddress] & 0xFF);
        }
        break;

    case MB_REG_WRITE:
        for (uint8_t i = 0; i < usNRegs; ++i)
        {
            HoldingReg[i + usAddress] = (pucRegBuffer[i * 2 + 0] << 8) | pucRegBuffer[i * 2 + 1];
        }
        break;

    default:
        return MB_EINVAL;
    }

    return MB_ENOERR;
}

eMBErrorCode eMBRegInputCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs)
{
    if ((usAddress + usNRegs) > Size_InputArray)
    {
        return MB_ENOREG;
    }
    for (uint8_t i = 0; i < usNRegs; ++i)
    {
        pucRegBuffer[i * 2 + 0] = (uint8_t)(InputReg[i + usAddress] >> 8);
        pucRegBuffer[i * 2 + 1] = (uint8_t)(InputReg[i + usAddress] & 0xFF);
    }
    return MB_ENOERR;
}

eMBErrorCode eMBRegCoilsCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNCoils,
                           eMBRegisterMode eMode)
{
    /* Check if we have registers mapped at this block. */
    if ((usAddress + usNCoils) > Size_CoilArray)
    {
        return MB_ENOREG;
    }
    else
    {
        switch (eMode)
        {
            /* Read current values and pass to protocol stack. */
        case MB_REG_READ:
            *pucRegBuffer = (Coil[0] >> usAddress) & ((1 << usNCoils) - 1);
            break;

            /* Update current register values. */
        case MB_REG_WRITE:
            Coil[0] = (Coil[0] & ~(((1 << usNCoils) - 1) << usAddress)) |
                      ((*pucRegBuffer << usAddress) & (((1 << usNCoils) - 1) << usAddress));
            break;

        default:
            return MB_EINVAL;
        }
    }
    return MB_ENOERR;
}

eMBErrorCode eMBRegDiscreteCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNDiscrete)
{
    return MB_ENOREG;
}