#include <Arduino.h>

#include "Servo.h"
/*********************************************************************************
 **
 **  
 **  
 **
 *********************************************************************************/


///////////////////////////////////////

#define DEFAULTBAUDRATE 9600    

#define MODE_DEFAULT 0            // Arduino Mode
#define COMMANDLENGTH 5

#define M1 30
#define M2 31
#define M3 32
#define M4 33
#define M5 34
#define M6 35

#define Mdir 38

#define ARM_CAM1 22
#define ROT_CAM1 24
#define MICROSCOPE 23
#define S_S 25

#define ANG_M1 93
#define ANG_M2 89
#define ANG_M3 92

#define ANG_M4 89
#define ANG_M5 89
#define ANG_M6 90

unsigned char currentCommand[COMMANDLENGTH];    //  Command For The Arduino 


///////////////////////////////////////  
unsigned int retVal;
int currentMode;
unsigned int freq;
unsigned long duration;
Servo MOTOR1,MOTOR2,MOTOR3,MOTOR4,MOTOR5,MOTOR6,CAM1_R,CAM1_A,MICROSP,DIRECTION,SHARP_SENSOR;
int vel,vel1,vel2,vel3,vel4; 
///////////////////////////////////////

void setMode(int mode);
int checkForCommand();
void processCommand(unsigned char command[]);
void Sync();
void moveSensor(int angle);
void moveServo(int angle,Servo ser);

void setup()
{
	MOTOR6.attach(M6);
	MOTOR5.attach(M5);
	MOTOR4.attach(M4);
	MOTOR3.attach(M3);
	MOTOR2.attach(M2);
	MOTOR1.attach(M1);

	MOTOR6.write(ANG_M6);
	MOTOR5.write(ANG_M5);
	MOTOR4.write(ANG_M4);
	MOTOR3.write(ANG_M3);
	MOTOR2.write(ANG_M2);
	MOTOR1.write(ANG_M1);

	SHARP_SENSOR.attach(S_S);
	SHARP_SENSOR.write(0);
	CAM1_R.attach(ROT_CAM1);
	CAM1_A.attach(ARM_CAM1);
	MICROSP.attach(MICROSCOPE);
	DIRECTION.attach(Mdir);

	CAM1_R.write(90);
	CAM1_A.write(120);
	MICROSP.write(0);
	DIRECTION.write(0);
	pinMode(13,OUTPUT) ;
	Sync();
}
///////////////////////////////////////
void  loop()
{
	checkForCommand();
	delay(1);
}
///////////////////////////////////////

