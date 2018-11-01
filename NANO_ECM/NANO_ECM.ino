#include  <Wire.h>

int EGTsense = A2; //Analog EGT reading needs calibration, sim with trimpot
int FPpin = 3;  //Fuel Pump Run pin
int RPMsense = 5;  //RPM sense input pin
int STRpin = 9;    //Starter Run Pin
int FVpin = 8;     //Fuel Valve enable / disable
int IGpin = 12;
volatile int RPMcmdin = 0; //Command input from ECU, used in Start/Run functions
volatile int RUNStatus = 0; // Global volatile system run status. 0 = OFF, 1 = START, 2 = RUN, 3 = SHUTDOWN

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent); // register event
  pinMode(FPpin, OUTPUT);
  pinMode(STRpin, OUTPUT);
  pinMode(EGTsense,INPUT);
  pinMode(RPMsense, INPUT_PULLUP);
  pinMode(FVpin, OUTPUT);
  pinMode(IGpin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  //Starter(int); FUELvalve(int); FuelPump(int); Ignition(int)
  //StartSEQ(); ShutdownSEQ();
  //StartSEQ();
  //delay(30000);
  RUNStatus = 0;
}

// Collects and sends RPM, EGT, CMD values from ECM to ECU
void requestEvent() {
  unsigned long rpm = GetRPM();  
  byte RPMarray[3];
  RPMarray[0] = (rpm >> 16) & 0xFF;
  RPMarray[1] = (rpm >> 8) & 0xFF;
  RPMarray[2] = rpm & 0xFF;
  int egt = GetEGT();
  int cmd = RPMcmdin;
  Wire.write(RPMarray,3); 
  Wire.write(egt); 
  Wire.write(cmd);
}

// Receive RPM or Start command from ECU, Start or change program RPM setpoint
void receiveEvent(int howMany) {
  Serial.print("Run Status:");
  Serial.print(RUNStatus);
  Serial.print("CMD in:");
  Serial.print(RPMcmdin);
  int RPMcmdinH = Wire.read();    // receive byte as an integer
  int RPMcmdinL = Wire.read();    // receive byte as an integer
  RPMcmdin = (RPMcmdinH<<8)|RPMcmdinL;
  if (RUNStatus == 0){
      StartSEQ(); 
  }else if (RUNStatus == 2){
      analogWrite(STRpin, RPMcmdin);    //will change to fuel pin for real operation 
  }
}

// Rough start sequence, needs work with fuel pump and handoff to RUN function
void StartSEQ(){
  RPMcmdin = 82;
  Starter(82);
  delay(10000);
  Starter(60);
  delay(5000);
  FUELvalve(1);
  Ignition(1);
  //FuelPump(20);
  delay(5000);
  EngineRUN();
}

void ShutdownSEQ(){
  FUELvalve(0); //Shut the fuel valve
  FuelPump(0);   //Turn off the fuel pump
  int egt = GetEGT(); //Read EGT 
  if(egt>120){
    Starter(85); 
    delay(5000);
    Starter(0);
    delay(5000); 
    ShutdownSEQ();
  }else{
    Starter(0);
    FuelPump(0);
    FUELvalve(0);
    Ignition(0);
    return;
  }
}

void EngineRUN(){
  if (RPMcmdin == 0){
    ShutdownSEQ();
  }else if (RUNStatus == 2){
   Starter(RPMcmdin);
   //FuelPump(20);
 }
}

// Only measures HIGH pulse width and sends value back to ECU for computation. 
// This requires 3 bytes, and is minimum load on processor.
int GetRPM(){
  unsigned long PWH = pulseIn(RPMsense,HIGH);
  return(PWH);
}

// Reads Thermocouple value. Will need calibration with real sensor at some point.
// If using k-type, use linear mapping requires component selections and testing
int GetEGT(){
  int measure = analogRead(EGTsense);
  int egt = map(measure, 0, 1023, 0, 255);
  return (egt);
}

// Run values need to be calibrated at 8.4V, need min 11.4A supply.
int Starter(int STRin){
  //2500RPM = 82PWM, 5000RPM = ???PWM Need to verify on Battery 
  analogWrite(STRpin, STRin);
  return;
}

int FuelPump(int FP){
  analogWrite(FPpin, FP);
  return;
}

int FUELvalve(int pos){
  //Pass in value of 1 opens valve, 0 closes valve.
  if (pos == 1){
    digitalWrite(FVpin, HIGH);
  }else{
    digitalWrite(FVpin, LOW);
  }
}

int Ignition(int i){
  if (i == 1){
    digitalWrite(IGpin, HIGH);
  }else{
    digitalWrite(IGpin, LOW);
  }
}

