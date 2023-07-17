#include <MQ135.h>
#include <Wire.h>

#define carbonMQ1351        A8
#define carbonMQ1352        A9


MQ135 CO2Sensor1 (carbonMQ1351);
MQ135 CO2Sensor2 (carbonMQ1352);


void setup() {
  // put your setup code here, to run once:
  Serial.begin (9600);
  pinMode (carbonMQ1351, INPUT);
  pinMode (carbonMQ1352, INPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int carbonPPM1 = CO2Sensor1.getPPM();
  int carbonPPM2 = CO2Sensor2.getPPM();

  Serial.println ("Sensor1");  
  Serial.println (carbonPPM1);
  delay(500);
  Serial.println ("Sensor2");  
  Serial.println (carbonPPM2);
  delay (2000);
}
