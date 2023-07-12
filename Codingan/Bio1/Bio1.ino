#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <max6675.h>
#include <DS3231.h>
#include <MQ135.h>

#define methaneMQ2          A8
#define pressureSensor1     A9
#define pressureSensor2     A10
#define phSensor            A11
#define carbonMQ135         A12

const byte tempSCK          = 24;
const byte tempCS           = 26;
const byte tempSO           = 28;
const byte levelSensor      = 30;

const byte strobeRelay      = 31;
const byte valveRelay       = 33;
const byte motorRelay       = 35;
const byte pumpRelay        = 37;


int methanePPM = 0;

int liquidLevel             = 0;        //LEVEL LIMBAH TANGKI 1
String kondisiTangki;                   //LEVEL LIMBAH TANGKI 1

int pressureAnalog1;
int pressureAnalog2;
float pressureValue1 ;                        //TEKANAN TANGKI 1
float pressureValue2;                        //TEKANAN TANGKI 2

int temperatureValue;                   //TEMPERATUR TANGKI 1

float phAnalog;
float finalphValue          = 0.0;      //PH TANGKI 1

MAX6675 thermocouple(tempSCK, tempCS, tempSO);
MQ135 CO2Sensor (carbonMQ135);

LiquidCrystal_I2C lcd1 = LiquidCrystal_I2C (0x27, 20, 4);
LiquidCrystal_I2C lcd2 = LiquidCrystal_I2C (0x25, 20, 4);

void setup() 
{
    Serial.begin (9600);
    Serial.println ("C0");
    
    lcd1.init();
    lcd2.init();
    lcd1.backlight();
    lcd2.backlight();

    lcd1.setCursor (0, 0);
    lcd1.print ("Tangki 1  : ");
    lcd1.setCursor (0, 1);
    lcd1.print ("Tekanan   : ");
    lcd1.setCursor (17, 1);
    lcd1.print ("Bar");
    lcd1.setCursor (0, 2);
    lcd1.print ("Temperatur:   ");
    lcd1.setCursor (18, 2);
    lcd1.print ((char)223);
    lcd1.print ("C");
    lcd1.setCursor (0, 3);
    lcd1.print ("pH Limbah : ");

    lcd2.setCursor (0, 0);
    lcd2.print ("Tangki 2");
    lcd2.setCursor (0, 1);
    lcd2.print ("Metana : ");
    lcd2.setCursor (17, 1);
    lcd2.print ("PPM");
    lcd2.setCursor (0, 3);
    lcd2.print ("Tekanan: ");
    lcd2.setCursor (17, 3);
    lcd2.print ("Bar");
    lcd2.setCursor (0, 2);
    lcd2.print ("CO2    : ");
    lcd2.setCursor (17, 2);
    lcd2.print ("PPM"); 
//    lcd2.setCursor (0, 3); kosong, bekas sensor flow
//    lcd2.print (" : ");
//    lcd2.setCursor (15, 3);
//    lcd2.print ("");

    Serial.println ("C1");

    pinMode (methaneMQ2, INPUT);        //MQ2 Sensor
    pinMode (pressureSensor1, INPUT);   //Pressure Sensor 1
    pinMode (pressureSensor2, INPUT);   //Pressure Sensor 2
    pinMode (phSensor, INPUT);          //pH Sensor
    pinMode (carbonMQ135, INPUT);       //MQ135 Sensor
    pinMode (levelSensor, INPUT);       //Level Sensor
    
    pinMode (strobeRelay, OUTPUT);      //Strobe Lamp Relay
    pinMode (valveRelay, OUTPUT);       //Valve Relay
    pinMode (motorRelay, OUTPUT);       //AC Motor Relay 
    
    Serial.println ("C2");
}

void loop() 
{    
    Serial.println ("C3");
    waterLevel();
    SensMethane();
    SenspH();
    int carbonPPM = CO2Sensor.getPPM();

    pressure1();
    pressure2();
  
    temperatureValue = thermocouple.readCelsius();
    
    Serial.println ("C4");
    lcd1.setCursor (12, 0);
    lcd1.print (kondisiTangki);
    //Serial.println (liquidLevel);
    //Serial.println (kondisiTangki);

    lcd1.setCursor (12, 1);
    lcd1.print (pressureValue1);
    //Serial.println (pressureValue1);

    lcd1.setCursor (14, 2);
    lcd1.print (int (temperatureValue));
    //Serial.println (temperatureValue);

    lcd1.setCursor (12, 3);
    lcd1.print (finalphValue);
    //Serial.println (finalphValue);
    
    lcd2.setCursor (9, 0);
    lcd2.print ("       ");
    lcd2.setCursor (9, 0);
    lcd2.print (methanePPM);    
    //Serial.println (methanePPM);
    
    lcd2.setCursor (9, 2);
    lcd2.print (pressureValue2);
    //Serial.print ((pressureValue2);

    lcd2.setCursor (9, 1);
    lcd2.print ("       ");
    lcd2.setCursor (9, 1);
    lcd2.print (carbonPPM);
    //Serial.println (carbonPPM);



    
    Serial.println ("K1");
    if (kondisiTangki = "Penuh"){
      digitalWrite (motorRelay, LOW);
    }else{
      digitalWrite (motorRelay, HIGH);
    }
    
    Serial.println ("C4");  
   
    Serial.println ("K4");  
    if ((finalphValue >= 6.5 && finalphValue <= 7.5)  || (temperatureValue >= 20 && temperatureValue <= 30)){
      digitalWrite (valveRelay, LOW);
    }else{
      digitalWrite (valveRelay, HIGH);
    }
    
    Serial.println ("K5");  
    
    if (pressureValue1 >= 0.4)
    {
        digitalWrite (strobeRelay, LOW);
    }
    else
    {
        digitalWrite (strobeRelay, HIGH);
    }

    Serial.println ("C5");
    
    delay (1250);
}
float pressure (int pressureValue){
    float voltage = (pressureValue*5.0)/1024.0;
    float pressurePascal = (3.0*(voltage-0.47))*1000000.0;
    float pressureBar = pressurePascal/10e5;
    return pressureBar; 
}

void pressure1(){
  pressureAnalog1 = analogRead (pressureSensor1);
  pressureValue1 = pressure (pressureAnalog1);
}

void pressure2(){
    pressureAnalog2 = analogRead (pressureSensor2);
    pressureValue2 = pressure (pressureAnalog2);
}

void waterLevel(){
  liquidLevel = digitalRead (levelSensor);
  if (liquidLevel == 1){
    kondisiTangki = "Penuh";
  }else if (liquidLevel == 0){
    kondisiTangki = "Kosong";
  }
}

void SensMethane(){
    int methaneGas = analogRead (methaneMQ2);
    float CH4Voltage = methaneGas * (5.0 / 1023.0);
    int methanePPM = (15000 / 1024) * ((CH4Voltage / 5) * 1024);
}

void SenspH(){
    phAnalog = analogRead (phSensor);
    finalphValue = (-0.0693 * phAnalog) + 7.3855;
}
    
