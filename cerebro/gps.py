import time
import serial
import re
import pynmea2

ser = serial.Serial(
    port = '/dev/ttyAMA0',
    baudrate = 38400 ,
    timeout = 1
)

while 1 :
    time.sleep( 0.3 )
    linea = str( ser.readline() )
    print linea
    if re.match( "\$GPGGA" , linea ) and re.match( "[WS]" , linea ):
        print linea
        partes_linea = pynmea2.parse( linea )
        if partes_linea.latitude:
            print partes_linea.latitude
    else:
        print "no recive datos del satelite"
    ser.flush()
ser.close()        
