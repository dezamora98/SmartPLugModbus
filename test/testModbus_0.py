from pymodbus.client import ModbusSerialClient
from pymodbus.exceptions import ModbusException

client = ModbusSerialClient(method='rtu', port='COM9', baudrate=9600,stopbits=1,bytesize=8,)
client.write_registers(address=1,values=20,slave=1) # Escribe el valor 10 en el registro 1
result = client.read_holding_registers(1,1,slave=1) # Lee el registro 1
print(result)
if not result.isError():
    print(result.registers[1]) # Imprime el valor del registro 1
else:
    print('Se produjo un error al leer el registro:', result)
    if isinstance(result, ModbusException):
        print('Tipo de excepción:', type(result).__name__)

client.close()