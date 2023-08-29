# FW_SmartPlugModbus

Repositorio para el firmware de la placa SmartPlugModbus.

## Protocolo MODBUS-RTU en SmartPlugModbus

El esclavo SmartPlugModbus en su versión **beta 1.0.0** controla y protege hasta **seis enchufes** e incorpora una **protección de línea** por tensión y corriente, además de incorporar un **sensor de temperatura de CPU**(_solo para protección contra sobre-temperatura en la cabina_).

Para establecer los parámetros de las protecciones, el Maestro MODBUS debe modificar interactuar con los Holdings Registers de las siguientes direcciones:

| Addr | HoldingRegister              | Description                   |
|------|------------------------------|-------------------------------|
| 0    | Param_OverCurrent            | Overcurrent limit             |  
| 1    | Param_LowCurrent             | Low current limit             |
| 2    | Param_OverVoltage            | Overvoltage limit             |
| 3    | Param_LowVoltage             | Low voltage limit             |
| 4    | Param_HighTemperature        | MCU high temperature          |
| 5    | Param_SystemOverCurrent      | Overcurrent on the board      |
| 6    | Param_SlaveID                | Change device ID              |
| 7    | Param_TimeoutPlugLowCurrent  | Low current fault time-out    |
| 8    | Param_TimeoutPLugOverCurrent | Overcurrent fault time-out    |
| 9    | Param_Timeout_LowVoltage     | Low voltage fault time-out    |
| 10   | Param_Timeout_OverVoltage    | Overcurrent fault time-out    |

____

Los parámetros de cada enchufe pueden ser leídos en los unos **registros de entrada** (solo lectura) en las siguientes direcciones.

| Addr | InputRegister      | Description                    |
|------|--------------------|--------------------------------|
| 0    |  Reg_PlugState_0   | Plug 0 status                  |
| 1    |  Reg_PlugState_1   | Plug 1 status                  |
| 2    |  Reg_PlugState_2   | Plug 2 status                  |
| 3    |  Reg_PlugState_3   | Plug 3 status                  |
| 4    |  Reg_PlugState_4   | Plug 4 status                  |
| 5    |  Reg_PlugState_5   | Plug 5 status                  |
| 6    |  Reg_Voltage   | Voltage measurement            |
| 7    |  Reg_BoardCurrent  | Main board current measurement |  
| 8    |  Reg_PlugCurrent_0 | Current measurement at plug 0  |
| 9    |  Reg_PlugCurrent_1 | Current measurement at plug 1  |
| 10   |  Reg_PlugCurrent_2 | Current measurement at plug 2  |
| 11   |  Reg_PlugCurrent_3 | Current measurement at plug 3  |
| 12   |  Reg_PlugCurrent_4 | Current measurement at plug 4  |
| 13   |  Reg_PlugCurrent_5 | Current measurement at plug 5  |
| 14   |  Reg_TempMCU       | Microcontroller temperature    |

* **Plug _n_ status**: indicates the status of the socket (ON or OFF). If a protection was activated, the socket will be in the deactivated state, but the cause of the state change will be shown in this register.States are defined as:

| Value | Name        |
|-------|-------------|
| 0     | OFF         |
| 1     | ON          |
| 2     | OverCurrent |
| 3     | LowCurrent  |
| 4     | OverVoltage |
| 5     | LowVoltage  |
____

Para reiniciar el módulo o modificar el estado de los enchufes, a estado encendido (**ON**) o apagado (**OFF**) se utilizan las siguientes salidas binarias (**Coils**):

| Addr  | Coil   |
|-------|--------|
| 0     | Plug_0 |
| 1     | Plug_1 |
| 2     | Plug_2 |
| 3     | Plug_3 |
| 4     | Plug_4 |
| 5     | Plug_5 |
| 6     | Reset  |
____

## Librería FreeModbus

Se escoge la librería [FreeModbus](https://www.embedded-experts.at/en/freemodbus/api-documentation/) por la su fiabilidad y su fácil portabilidad a otras arquitecturas de hardware. Para utilizar esta librería fue necesario realizar un port para el Atmega328P y se realizó una modificación a la rutina de encuesta de la pila MODBUS (```eMBErrorCode  eMBPoll();``` -> ```eMBErrorCode  eMBPoll(eMBEventType* ActualEvent);```) para que no bloquera el flujo del programa y solo fuera atendida cuando ocurra una interrupción UART. Además, se añadió una variable de estado que indica cuando debe ser ejecutado el sondeo a la pila MODBUS en una máquina de estado externa.
____

## Tools

1. Se recomienda utilizar la aplicación de test de modbus [qModbusMaster](https://github.com/Thuzerland/qModbusMaster).**Se detectó un problema en esta APP; si no se cambia de celda al modificar un campo de registros este no se actualiza y la trama que se envía no es la deseada**.
