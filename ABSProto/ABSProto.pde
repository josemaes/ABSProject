#include <ABSStateMachineImp.h>

#include "nvic.h"
#include <InvServo.h>
#include <FLBuffer.h>
#include <Servo.h>
#include <SWheel.h>


// SD inclues 
#include <SdFat.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* Timer channels:                1 - 2 - 3 - 4*/
/* Pins used for the PWM outputs: 5,  9,  14, 24 */
/* Pins used for the PWM Throttle: 6 */
                         /* Extis : 0, 1, 2, 3*/
/* Timer channels:                  1 - 2 - 3 - 4*/
/* Pins used for the Sensor inputs: 2,  3,  1,  0 */

 /*buffer*/
FLBuffer memBuffer;
 /*buffer*/

/* Timer 50ms */
HardwareTimer  timerWheel(2);

char cadena[200];
unsigned long indexLoop=0;
unsigned char flagStore=0;
unsigned char numLines=0;
/* END SD variables */
unsigned int toggle = 0;
uint8 counter=0;
uint8 wheelCounter=0;

/*pwm stuff*/
 volatile uint32 pwmInputCh1 = 1470; // servo value 
 volatile uint32 pwmStepCh1 = 0; // temporary variable for servo1 
 volatile uint32 degreeSCh1 = 0;
 volatile uint32 preValue = 0;

 volatile uint32 pwmInputCh2 = 1470; // servo value 
 volatile uint32 pwmStepCh2 = 0; // temporary variable for servo1 
 volatile uint32 degreeSCh2 = 0;
 volatile uint32 preValue2 = 0;
/* pwm stuff */

/* SD variables */
char initialicedSD = 0;
Sd2Card card;
SdVolume volume;
SdFile root;
SdFile file;
/* END SD variables */

SWhell WheelFR(FrontRight, 10);
SWhell WheelFL(FrontLeft, 10);
SWhell WheelRL(RearRight, 10);
SWhell WheelRR(RearLeft, 10);

/* servos */
InvServo myServoFR;
InvServo myServoFL;
InvServo myServoRR;
InvServo myServoRL;

Servo throttleServo;
/* servos */

volatile boolean mutex = false;
uint8 flagWriteLine = 0;

volatile boolean calculeABS = false;

volatile boolean timerAction = false;
volatile uint8 FRAction = false;
volatile uint8 FLAction = false;
volatile uint8 RRAction = false;
volatile uint8 RLAction = false;

/* ABS objects */
 /*Servo myServo; */
ABSStateMachineImp FRMachine;
ABSStateMachineImp  FLMachine;
ABSStateMachineImp  RRMachine;
ABSStateMachineImp  RLMachine;
/* ABS objects */

