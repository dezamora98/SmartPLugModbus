#include <ModbusSlave.h>
#include <Arduino_FreeRTOS.h>

#define SLAVE_ID 1           // The Modbus slave ID, change to the ID you want to use.
#define SERIAL_BAUDRATE 9600 // Change to the baudrate you want to use for Modbus communication.
#define SERIAL_PORT Serial   // Serial port to use for RS485 communication, change to the port you're using.

// Comment out the following line if your not using RS485
#define RS485_CTRL_PIN 8 // Change to the pin the RE/DE pin of the RS485 controller is connected to.

enum class State
{
    off,
    on,
    over_current,
    low_current,
    over_voltage,
    low_voltage,
};

enum class CoilsModule
{
    reset = 0,
    coil_0,
    coil_1,
    coil_2,
    coil_3,
    coil_4,
    coil_5,
    coil_6,
    coil_7,
};

enum class PlugConstant
{
    const_over_current = 0,
    const_low_current = 2,
    const_over_voltage = 4,
    const_low_voltage = 6,
};

enum class PlugParam
{
    voltage = 0,
    current_0 = 2,
    current_1 = 4,
    current_2 = 6,
    current_3 = 8,
    current_4 = 10,
    current_5 = 12,
    current_6 = 14,
    current_7 = 16,
};

enum class PlugState
{
    state_0 = 18,
    state_1 = 19,
    state_2 = 20,
    state_3 = 21,
    state_4 = 22,
    state_5 = 23,
    state_6 = 24,
    state_7 = 25,
};

bool Coil[8];
uint8_t HoldingRegister[8];
uint8_t InputRegister[26];

// You shouldn't have to change anything below this to get this example to work

#ifdef RS485_CTRL_PIN
// Modbus object declaration
Modbus slave(SERIAL_PORT, SLAVE_ID, RS485_CTRL_PIN);
#else
Modbus slave(SERIAL_PORT, SLAVE_ID);
#endif

uint8_t forceCoils(uint8_t fc, uint16_t address, uint16_t length);
uint8_t forceHoldingRegisters(uint8_t fc, uint16_t address, uint16_t length);
uint8_t readInputRegisters(uint8_t fc, uint16_t address, uint16_t length);

void setup()
{
    // Set the defined input pins to input mode.

    // Set the defined analog pins to input mode.

    // Set the defined output pins to output mode.

    // Register functions to call when a certain function code is received.
    slave.cbVector[CB_WRITE_COILS] = forceCoils;
    slave.cbVector[CB_WRITE_HOLDING_REGISTERS] = forceHoldingRegisters;
    slave.cbVector[CB_READ_INPUT_REGISTERS] = readInputRegisters;

    // Set the serial port and slave to the given baudrate.
    SERIAL_PORT.begin(SERIAL_BAUDRATE);
    slave.begin(SERIAL_BAUDRATE);
}

void loop()
{
    // Listen for modbus requests on the serial port.
    // When a request is received it's going to get validated.
    // And if there is a function registered to the received function code, this function will be executed.
    slave.poll();
}

// Modbus handler functions
// The handler functions must return an uint8_t and take the following parameters:
//     uint8_t  fc - function code
//     uint16_t address - first register/coil address
//     uint16_t length/status - length of data / coil status

uint8_t forceCoils(uint8_t fc, uint16_t address, uint16_t length)
{
    // Check if the requested addresses exist in the array
    if ((address + length) > sizeof(Coil) / sizeof(Coil[0]))
    {
        return STATUS_ILLEGAL_DATA_ADDRESS;
    }

    // Set the output pins to the given state.
    for (uint8_t i = 0; i < length; i++)
    {
        // Write the value in the input buffer to the digital pin.
        Coil[address + i] = slave.readCoilFromBuffer(i);
    }

    if (Coil[(uint8_t)CoilsModule::reset])
    {
        // reset MCU
    }

    return STATUS_OK;
}

uint8_t forceHoldingRegisters(uint8_t fc, uint16_t address, uint16_t length)
{
    // Check if the requested addresses exist in the array
    if ((address + length) > sizeof(HoldingRegister) / sizeof(HoldingRegister[0]))
    {
        return STATUS_ILLEGAL_DATA_ADDRESS;
    }

    // Set the output pins to the given state.
    for (uint8_t i = 0; i < length; i++)
    {
        // Write the value in the input buffer to the digital pin.
        HoldingRegister[address + i] = slave.readRegisterFromBuffer(i);
    }

    return STATUS_OK;
}

uint8_t readInputRegisters(uint8_t fc, uint16_t address, uint16_t length)
{
    // Check if the requested addresses exist in the array
    if ((address + length) > sizeof(InputRegister) / sizeof(InputRegister[0]))
    {
        return STATUS_ILLEGAL_DATA_ADDRESS;
    }

    // Set the output pins to the given state.
    for (uint8_t i = 0; i < length; i++)
    {
        // Write the state of the analog pin to the response buffer.
        slave.writeRegisterToBuffer(i,InputRegister[address + i]);
    }

    return STATUS_OK;
}
