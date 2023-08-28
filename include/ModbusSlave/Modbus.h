#ifndef MODBUSADDR_H
#define MODBUSADDR_H

#include "mb.h"
#include "ModbusStruct.h"

t_HoldingReg HoldingReg;
t_InputReg InputReg;
t_Coil Coil; 

/**
 * @brief Function for polling the modbus stack
 */
void ModbusPoll(void);

eMBErrorCode eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress,
                             USHORT usNRegs, eMBRegisterMode eMode);

eMBErrorCode eMBRegInputCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs);

eMBErrorCode eMBRegCoilsCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNCoils,
                           eMBRegisterMode eMode);

eMBErrorCode eMBRegDiscreteCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNDiscrete);

#endif // !MODBUSADDR_H
