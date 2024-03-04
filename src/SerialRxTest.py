import serial
import struct
import time

ser = serial.Serial('/dev/ttyACM0', 115200, timeout=None)

try:
    while True:
        line = ser.readline().strip()
        data_str = line.decode('utf-8')
        print("Baud rate received:", data_str)
            
except KeyboardInterrupt:
    ser.close()
    print("Serial port closed")


'''DutyCycle = 41.00001 #message saved to local attribute
DutyCycleBytes = struct.pack('f', DutyCycle) #package python float into 4 byte transmission to be send off(check struct documentation for byte order if necessary)
for i in range(0,19):
    ser.write(b'\xa0') #first write two 170 bytes to serial to tell teensy next 4 bytes are pid command
    ser.write(b'\xaa') #this technique is susiptible to dropped and partial packets but at 250hz we should be fine with 1 or 2 bad packets
    ser.write(DutyCycleBytes)

for i in range(0,3):
    ser.write(b'\xaa') #first write two 170 bytes to serial to tell teensy next 4 bytes are pid command
    ser.write(b'\xaa') #this technique is susiptible to dropped and partial packets but at 250hz we should be fine with 1 or 2 bad packets
    ser.write(DutyCycleBytes) #write floating point duty cycle from PID calculation to serial for teensy to see

time.sleep(0.25)

#print(len(DutyCycleBytes))
while True:
    response_bytes = ser.read(len(DutyCycleBytes))
    response_value = struct.unpack('f', response_bytes)[0]
    print(response_value)
    time.sleep(0.01)'''