void setup()
{
  FRMachine = ABSStateMachineImp(); 
  FLMachine = ABSStateMachineImp();
  RRMachine = ABSStateMachineImp();
  RLMachine = ABSStateMachineImp();
  //SerialUSB.begin();

  card = Sd2Card();
  ////SerialUSB.println("Buffer init");
  memBuffer = FLBuffer();
  //SerialUSB.println("Buffer initialized");
  delay(300);
  
  if (!card.init()) 
  {
    //SerialUSB.println("card.init failed");
    initialicedSD = 0;
  }
  else
  {
    //SerialUSB.println("card.init passed");
    initialicedSD = 1;
  }
  if (initialicedSD==1)
  {
/*    spi.begin(SPI_4_5MHZ, MSBFIRST, 0);  
    delay(100);*/
    // initialize a FAT volume
    if (!volume.init(&card,1)) 
    {
      //SerialUSB.println("volume.init failed");
      initialicedSD = 0;
    }
    else
    {
      //SerialUSB.println("volume.init passed");
      initialicedSD = 1;
    }
    if (!root.openRoot(&volume)) 
    {
      //SerialUSB.println("openRoot failed");
      initialicedSD = 0;
    }
    else
    {
      initialicedSD = 1;
      //SerialUSB.println("openRoot passed");
    }
    if (file.open(&root, "logFile.csv", O_CREAT | O_WRITE | O_APPEND)) 
    {
      //SerialUSB.println("Open file success");
          
      sprintf(cadena, "FrontRight;FrontLeft;RearRight;RearLeft;Channel1;Channel2;FrontRightABS;FrontLeftABS;RearRightABS;RearLeftABS;\r\n");
      memBuffer.addString((unsigned char*)cadena, (unsigned short)strlen(cadena));
      initialicedSD = 1;
    }
    else
    {
      //SerialUSB.println("Open file failed");
      initialicedSD = 0;
    }
  }
  
   // Pause the timer while we're configuring it
  timerWheel.pause();
  
  /* sensor input configuration */
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(1, INPUT);
  pinMode(0, INPUT);
  attachInterrupt(2, onPulseFR, RISING);
  attachInterrupt(3, onPulseFL, RISING);
  attachInterrupt(1, onPulseRR, RISING);
  attachInterrupt(0, onPulseRL, RISING);
  
  myServoFR.attach(5, SERVO_DEFAULT_MIN_PW, SERVO_DEFAULT_MAX_PW, 10, 170);
  myServoFL.attach(9, SERVO_DEFAULT_MIN_PW, SERVO_DEFAULT_MAX_PW, 10, 170);
  myServoRR.attach(14, SERVO_DEFAULT_MIN_PW, SERVO_DEFAULT_MAX_PW, 10, 170);
  myServoRL.attach(24, SERVO_DEFAULT_MIN_PW, SERVO_DEFAULT_MAX_PW, 10, 170);
  throttleServo.attach(6, SERVO_DEFAULT_MIN_PW, SERVO_DEFAULT_MAX_PW, 10, 170);
  myServoFR.write(180);
  myServoFL.write(0);
  myServoRR.write(180);
  myServoRL.write(0);
  throttleServo.write(90);
  /* Start the timer counting */  
  /* FAT configuration and file opening */
  
  /* pwm inputs */ 
  pinMode(8, INPUT_PULLUP); /* EXTI 5 */
  attachInterrupt(8,handleInterruptCh1,CHANGE);
  pinMode(4, INPUT_PULLUP);  /* EXTI 10 */
  attachInterrupt(4,handleInterruptCh2,CHANGE);
 /* pinMode(7, INPUT);
  attachInterrupt(7,handleInterruptCh3,CHANGE);*/
  /* pwm inputs */
  
  /* Extis : 0, 1, 2, 3*//* 5 - 10 */

/* Set up period */
  timerWheel.setPeriod(T1MILIS); /* in microseconds */
  /* Set up an interrupt on channel 1 */
  timerWheel.setChannel1Mode(TIMER_OUTPUT_COMPARE);
  timerWheel.setCompare(TIMER_CH1, 2);  /* Interrupt 1 count after each update */
  timerWheel.attachCompare1Interrupt(localTimer);
  timerWheel.refresh();
  timerWheel.resume();
  //SerialUSB.println("Setup finished");

  //SerialUSB.end();
  myServoFR.write(90);
  myServoFL.write(90);
  myServoRR.write(90);
  myServoRL.write(90);
}

void localTimer()
{
  timerAction = true;
}

void onPulseFR(void)
{
  //FRAction++;
  WheelFR.onPulse(micros());
}

void onPulseFL(void)
{
  //FLAction++;
  WheelFL.onPulse(micros());
}

void onPulseRR(void)
{
  //RRAction++;
  WheelRR.onPulse(micros());
}

void onPulseRL(void)
{
  //RLAction++;
  WheelRL.onPulse(micros());
}
/* pwm stuff */
 void handleInterruptCh1() 
 {
   uint32 temp = micros();
   mutex=true;
   if(digitalRead(8)== HIGH)
   {     
     pwmStepCh1=temp; // we got a positive edge 
   }
   else 
   {
     preValue = pwmInputCh1;
     if(pwmStepCh1<temp)
     {
       pwmInputCh1=temp-pwmStepCh1;
     }else
     {
       pwmInputCh1=0xFFFFFFFF - pwmStepCh1 + temp;
     }
     if ((pwmInputCh1 < 2400)&&(pwmInputCh1 > 0))
       degreeSCh1 = map(constrain(pwmInputCh1, 540, 2400), 540, 2400, 10, 170);  
   }
   mutex=false;
 }
 
 /* pwm staff */
 void handleInterruptCh2() 
 { 
   uint32 temp2 = micros();
   mutex=true;
   if(digitalRead(4)== HIGH)
   {
     pwmStepCh2=temp2; // we got a positive edge 
   }
   else 
   {
     preValue2 = pwmInputCh2;
     if(pwmStepCh2<temp2)
     {
       pwmInputCh2=temp2-pwmStepCh2;
     }else
     {
       pwmInputCh2=0xFFFFFFFF - pwmStepCh2 + temp2;
     }
     if ((pwmInputCh2 < 2400)&&(pwmInputCh2 > 0))
       degreeSCh2 = map(constrain(pwmInputCh2, 540, 2400), 540, 2400, 10, 170);
   }
   mutex=false;
 }
 
