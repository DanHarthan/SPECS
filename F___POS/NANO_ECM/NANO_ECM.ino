
#include <Servo.h>  //BLDC operations -> 50Hz pwm generation ESC input

volatile byte command = 0;
void ss_falling (){  // start of SPI transaction, no command yet
  command = 0;
}  // end of interrupt service routine (ISR) ss_falling

//PINS
const uint8_t RPMsense = 2;  //RPM sense input pin
const uint8_t FPpin = 6;     //Fuel Pump Run pin
const uint8_t IGpin = 9;     //Glow plug / ignition pin enable / disable
const uint8_t FVpin = 8;     //Fuel Valve enable / disable

//Global Variables
volatile uint16_t RPM = 0;
volatile uint8_t RPMcmdin = 0;       //Command input from ECU, used in Start/Run functions
volatile uint8_t RUNStatus = 0; // Global volatile system run status. 0 = OFF, 1 = START, 2 = RUN, 3 = SHUTDOWN
volatile uint8_t rpm = 0; 
//const uint8_t modeT2 = 0x02;  
const uint8_t modeT1 = 0x02; //PWM Freq Scale 16B P
//const uint8_t modeT0 = 0x02; //PWM Freq Scale 8B P87 T2 BASE =  PS(1, 8, 64, 256, 1024)
int value = 0; // servo program value for ESC. 1000 SD, 1200 Stop, 2000 MAX
Servo StarterCON; // Brushless motor instance

void setup() {
  //SPI setup
  pinMode(MISO, OUTPUT);  // have to send on master in, *slave out*
  SPCR |= _BV(SPE);  // turn on SPI in slave mode
  SPCR |= _BV(SPIE);  // turn on interrupts
  attachInterrupt (0, ss_falling, FALLING);  // interrupt for SS falling edge
  
  //Pins
  pinMode(FPpin, OUTPUT);
  pinMode(RPMsense, INPUT_PULLUP);
  pinMode(FVpin, OUTPUT);
  pinMode(IGpin, OUTPUT);

  //PWM Change Frequency
  StarterCON.attach(5);    // Pin 5 ESC control for Brushless Motor
  //TCCR2B = TCCR2B & 0b11111000 | modeT2;//Pin 3 and 11 PWM freq
  TCCR1B = TCCR1B & 0b11111000 | modeT1;//Pin 9 and 10 PWM freq
  //TCCR0B = TCCR0B & 0b11111000 | modeT0; //Pin 5 and 6 PWM Freq
  Serial.begin(115200);

}

void loop() {
  //Starter(int); FUELvalve(int); FuelPump(int); Ignition(int)
  //StartSEQ(); ShutdownSEQ();
  if (Serial.available()>0){
    RPM = Serial.parseInt();
    if(RPM ==1){
      Starter(1000);
      StartSEQ();
    }else{
      ShutdownSEQ();
    }
  }
  //Starter(RPM);
  //Ignition(1);
  //if(
  //}
  GetRPM();
  //
  Serial.print("CMD:");
  Serial.println(command);
  Serial.print("RPMcmdin:");
  Serial.println(RPMcmdin);
  delay(100);
}

// Rough start sequence, needs work with fuel pump and handoff to RUN function
void StartSEQ(){

  Starter(1240);
  FUELvalve(1);
  Ignition(1);
  FuelPump(50);
  GetRPM();
  
  return;
}

void ShutdownSEQ(){
  Starter(1000);
  FUELvalve(0);
  Ignition(0);
  FuelPump(0);

  return;
}

void EngineRUN(){
  return;
}

// RPM Sense 
int GetRPM(){
  unsigned long PWH = pulseIn(RPMsense,HIGH);
  rpm = 249285/PWH; // rpm:24928570 kRPM 24928
  return(rpm);
}//END RPM Sense 

//Starter Speed Control 
uint16_t Starter(uint16_t STRin){
  StarterCON.write(STRin);
  //Serial.println(STRin);
  return;
}//END Starter Speed Control

//Fuel Pump Control
int FuelPump(int FP){
  analogWrite(FPpin, FP);
  return;
} //END Fuel Pump Control

//Fuel Valve Control function 1 opens valve, 0 closes valve.
int FUELvalve(int pos){
  if (pos == 1){
    digitalWrite(FVpin, HIGH);
  }else{
    digitalWrite(FVpin, LOW);
  }
  return;
}//END Fuel Valve Control

//Igniter Control function 1 on, 0 off.
int Ignition(int i){
  if (i == 1){
    analogWrite(IGpin, 35);
  }else{
    analogWrite(IGpin, 0);
  }
  return;
}//END Igniter Control

// SPI ISR
ISR (SPI_STC_vect){
  int c = SPDR;
  //RPMcmdin = c;
  switch (command){
  // no command? then this is the command
  case 0:
    command = c;
    SPDR = 0;
    //RPMcmdin = 0;
    break;

  case 'a': // Get RPM value and return
    SPDR = c + rpm;  
    break;
    
  case 'd': // Save a new RPM command value

    RPMcmdin = c;
    SPDR = RPMcmdin;
    break;   


  } // end of switch
}  //END(ISR) SPI_STC_vect











//#include  <Wire.h>  //I2C communciations

  //I2C initialization
  //Wire.begin(8);                // join i2c bus with address #8
  //Wire.onRequest(requestEvent); // register event
  //Wire.onReceive(receiveEvent); // register event

// Collects and sends RPM, EGT, CMD values from ECM to ECU
//void requestEvent() {
  //unsigned long rpm = GetRPM();  
  //byte RPMarray[3];
  //RPMarray[0] = (rpm >> 16) & 0xFF;
  //RPMarray[1] = (rpm >> 8) & 0xFF;
  //RPMarray[2] = rpm & 0xFF;
  //byte EGTarray[3];
  //int egt = GetEGT();
  //EGTarray[0] = (egt >> 8) & 0xFF;
  //EGTarray[1] = egt & 0xFF;
  //int cmd = RPMcmdin;
  //Wire.write(RPMarray,3); 
  //Wire.write(EGTarray,2); 
  //Wire.write(cmd);
  //Wire.write(RUNStatus);
  //return;
//}


// Receive RPM or Start command from ECU, Start or change program RPM setpoint
//void receiveEvent(int howMany) {
  //int RPMcmdin = Wire.read();    // receive byte as an integer
  //value = RPMcmdin*10;
  //Starter(value);
  //Serial.print("RPMcmdin:");
  //Serial.println(value);
//}

