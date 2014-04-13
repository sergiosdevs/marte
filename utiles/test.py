import time
import serial
import re
import pynmea2

ser = serial.Serial(
    port = '/dev/ttyACM0',
    baudrate = 9600 ,
    timeout = 1
)
#ser.write("dfsdfsd")
ser.write( chr(0xFF)+chr(0X03)+chr(0X0D)+chr(0X01)+chr(0X00) )
ser.close()        
