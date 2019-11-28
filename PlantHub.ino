
#include <Wire.h>
#include "rgb_lcd.h"

#define SoilMoisturePin A2
#define SoilMoisturePower 6
#define LightSensorPin A0
#define TemperatureSensorPin A3

rgb_lcd lcd;
byte x;
bool flag = LOW;

const byte DATA_SIZE = 2;

String soil;
String lum;
String tem;
String d;

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

void setup() {
  Serial.begin(115200);
  Wire.begin(0x08);
  pinMode(13, OUTPUT);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(sendEvent);

  pinMode(SoilMoisturePower, OUTPUT);         //Set D6 as an OUTPUT
  digitalWrite(SoilMoisturePower, LOW);       //Turn it Off when not in use

  Wire.beginTransmission(0x3C);
  
  lcd.begin(16, 2);

  lcd.createChar(1, water);
  lcd.createChar(2, sun);
  lcd.createChar(3, temp);

  lcd.setCursor(0, 0);
  lcd.write(byte(1));
  lcd.setCursor(9, 0);
  lcd.write(byte(2));
  lcd.setCursor(0, 1);
  lcd.write(byte(3));

  Wire.endTransmission();

  delay(500); //Just to make sure setup completed
}

int getSoil(){
  int l = readSoil();
  Serial.print("Soil moisture: ");
  Serial.println(l);
  return l;
}

float getTemp(){
  int temperatureSensorValue = analogRead (TemperatureSensorPin);
  float R = 1023.0/temperatureSensorValue-1.0;
  R = R0*R;
  float temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15;
  Serial.print("Temperature: ");
  Serial.println(temperature);
  return temperature;
}

int getLuminosity(){
  int lightSensorValue = analogRead (LightSensorPin);
  Serial.print("Luminosity: ");
  Serial.println(lightSensorValue);
  return lightSensorValue;
}

void receiveEvent(int howMany){
  x = Wire.read();
  flag=HIGH;
}

void sendEvent(){
  Serial.println("Sending...");
  Wire.write(d.c_str());
  Serial.println("Sent!");
}

void loop(){
  if (flag == HIGH){
    flag = LOW;
    Serial.println(x, HEX);
  }
  digitalWrite(13, !digitalRead(13));

  int s = getSoil();
  int l = getLuminosity();
  float t = getTemp();
  
  soil = String(s);
  lum = String(l);
  tem = String(t, 2);
  
  Wire.beginTransmission(0x3C);

  lcd.setCursor(2, 0);
  lcd.print(soil);
  lcd.setCursor(11, 0);
  lcd.print(lum);
  lcd.setCursor(2, 1);
  lcd.print(tem);
  
  Wire.endTransmission();

  d = soil + "#" + lum + "#" + tem + "#";

  delay(1000);
}
