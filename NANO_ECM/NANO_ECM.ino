#include  <Wire.h>

int EGTsense = A2; //Analog EGT reading needs calibration, sim with trimpot
int FPpin = 9;     //Fuel Pump Run pin
int RPMsense = 5;  //RPM sense input pin
int STRpin = 6;    //Starter Run Pin
int FVpin = 8;     //Fuel Valve enable / disable
volatile int RPMcmdin = 0; //Command input from ECU, used in Start/Run functions

void setup() {
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent); // register event
  pinMode(FPpin, OUTPUT);
  pinMode(STRpin, OUTPUT);
  pinMode(EGTsense,INPUT);
  pinMode(RPMsense, INPUT_PULLUP);
  pinMode(FVpin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  //RUNstarter(int); FUELvalve(int); RUNfuel(int)
  //StartSEQ();
  RUNstarter(100);
  delay(5000);
  RUNstarter(150);
  delay(5000);
  //ShutdownSEQ();
  RUNstarter(0);
  delay(30000);
  
}

// Collects and sends RPM, EGT, CMD values from ECM to ECU
void requestEvent() {
  int rpm = GetRPM();  
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
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    if (c == 's'){
      StartSEQ();
    }
  }

  RPMcmdin = Wire.read();    // receive byte as an integer
  analogWrite(FPpin, RPMcmdin);      
}

// Rough start sequence, needs work with fuel pump and handoff to RUN function
void StartSEQ(){
  //RUNstarter(int); FUELvalve(int); RUNfuel(int);
  RUNstarter(77); //Start the engine at 2500 RPM
  FUELvalve(1); //This will be the pilot light fuel solenoid on the real system.
  RUNfuel(15); // Start the fuel pump to push just a little fuel through the ignition system
  int egt = GetEGT(); //Read EGT 
  for (int i = 0; i<100;i++){
      if(egt>120){
        FUELvalve(1); //This will be the main fuel valve for the injectors
        RUNstarter(120);
          for(int j = 15; j < 100; j++){ //ramp up the fuel pump slowly
            RUNfuel(j);
            delay(50); //ramp rate delay
          }
          RUNfuel(100);
          RUNstarter(70);          
          delay(5000);
      }else{
        delay (100);
        egt = GetEGT(); //Read EGT // if EGT <120 read again 
      }
  }
}

void ShutdownSEQ(){
  FUELvalve(0); //Shut the fuel valve
  RUNfuel(0);   //Turn off the fuel pump
  int egt = GetEGT(); //Read EGT 
  if(egt>120){
    RUNstarter(77); 
    delay(2000);
    RUNstarter(0);
    delay(2000); 
    ShutdownSEQ();
  }else{
    RUNstarter(0);
    return;
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

// Run values need to be calibrated at 8.4V, need min 10A supply.
int RUNstarter(int STRin){
  //2500RPM = 77PWM, 5000RPM = 120PWM Heating of motor is a concern at 12.6V
  analogWrite(STRpin, STRin);
}

int RUNfuel(int FP){
  //Runs on 12.6V now, consider lower voltage for better resolution
  analogWrite(FPpin, FP);
}

int FUELvalve(int pos){
  //Pass in value of 1 opens valve, 0 closes valve.
  if (pos == 1){
    digitalWrite(FVpin, HIGH);
  }else{
    digitalWrite(FVpin, LOW);
  }
}
