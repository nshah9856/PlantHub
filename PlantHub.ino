
#include <Wire.h>
#include "rgb_lcd.h"
#include "SparkFunCCS811.h"

#define SoilMoisturePin A2
#define SoilMoisturePower 6
#define CCSAddr 0x5A
#define LightSensorPin A0

rgb_lcd lcd;
CCS811 airSensor(CCSAddr);

const int wetThreshold = 400;
const int dryThreshold = 250;

int readSoil() {
  digitalWrite(SoilMoisturePower, HIGH);//turn monitoring "On"
  delay(10);//wait 10 milliseconds 
  int val = analogRead(SoilMoisturePin);//Read the SIG value form sensor 
  digitalWrite(SoilMoisturePower, LOW);//turn monitoring "Off"
  return val;//send current moisture value
}

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);

  pinMode(SoilMoisturePower, OUTPUT);         //Set D6 as an OUTPUT
  digitalWrite(SoilMoisturePower, LOW);       //Turn it Off when not in use

  airSensor.begin();
  
}

void loop() {
//  int soilMoistureLevel = readSoil();
//
//  Serial.print("Soil Moisture level: ");
//  Serial.println(soilMoistureLevel);

//  if(airSensor.dataAvailable()){
//    airSensor.readAlgorithmResults();
//    Serial.print("CO2:");
//    Serial.println(airSensor.getCO2());
//  }

//    int sensorValue = analogRead (LightSensorPin);
//    Serial.print("Light:");
//    Serial.println(sensorValue);
   
}
