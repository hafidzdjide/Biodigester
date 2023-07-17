#include <Wire.h>

const byte levelSensor      = 30;

int liquidLevel             = 0;        //LEVEL LIMBAH TANGKI 1
String kondisiTangki;                   //LEVEL LIMBAH TANGKI 1

void setup() {
  // put your setup code here, to run once:
    Serial.begin (9600);
    pinMode (levelSensor, INPUT);       //Level Sensor
}

void loop() {
  // put your main code here, to run repeatedly:
  liquidLevel = digitalRead (levelSensor);
  if (liquidLevel == 1){
    kondisiTangki = "Penuh";
  }else if (liquidLevel == 0){
    kondisiTangki = "Kosong";
  }
    Serial.println (liquidLevel);
    Serial.println (kondisiTangki);
}
