import minimalmodbus

minimalmodbus.BAUDRATE = 38400

# Utiliza la dirección correcta del esclavo en lugar de 0
slave = minimalmodbus.Instrument("COM9", 1, minimalmodbus.MODE_RTU, True, True)

slave.write_registers(1,[int("0xFFFF",16)])
slave.read_registers(0,8,3)