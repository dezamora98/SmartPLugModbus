#ifndef MODBUSADDR_H
#define MODBUSADDR_H

#include "mb.h"
#include "ModbusStruct.h"

#define SlaveID 0x01

t_HoldingReg HoldingReg;
t_InputReg InputReg;
t_Coil Coil; 

void ModbusPoll(void);

eMBErrorCode eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress,
                             USHORT usNRegs, eMBRegisterMode eMode);

eMBErrorCode eMBRegInputCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs);

eMBErrorCode eMBRegCoilsCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNCoils,
                           eMBRegisterMode eMode);

eMBErrorCode eMBRegDiscreteCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNDiscrete);

#endif // !MODBUSADDR_H
