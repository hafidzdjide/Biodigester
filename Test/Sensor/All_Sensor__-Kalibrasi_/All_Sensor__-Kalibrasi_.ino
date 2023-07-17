/*
 XKC-Y25-V Water Level Sensor
 Brown VCC 5V
 Yellow (Orange) Data 3
 Blue GND GND
 */

/*
 LCD 16x02 & 20x04 + I2C
 SDA A4
 SCL A5
 VCC 5V
 GND GND
 */

/*
 Opto Relay
 IN 2
 VCC 5V
 GND GND
 */

/*
 MAX6675 Termocouple
 VCC 5V
 GND GND
 SCK 13
 CS 10
 S0 4
 */

/*
 Flow Sensor
 IN 5
 VCC 5V
 GND GND
 */

/*
 Pressure Sensor 1
 IN A0
 VCC 5V
 GND GND
 */

/*
 Pressure Sensor 2
 IN A1
 VCC 5V
 GND GND
 */

/*
 Soil pH Sensor
 Putih A2
 Hitam GND
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <max6675.h>
#include <SoftwareSerial.h>

SoftwareSerial nanoSerial (9, 8);

LiquidCrystal_I2C lcd1 = LiquidCrystal_I2C (0x27, 20, 4);
LiquidCrystal_I2C lcd2 = LiquidCrystal_I2C (0x25, 16, 2);

int liquidLevel = 0;
char tangki;

int pressureValue1;
int pressureValue2;
int temperatureValue;

int adcValue = 0;
float finalphValue = 0.0;

const byte thermosoPin  = 4;
const byte thermocsPin  = 10;
const byte thermosckPin = 13;

const byte flowSensor = 2;
volatile int flowFrequency;
unsigned int lperHour;
unsigned int lperMinute;
unsigned long currentTime;
unsigned long cloopTime;

float pressureBar1;
float pressureBar2;

MAX6675 thermocouple(thermosckPin, thermocsPin, thermosoPin);

void flow()
{
    flowFrequency++;
}

void setup() 
{
    Serial.begin (9600);
    nanoSerial.begin (9600);
    
    lcd1.init();
    lcd2.init();
    lcd1.backlight();
    lcd2.backlight();
    
    pinMode (3,INPUT); //Level Sensor
    pinMode (5, OUTPUT); //Relay 1
    pinMode (6, OUTPUT); //Relay 2
    pinMode (A0, INPUT); //Pressure Sensor 1
    pinMode (A1, INPUT); //Pressure Sensor 2
    pinMode (A2, INPUT); //pH Sensor

    pinMode (flowSensor, INPUT); //Flow Sensor
    digitalWrite (flowSensor, HIGH);
    attachInterrupt (0, flow, RISING);
    sei();
    currentTime = millis();
    cloopTime = currentTime;
}

void loop() 
{
    currentTime = millis();
    if (currentTime >= (cloopTime + 1000))
    {
        cloopTime = currentTime;
        //Pulse Frequency (Hz) = 7.5Q, Q is flow rate in L/Min.
        lperMinute = (flowFrequency / 7.5);
        lcd2.setCursor (0, 1);
        lcd2.print ("Laju : ");
        lcd2.print (lperMinute);
        lcd2.print ("   L/Min");
        flowFrequency = 0;
    }
    else
    {
        lcd2.setCursor (0, 1);
        lcd2.print ("Laju : ");
        lcd2.print (flowFrequency);
        lcd2.print ("   L/Min");
    }
    
    pressureValue1 = analogRead (A0);
    pressureValue2 = analogRead (A1);
    
    liquidLevel = digitalRead (3);
    temperatureValue = thermocouple.readCelsius();
    
    adcValue = analogRead (A2);
    finalphValue = (0.109*adcValue) + 0.575;

    float pressureBar1 = pressure (pressureValue1);
    float pressureBar2 = pressure (pressureValue2);

    if (liquidLevel == 1)
    {
        digitalWrite (5, HIGH);
        tangki = "Penuh";
        lcd1.setCursor (0, 0);
        lcd1.print ("Tangki     : ");
        lcd1.print ("Penuh ");
        //+Serial.println ("Penuh");
    }
    
    else
    {
        digitalWrite (5, LOW);
        tangki = "Kosong";
        lcd1.setCursor (0, 0);
        lcd1.print ("Tangki     : "); 
        lcd1.print ("Kosong");
        Serial.println ("Kosong");
    }

    lcd1.setCursor (0, 1);
    lcd1.print ("Tekanan    : ");
    lcd1.print (pressureBar1);
    lcd1.print ("Bar");

    lcd1.setCursor (0, 2);
    lcd1.print ("Temperatur :   ");
    lcd1.print (temperatureValue);
    lcd1.print ((char)223);
    lcd1.print ("C");

    lcd1.setCursor (0, 3);
    lcd1.print ("pH Limbah  : ");
    lcd1.print (finalphValue);

    lcd2.setCursor (0, 0);
    lcd2.print ("Tekanan :");
    lcd2.print (pressureBar2);
    lcd2.print ("Bar");
    
    sendLevel();
    
    delay(500);
}

float pressure (int pressureValue)
{
    float voltage = (pressureValue*5.0)/1024.0;
    float pressurePascal = (3.0*((float)voltage-0.47))*1000000.0;
    float pressureBar = pressurePascal/10e5;
    return pressureBar;
}

void sendLevel()
{
    nanoSerial.print (tangki);
    nanoSerial.print (',');
    nanoSerial.print ("\n");
}
