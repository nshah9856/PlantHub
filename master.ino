#include <Wire.h>

String soilMoisture = "";
String luminosity = "";
String temperature = "";

void setup() {
  // put your setup code here, to run once:
  Wire.begin(2, 14);
  Serial.begin(115200);
  Wire.beginTransmission(0x08);
  Serial.begin(9600);
}

void readAll(){
  Wire.requestFrom(0x08, 15);
  soilMoisture = "";
  luminosity = "";
  temperature = "";

  for(int i = 0; i < 4 && Wire.available(); ++i){
    char c = Wire.read();
    if (c == '#'){
      break;
    }
    soilMoisture += String(c);
  }
    
  for(int i = 0; i < 4 && Wire.available(); ++i){
    char c = Wire.read();
    if (c == '#'){
      break;
    }
    luminosity += String(c);
  }

  while(Wire.available()){
    char c = Wire.read();
    if (c == '#'){
      break;
    }
    temperature += String(c);
  }
}

void loop() {
  readAll();  
  Serial.print("Soil: ");
  Serial.println(soilMoisture);
  Serial.print("Lum: ");
  Serial.println(luminosity);
  Serial.print("Temp: ");
  Serial.println(temperature);
  delay(1000);
}