void setMode(int mode)
{
  currentMode = mode;
}
////////////////////////////////////////////////
int checkForCommand()
{  
  int bufferBytes = Serial.available();

  if(bufferBytes >= COMMANDLENGTH) 
  {
	for(int i=0; i<COMMANDLENGTH; i++)
	{
	  currentCommand[i] = Serial.read();       
	}     
	processCommand(currentCommand);     
	return 1;
  }
  else
  {
	return 0; 
  }
}
/////////////////////////////////////////////////
void processCommand(unsigned char command[])
{  
  if(command[0] == 0xFF )
  {
	switch(command[1])
	{    
	case 0x00:     
	  Serial.print("sync");
	  Serial.flush();        
	  break;
	case 0x01:    
	  Serial.flush();
	  break;
	  
	/*********************************************************************************
	**  Digital I/O Command
	*********************************************************************************/
	case 0x02:    // Set Pin      
	  pinMode(command[2], command[3]);
	  Serial.write(command[2]);
	  break;
	case 0x03:    // Write Digital Pin
	  digitalWrite(command[2], command[3]);
	   Serial.write('0');
	  break;
	
	case 0x06:    // Read Digital Pin
	  retVal = digitalRead(command[2]);  
	  Serial.write(retVal);    
	  break;
	case 0x07:    // Digital Read Port
	  retVal = 0x0000;
	  for(int i=0; i <=13; i++)
	  {
		if(digitalRead(i))
		{
		  retVal += (1<<i);
		} 
	  }
	  Serial.write( (retVal & 0xFF));
	  Serial.write( (retVal >> 8));    
	  break;
	  
	/*********************************************************************************
	** Analog read
	*********************************************************************************/
	case 0x08:    // Read Analog Pin    
	  retVal = analogRead(command[2]);
	  Serial.write( (retVal >> 8));
	  Serial.write( (retVal & 0xFF));
	  break;
	  
	/*********************************************************************************
	**  PWM Command
	*********************************************************************************/      
	case 0x09:    // PWM Write Pin
	  analogWrite(command[2], command[3]);
	   Serial.write('0');
	  break;
	  
	/*********************************************************************************
	** Servos
	*********************************************************************************/      
	case 0x0A:  // move forward
	 DIRECTION.write(0);
	 vel=map(command[2], 0, 100, 0, 90);
	 MOTOR1.write(ANG_M1+vel);
	 MOTOR2.write(ANG_M2+vel);
	 MOTOR3.write(ANG_M3+vel);

	 MOTOR4.write(ANG_M4-vel);
	 MOTOR5.write(ANG_M5-vel);
	 MOTOR6.write(ANG_M6-vel);
	  break;
	case 0x0B:	//move back
	 DIRECTION.write(0); 
	 vel=map(command[2], 0, 100, 90, 0);
	 MOTOR1.write(ANG_M1-vel);
	 MOTOR2.write(ANG_M2-vel);
	 MOTOR3.write(ANG_M3-vel);

	 MOTOR4.write(ANG_M4+vel);
	 MOTOR5.write(ANG_M5+vel);
	 MOTOR6.write(ANG_M6+vel); 
	  break;
	 case 0x0C:
	  moveSensor(command[2]);//SHARP SENSOR CONTROL
	  break;
	
    	break;
     case 0X0D://rotate left vel 100%
    	vel1=map(command[2], 0, 100, 0, 90);
    	
    	 DIRECTION.write(90);
    	 MOTOR1.write(ANG_M1-vel);
		 MOTOR2.write(ANG_M2-vel);
		 MOTOR3.write(ANG_M3-vel);

		 MOTOR4.write(ANG_M4-vel);
		 MOTOR5.write(ANG_M5-vel);
		 MOTOR6.write(ANG_M6-vel);
	 break;
     case 0x0E://rotate rigth 100%
    	vel3=map(command[2], 0, 100, 0, 90);
    
    	 DIRECTION.write(90);
    	 MOTOR1.write(ANG_M1+vel3);
		 MOTOR2.write(ANG_M2+vel3);
		 MOTOR3.write(ANG_M3+vel3);

		 MOTOR4.write(ANG_M4+vel3);
		 MOTOR5.write(ANG_M5+vel3);
		 MOTOR6.write(ANG_M6+vel3);;

    break;
    case 0x17:
    moveServo(180,CAM1_R);
    delay(100);
    moveServo(90,CAM1_R);
    delay(100);

    moveServo(0,CAM1_A);
    delay(100);
    moveServo(120,CAM1_A);
    delay(50);
	   
    break;	

	case 0x18:
	moveServo(command[2],CAM1_A);
	
	  break; 
	case 0x19:  // Servo Write
		
			moveServo(command[2],MICROSP);

		
	  // MICROSP.write(command[2]);
	  break;
	
	case 0x1A:  // stop all
			DIRECTION.write(0);
			MOTOR6.write(ANG_M6);
			MOTOR5.write(ANG_M5);
			MOTOR4.write(ANG_M4);
			MOTOR3.write(ANG_M3);
			MOTOR2.write(ANG_M2);
			MOTOR1.write(ANG_M1);      
			break;
	 
	////////////////////////////////////////////////////////    
	  
	default:      
	  Serial.flush();
	  break;     
	}
  }
  else{  
	
	Serial.flush(); 
  }   
}


void Sync()
{
  Serial.begin(DEFAULTBAUDRATE); 
  Serial.flush();
}

/*unsigned char checksum_Compute(unsigned char command[])
{
  unsigned char checksum;
  for (int i=0; i<(COMMANDLENGTH-1); i++)
  {
	checksum += command[i]; 
  }
  return checksum;
}

int checksum_Test(unsigned char command[])
{
  unsigned char checksum = checksum_Compute(command);
  if(checksum == command[COMMANDLENGTH-1])
  {
	return 0; 
  }
  else
  {
	return 1;
  }
}*/
void moveSensor(int angle)
{
    int valActu=SHARP_SENSOR.read(); 
    if (valActu>angle){
        while (valActu!=angle){
            valActu=valActu-1;
            SHARP_SENSOR.write(valActu);
            valActu=SHARP_SENSOR.read();
            delay(25);
        }
    }   
    else
    { 
        if (valActu<angle){
             while (valActu!=angle){
                valActu=valActu+1;
                SHARP_SENSOR.write(valActu); 
                valActu=SHARP_SENSOR.read();
                delay(25);
             }
        }
    }
}
void moveServo(int angle,Servo ser)
{
    int valActu=ser.read(); 
    if (valActu>angle){
        while (valActu!=angle){
            valActu=valActu-1;
            ser.write(valActu);
            valActu=ser.read();
            delay(25);
        }
    }   
    else
    { 
        if (valActu<angle){
             while (valActu!=angle){
                valActu=valActu+1;
                ser.write(valActu); 
                valActu=ser.read();
                delay(25);
             }
        }
    }
}






