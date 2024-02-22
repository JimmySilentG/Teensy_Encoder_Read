import serial
import struct
import time

ser = serial.Serial('/dev/ttyACM0',115200,timeout = None) #open serial port assigned to teensy, this can change between ACMO and ACM1, set baudrate to 115200 even though teensy ignores it. timeout set to none so the port waits forever until the requested number of bytes are recieved

DutyCycle = 50.00001 #message saved to local attribute
DutyCycleBytes = struct.pack('f', DutyCycle) #package python float into 4 byte transmission to be send off(check struct documentation for byte order if necessary)
for i in range(0,19):
    ser.write(b'\xa0')
    print(i)

ser.write(b'\xaa') #first write two 170 bytes to serial to tell teensy next 4 bytes are pid command
ser.write(b'\xaa') #this technique is susiptible to dropped and partial packets but at 250hz we should be fine with 1 or 2 bad packets
ser.write(DutyCycleBytes) #write floating point duty cycle from PID calculation to serial for teensy to see

time.sleep(0.25)

#print(len(DutyCycleBytes))
while True:
    response_bytes = ser.read(len(DutyCycleBytes))
    response_value = struct.unpack('f', response_bytes)[0]
    print(response_value)
    time.sleep(0.01)