/* void handleInterruptCh3() 
 { 
   long temp3 = micros();
   mutex=true;
   if(digitalRead(7)== HIGH)
   {
     pwmStepCh3=temp3; // we got a positive edge 
   }
   else 
   {
     preValue3 = pwmInputCh3;
     if(pwmStepCh3<temp3)
     {
       pwmInputCh3=temp3-pwmStepCh2;
     }else
     {
       pwmInputCh3=0xFFFFFFFF - pwmStepCh3 + temp3;
     }
     if (pwmInputCh3 < 2400)
       degreeSCh3 = map(constrain(pwmInputCh3, 540, 2400), 540, 2400, 0, 180);
   }
   mutex=false;
 }*/
/*pwm stuff*/

void writeLine(void)
{/* writes a line in the file, which must be open */     
    if(!mutex)
    {
      mutex=true;
      sprintf (cadena, "%lf;", WheelFR.getMeanSpeed());
      sprintf (cadena, "%s%lf;", cadena, WheelFL.getMeanSpeed());
      sprintf (cadena, "%s%lf;", cadena, WheelRR.getHalfSpeed());
      sprintf (cadena, "%s%lf;", cadena, WheelRL.getInstantSpeed());
      sprintf (cadena, "%s%d;", cadena, degreeSCh1);
      sprintf (cadena, "%s%d;", cadena, degreeSCh2);
      sprintf (cadena, "%s%d;", cadena, FRMachine.state);
      sprintf (cadena, "%s%d;", cadena, FLMachine.state);
      sprintf (cadena, "%s%d;", cadena, RRMachine.state);
      sprintf (cadena, "%s%d;\r\n", cadena, RLMachine.state);
      memBuffer.addString((unsigned char*)cadena, (unsigned short)strlen(cadena));
      mutex=false;
    }
}

void flushMemBuffer()
{
  char res = 0;
    if(file.isOpen())
    {
      res = file.write(memBuffer.getMemBuffer(), BUFSIZE);
      memBuffer.fullBuff = 0;
      if(res != -1)
      {
        if(toggle==1)
        {
          toggle = 0;
          //digitalWrite(BOARD_LED_PIN, HIGH);
        }else
        {
          toggle = 1;
          //digitalWrite(BOARD_LED_PIN, LOW);
          //file.sync();
        }
      }else
      {
        //SerialUSB.println("Write error en main");
        file.writeError = 0;
        //file.close();
      }
    }else      
      flagStore=0;
}

void loop()
{  
  if(timerAction == true)
  {
    /*  The timer signals every 1ms
        each signal of the timer, only 1 the speed of one wheel is calculated so,
        each wheel is updated every 4ms in stair way mode.  */
    timerAction = false;
    
    if(wheelCounter==0)
      WheelFR.handlerTime();
      
    if(wheelCounter==1)
      WheelFL.handlerTime();
    
    if(wheelCounter==2)
      WheelRR.handlerTime();
    
    if(wheelCounter==3)
      WheelRL.handlerTime();
    counter++;
    wheelCounter++;
    
    if(wheelCounter==4)
    {
      /*  Every 4ms the throtle/brake servo action is updated.  */
      wheelCounter = 0;
      calculeABS = true;
    }
    if(counter==100) /* Write a line to the log buffer every 100ms */
    {
      //writeLine();
      counter=0;
    }
    if(counter%10==0) /*  10 ms  */
    {
      if((degreeSCh1>90)&&(degreeSCh1<170))
      {
        myServoFR.write(FRMachine.getBreakLevel());
        myServoRL.write(RLMachine.getBreakLevel());
         /* Inverted servos */
        myServoFL.write(90 - (FLMachine.getBreakLevel() - 90));
        myServoRR.write(90 - (RRMachine.getBreakLevel() - 90));
        throttleServo.write(95);
      }else
      {
        throttleServo.write(degreeSCh1);
        myServoFR.write(85);
        myServoFL.write(95);
        myServoRR.write(95);
        myServoRL.write(85);
      }
      writeLine();
    }
      
  }else
  {/* In case the timer did not signal this loop, ABS is calculated */
    if(calculeABS==true)
    {
      /*  Calculates the status of the ABS state machine according to the wheel speed and throtle/brake degree  */
      /*  This calculation is done every 10ms  */
      FRMachine.calulateState(WheelFR.getHalfSpeed(), degreeSCh1);
      FLMachine.calulateState(WheelFL.getQuarterSpeed(), degreeSCh1);
      RRMachine.calulateState(WheelRR.getInstantSpeed(), degreeSCh1);
      RLMachine.calulateState(WheelRL.getInstantSpeed(), degreeSCh1);    
      calculeABS=false;
    }else
    {
      /*  The buffer is only written to SD when the currently used buffer is full and nothing was done in this loop aready  */
      if(memBuffer.fullBuff == 1)
      {
        /*  Actually writes de buffer to the SD and empties the buffer  */
        flushMemBuffer();
        /*  Commits the content of the file to SD  */
        file.sync();
      }
    }
  }
  
}
