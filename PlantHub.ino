
#include <Wire.h>
#include "rgb_lcd.h"

#define SoilMoisturePin A2
#define SoilMoisturePower 6
#define LightSensorPin A0
#define TemperatureSensorPin A3

rgb_lcd lcd;
bool flag1 = LOW;
byte x;

const int wetThreshold = 400;
const int dryThreshold = 250;
const int R0 = 100000;            // R0 = 100k
const int B = 4275;               // B value of the thermistor

uint8_t water[8] = {
  0b00100,
  0b00100,
  0b01010,
  0b01010,
  0b10001,
  0b10001,
  0b01010,
  0b00100
};

uint8_t sun[8] = {
  0b00000,
  0b10101,
  0b01110,
  0b11011,
  0b01110,
  0b10101,
  0b00000,
  0b00000
};

uint8_t temp[8] = {
  0b00100,
  0b01010,
  0b01110,
  0b01010,
  0b01110,
  0b10001,
  0b10001,
  0b01110
};


int readSoil() {
  digitalWrite(SoilMoisturePower, HIGH);//turn monitoring "On"
  delay(10);//wait 10 milliseconds 
  int val = analogRead(SoilMoisturePin);//Read the SIG value form sensor 
  digitalWrite(SoilMoisturePower, LOW);//turn monitoring "Off"
  return val;//send current moisture value
}

void receiveEvent(int howmany){  
  x = Wire.read();  
  flag1 = HIGH;  
  //Serial.print(x, HEX);
}

void sendEvent(int howmany){  
  Wire.write(0x45);
}

void setup() {
  Serial.begin(115200);  
  Wire.begin(0x08);  
  pinMode(13, OUTPUT); 
  Wire.onReceive(receiveEvent);  
  Wire.onRequest(sendEvent);
  
  lcd.begin(16, 2);

  lcd.createChar(1, water);
  lcd.createChar(2, sun);
  lcd.createChar(3, temp);
  pinMode(SoilMoisturePower, OUTPUT);         //Set D6 as an OUTPUT
  digitalWrite(SoilMoisturePower, LOW);       //Turn it Off when not in use

  lcd.setCursor(0, 0);
  lcd.write(byte(1));
  lcd.setCursor(9, 0);
  lcd.write(byte(2));
  lcd.setCursor(0, 1);
  lcd.write(byte(3));


}

char soilBuf[4];
char lightBuf[4];
char temperatureBuf[4];

void loop() {

  if (flag1 == HIGH)  {    
    Serial.println(x, HEX);    
    flag1 = LOW;  
  }  
  digitalWrite(13, !digitalRead(13));  
  delay(1000);

  
  int soilMoistureLevel= readSoil();
  lcd.setCursor(2, 0);
  sprintf(soilBuf, "%-3d", soilMoistureLevel);
  lcd.print(soilBuf);

  Serial.print("Soil Moisture level: ");
  Serial.println(soilMoistureLevel);

  int lightSensorValue = analogRead (LightSensorPin);
  lcd.setCursor(12, 0);
  sprintf(lightBuf, "%-3d", lightSensorValue);
  lcd.print(lightBuf);

  Serial.print("Light:");
  Serial.println(lightSensorValue);

  int temperatureSensorValue = analogRead (TemperatureSensorPin);
  
  float R = 1023.0/temperatureSensorValue-1.0;
  R = R0*R;

  float temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15;
  dtostrf(temperature, 4, 1, temperatureBuf);
  lcd.setCursor(2, 1);
    
  lcd.print(temperatureBuf);
  lcd.print("C");
  Serial.print("Temperature:");
  Serial.println(temperature);
  
   delay(1000);
}
