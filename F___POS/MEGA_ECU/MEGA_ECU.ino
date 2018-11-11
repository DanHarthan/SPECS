#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SparkFunMAX31855k.h> // Using the max31855k driver

LiquidCrystal_I2C lcd(0x3F,16,2);  //LCD address to 0x3f for a 16 char 2 line display

int Vpin = A8;
const uint8_t TEMPSS = 24;
const uint8_t VCC = 2;       // NOT USED, Function Place Holder
const uint8_t GND = 4;       // NOT USED, Function Place Holder
volatile uint8_t cmd = 0;


//Subinitialazation Funcitons
SparkFunMAX31855k probe(TEMPSS, VCC, GND);
SPISettings settingsA(2000000, MSBFIRST, SPI_MODE1); 

void setup(){
  SPI.begin ();
  //SPI.setClockDivider(SPI_CLOCK_DIV8);
  pinMode(SS,OUTPUT);
  pinMode(TEMPSS, OUTPUT);
  digitalWrite(SS, HIGH);  // ECM Slave Select Pin
  digitalWrite(TEMPSS, HIGH); // TEMP Slave Select Pin
  
  Wire.begin();        // join i2c bus (address optional for master)
  lcd.init();          // initialize the lcd 
  lcd.backlight();
  pinMode(Vpin, INPUT);
  Serial.begin(115200);


}

void loop(){
if (Serial.available()>0){
  cmd = Serial.parseInt();
  WriteDATA(cmd);
   
} 
ReadDATA();
delay(200);
}

byte WriteDATA(byte cmdin){
  digitalWrite(SS, LOW); // enable Slave Select
  SPITransfer ('d'); 
  SPITransfer (cmdin);
  int c = SPITransfer (0);
  SPITransfer (0);
  digitalWrite(SS, HIGH);  // disable Slave Select 
  Serial.println(c);
}

//Requests parameter data from the ECU. Min 20ms response time a normal I2C freq. 
void ReadDATA(){
  digitalWrite(SS, LOW); // enable Slave Select  
  SPITransfer ('a');  // add command
  SPITransfer (1);
  byte rpm = SPITransfer (1);
  SPITransfer (0);
  digitalWrite(SS, HIGH);  // disable Slave Select
  int RS = 1;
  int egt = GetEGT();
  int Vbat = Volt();
  LCD(rpm,egt,cmd,Vbat,RS);
}

//SPI transfer byte
byte SPITransfer (const byte dat){
  //SPI.beginTransaction(settingsA);
  byte a = SPI.transfer (dat);
  delayMicroseconds (20);  
  //SPI.endTransaction();
  return a;
} //END SPITransfer

// Read Thermocouple 
int GetEGT(){
  float temperature = probe.readCJT();
  temperature = probe.readTempC();
  return (temperature);
  return;
}//END Reads Thermocouple 

//Read Voltage Value
int Volt(){
  //This needs to be worked on Cell Value, implemented for testing
  int Vin = analogRead(Vpin);
  int Vout = map(Vin, 0,1023,0,100);
  return(Vout);
}//END Read Voltage Value

// Write data to LCD in fomat for 16 x 2 LCD
void LCD(int rpm, int egt, int cmd, int vlt, byte RS){
  lcd.setCursor(0,0);
  lcd.print("RPM:     CMD:   ");
  lcd.setCursor(0,1);
  lcd.print("EGT:     BAT:   ");
  lcd.setCursor(4,0);
  lcd.print(rpm);
  lcd.setCursor(13,0);
  lcd.print(cmd);
  lcd.setCursor(4,1);
  lcd.print(egt);
  lcd.setCursor(8,1);
  lcd.print(RS);
  lcd.setCursor(13,1);
  lcd.print(vlt);
}//END Write LCD





//int WriteData(int x){
//  Wire.beginTransmission(8); // transmit to device #8
//  Wire.write(x); 
//  Wire.endTransmission();    // stop transmitting
//}
//Wire.requestFrom(8, 7);    // request 5 bytes from slave device #8
//  while (Wire.available()) {  // slave may send less than requested
//    byte rpmH = Wire.read();  
//    byte rpmM = Wire.read(); 
//    byte rpmL = Wire.read(); 
//    int egtH = Wire.read();
//    int egtL = Wire.read();  
//    int cmd = Wire.read();
//    byte RS = Wire.read();
//   int egt = (egtH<<8)|egtL;
//    unsigned long rpm = rpmH;
//    rpm  = (rpm<<8)|rpmM;
//    rpm  = (rpm<<8)|rpmL;    
//    // In RPM will work for 500 - 99999 RPM (get negative values below 500 due to data rollover)
//    //(will switch to KRPM later, better resolution for calibration ***need to redo after sensor move***)
//    rpm = 24928570/rpm; // value compensation for microsec & duty cycle compressed data format 23809523
    
    //Serial.println(rpm);
//  }



