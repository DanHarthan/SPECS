#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
volatile int RUNStatus = 0; // Global volatile system run status. 0 = OFF, 1 = START, 2 = RUN, 3 = SHUTDOWN

void setup(){
  Wire.begin();        // join i2c bus (address optional for master)
  lcd.init();          // initialize the lcd 
  lcd.backlight();
  Serial.begin(9600);
}

void loop(){
  char RPMcmd;
  if(Serial.available()>0){
    while(Serial.available()>0){
      RPMcmd = Serial.read();
    
      WriteData(RPMcmd);
      Serial.print(RPMcmd);
    }
  }
  ReadDATA();
  delay(200);
}

int WriteData(int x){
  byte CMDarray[2];
  CMDarray[0] = (x >> 8) & 0xFF;
  CMDarray[1] = x & 0xFF;
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(CMDarray,2); 
  Wire.endTransmission();    // stop transmitting
}

//Requests parameter data from the ECU. Min 20ms response time a normal I2C freq. 
void ReadDATA(){
  Wire.requestFrom(8, 5);    // request 5 bytes from slave device #8
  while (Wire.available()) {  // slave may send less than requested
    byte rpmH = Wire.read();  
    byte rpmM = Wire.read(); 
    byte rpmL = Wire.read(); 
    int egt = Wire.read(); 
    int cmd = Wire.read();
    unsigned long rpm = rpmH;
    rpm  = (rpm<<8)|rpmM;
    rpm  = (rpm<<8)|rpmL;    
    // In RPM will work for 500 - 99999 RPM (get negative values below 500 due to data rollover)
    //(will switch to KRPM later, better resolution for calibration ***need to redo after sensor move***)
    rpm = 26086956/rpm; // value compensation for microsec & duty cycle compressed data format 26086956
    LCD(rpm,egt,cmd);
  }
}

// Write data to LCD in fomat for 16 x 2 LCD
void LCD(int rpm, int egt, int cmd){
  lcd.setCursor(0,0);
  lcd.print("RPM:");
  lcd.setCursor(4,0);
  lcd.print("     ");
  lcd.setCursor(4,0);
  lcd.print(rpm);
  lcd.setCursor(0,1);
  lcd.print("EGT:");
  lcd.setCursor(4,1);
  lcd.print("   ");
  lcd.setCursor(4,1);
  lcd.print(egt);
  lcd.setCursor(9,0);
  lcd.print("CMD:");
  lcd.setCursor(13,0);
  lcd.print("   ");
  lcd.setCursor(13,0);
  lcd.print(cmd);
}


