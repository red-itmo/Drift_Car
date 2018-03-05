import serial

port = serial.Serial(  port="/dev/ttyACM0",
                        baudrate=9600,
                        parity=serial.PARITY_NONE,
                        stopbits=serial.STOPBITS_ONE,
                        bytesize=serial.EIGHTBITS)
port.flushInput()
port.flushOutput()
vel1 = "1.5"
vel2 = "2.5"
port.write("<" + vel1 + ";" + vel2 + ">")
#port.write("<1.5;2.5>")
while(True):
    print(port.read())
