#include "Modbus.h"
#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdbool.h>

void ModbusPoll(void)
{
    eMBEventType MB_eEvent = EV_READY;
    eMBErrorCode MB_ERROR;

    MB_ERROR = eMBPoll(&MB_eEvent);

    if (MB_ERROR != MB_ENOERR || MB_eEvent == EV_READY || MB_eEvent == EV_FRAME_SENT)
    {
        FSM_State = FSM_LastState;
    }    
}

eMBErrorCode eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress,
                             USHORT usNRegs, eMBRegisterMode eMode)
{

    if ((usAddress + usNRegs) > SIZE_HoldingReg)
    {
        return MB_ENOREG;
    }

    switch (eMode)
    {
    case MB_REG_READ:
        for (uint8_t i = 0; i < usNRegs; ++i)
        {
            pucRegBuffer[i * 2 + 0] = (uint8_t)(HoldingReg.Array[i + usAddress] >> 8);
            pucRegBuffer[i * 2 + 1] = (uint8_t)(HoldingReg.Array[i + usAddress] & 0xFF);
        }
        break;

    case MB_REG_WRITE:
        for (uint8_t i = 0; i < usNRegs; ++i)
        {
            HoldingReg.Array[i + usAddress] = (pucRegBuffer[i * 2 + 0] << 8) | pucRegBuffer[i * 2 + 1];
        }
        break;

    default:
        return MB_EINVAL;
    }

    return MB_ENOERR;
}

eMBErrorCode eMBRegInputCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs)
{
    if ((usAddress + usNRegs) > SIZE_InputReg)
    {
        return MB_ENOREG;
    }
    for (uint8_t i = 0; i < usNRegs; ++i)
    {
        pucRegBuffer[i * 2 + 0] = (uint8_t)(InputReg.Array[i + usAddress] >> 8);
        pucRegBuffer[i * 2 + 1] = (uint8_t)(InputReg.Array[i + usAddress] & 0xFF);
    }
    return MB_ENOERR;
}

eMBErrorCode eMBRegCoilsCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNCoils,
                           eMBRegisterMode eMode)
{
    /* Check if we have registers mapped at this block. */
    if ((usAddress + usNCoils) > SIZE_Coil)
    {
        return MB_ENOREG;
    }
    else
    {
        switch (eMode)
        {
            /* Read current values and pass to protocol stack. */
        case MB_REG_READ:
            *pucRegBuffer = (Coil.Array[0] >> usAddress) & ((1 << usNCoils) - 1);
            break;

            /* Update current register values. */
        case MB_REG_WRITE:
            Coil.Array[0] = (Coil.Array[0] & ~(((1 << usNCoils) - 1) << usAddress)) |
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