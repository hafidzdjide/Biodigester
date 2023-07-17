#include <MQ135.h>

#define methaneMQ2Sensor    A0
#define carbonMQ135Sensor   A5

MQ135 co2Sensor (carbonMQ135Sensor);

int methaneGas = 0;
float CH4Voltage;
int methanePPM;

int carbonPPM = 0;

void setup() 
{
    Serial.begin (9600);

    pinMode (methaneMQ2Sensor, INPUT);
    pinMode (carbonMQ135Sensor, INPUT);    
}

void loop() 
{
    methaneGas = analogRead (methaneMQ2Sensor);
    CH4Voltage = methaneGas * (5.0 / 1023.0);
    methanePPM = (15000 / 1024) * ((CH4Voltage / 5) * 1024);

    carbonPPM = co2Sensor.getPPM();

    Serial.print ("Methane PPM : ");
    Serial.print (methanePPM);
    Serial.print ("\t\t");
    Serial.print ("Carbon PPM : ");
    Serial.println (carbonPPM);

    delay (3000);
}
