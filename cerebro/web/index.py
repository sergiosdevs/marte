import web
import serial
import re
ser = serial.Serial(
    port = '/dev/ttyACM0' ,
    baudrate = 9600 ,
    timeout = 1
)


urls = (
    '/(.*)', 'control'
)
app = web.application(urls, globals())
# peticiones
# shot , up* , down* , left* , right* , stop

# vup , vdown , vleft , vright , mup , mdown

patron = re.compile('\d+')
MAX = 100
MIN = 0

class control:
    def GET( self , peticion ):
        if not peticion:
            return 0
        
        if re.match( "shot" , peticion ) :
            #ser.write( chr(0xFF)+chr(0X03)+chr(0X0D)+chr(0X01)+chr(0X00) )
            return patron.findall( peticion )[0]

        if re.match( "up" , peticion ):
            vel = int( patron.findall( peticion )[0] )
            if vel > MIN and vel <= MAX:
                ser.write( chr(0xFF)+chr(0X0A)+ hex( val ) + chr(0X00)+chr(0X00) )
                return "up: "
            else:
                return 0

        if re.match( "down" , peticion ):
            vel = int( patron.findall( peticion )[0] )
            if vel > MIN and vel <= MAX:
                ser.write( chr(0xFF)+chr(0X0B)+ hex( val ) +chr(0X00)+chr(0X00) )
                return "down: "
            else:
                return 0

        if re.match( "stop" , peticion ):
            ser.write( chr(0xFF)+chr(0X1A)+chr(0X00)+chr(0X00)+chr(0X00) )

        if re.match( "left" , peticion ):
            #ser.write( chr(0xFF)+chr(0X03)+chr(0X0D)+chr(0X01)+chr(0X00) )
            return "left"
        if re.match( "right" , peticion ):
            #ser.write( chr(0xFF)+chr(0X03)+chr(0X0D)+chr(0X01)+chr(0X00) )
            return "right"
        # control de camara
        if re.match( "vup" , peticion ):
            #ser.write( chr(0xFF)+chr(0X03)+chr(0X0D)+chr(0X01)+chr(0X00) )
            return "vup"

        if re.match( "vdown" , peticion ):
            #ser.write( chr(0xFF)+chr(0X03)+chr(0X0D)+chr(0X01)+chr(0X00) )
            return "vdown"

        if re.match( "vleft" , peticion ):
            #ser.write( chr(0xFF)+chr(0X03)+chr(0X0D)+chr(0X01)+chr(0X00) )
            return "vleft"

        if re.match( "vright" , peticion ):
            #ser.write( chr(0xFF)+chr(0X03)+chr(0X0D)+chr(0X01)+chr(0X00) )
            return "vright"

        # control   microscopio
        if re.match( "mup" , peticion ):
            #ser.write( chr(0xFF)+chr(0X03)+chr(0X0D)+chr(0X01)+chr(0X00) )
            return "mup"

        if re.match( "mdown" , peticion ):
            #ser.write( chr(0xFF)+chr(0X03)+chr(0X0D)+chr(0X01)+chr(0X00) )
            return "mdown"


    def POST( self , peticion ):
        if peticion:
            #ser.write( chr(0xFF)+chr(0X03)+chr(0X0D)+chr(0X01)+chr(0X00) )
            return "1"
        else:
            return "0"    

application = app.wsgifunc()
