#include <Wire.h>
#include <LiquidCrystal_I2C.h> //Library untuk LCD
// Lib untuk Sensor
#include <max6675.h> //Thermocouple
#include <MQ135.h> //CO2
// Lib Untuk Waktu
#include <NTPClient.h>
#include<WiFiEspUdp.h>
// Lib Untuk ThingSpeak
#include<KRwifi.h>

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
const byte buzzerRelay      = 37;

long currentTime;
unsigned long previousLoop = 0;

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

const byte onHour   = 16;
const byte onMin    = 0;
const byte offHour  = 16;
const byte offMin   = 30;

LiquidCrystal_I2C lcd1 = LiquidCrystal_I2C (0x27, 20, 4);
LiquidCrystal_I2C lcd2 = LiquidCrystal_I2C (0x25, 20, 4);

WiFiEspUDP ntpUDP;
const long utcOffsetInSeconds = 25200;
char daysOfTheWeek[7][10] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jum'at", "Sabtu"};
NTPClient timeClient(ntpUDP, "id.pool.ntp.org", utcOffsetInSeconds);

String TimeNow = "";
String DateNow = "";

//Setting WiFi dan ThingSpeak
char* ssid         = "muro-3";
char* pass         = "Piscok2000";
char* server       = "api.thingspeak.com";
char* APIKey       = "4CBVIOUMLX9PK4ZK";

//Setting Periode Kirim ThingSpeak
uint32_t periodeKirim   = 20000;
uint32_t millisKirim;

void setup() 
{
    Serial.begin (9600);
    
    setWifi(ssid, pass);
    millisKirim = millis();
    
    Serial.println ("C0");
    timeClient.begin();
    
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
    lcd2.print ("Metana : ");
    lcd2.setCursor (17, 0);
    lcd2.print ("PPM");
    lcd2.setCursor (0, 2);
    lcd2.print ("Tekanan: ");
    lcd2.setCursor (17, 2);
    lcd2.print ("Bar");
    lcd2.setCursor (0, 1);
    lcd2.print ("CO2    : ");
    lcd2.setCursor (17, 1);
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
    pinMode (buzzerRelay, OUTPUT);      //Buzzer Relay
    
    Serial.println ("C2");
}

void loop() 
{   
    timeClient.update();
    DateNow = daysOfTheWeek[timeClient.getDay()] ;
    TimeNow = String()+timeClient.getHours()+":"+timeClient.getMinutes()+":"+timeClient.getSeconds();
    
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

    lcd2.setCursor (0,3);
    lcd2.print(DateNow);
    lcd2.setCursor (8,3);
    lcd2.print(TimeNow);
    
    Serial.println ("K1");
    if (kondisiTangki = "Penuh"){
      digitalWrite (motorRelay, LOW);
    }else{
      digitalWrite (motorRelay, HIGH);
    }

    Serial.println ("K2"); // Kondisi setiap jam 4.30 menyalakan motorDC
    if (timeClient.getHours() == onHour && timeClient.getMinutes() == onMin){
      digitalWrite (motorRelay, HIGH);
    }else if (timeClient.getHours() == offHour && timeClient.getMinutes() == offMin){
      digitalWrite (motorRelay, HIGH);
    }
    
    Serial.println ("K3");  
    if ((finalphValue >= 6.5 && finalphValue <= 7.5)  || (temperatureValue >= 20 && temperatureValue <= 30)){
      digitalWrite (valveRelay, LOW);
    }else{
      digitalWrite (valveRelay, HIGH);
    }
    
    Serial.println ("K4"); 
    if (pressureValue1 <= 0.4){
      digitalWrite (strobeRelay, LOW);
    }else if (pressureValue1 <= 0.2){
      Pattern();
    }else{
      digitalWrite (strobeRelay, HIGH);
    }

    Serial.println ("K5");
    if (pressureValue2 <= 0.4){
        digitalWrite (buzzerRelay, LOW);
    }else{
        digitalWrite (buzzerRelay, HIGH);
    }
    
    Serial.println ("C5");
    if (millisKirim < millis()) {
      millisKirim = millis() + periodeKirim;
      String konten;
      konten = String()+"field1="+pressureValue1+"&field2="+finalphValue+"&field3="+temperatureValue+"&field4="+pressureValue2+"&field5="+methanePPM+"&field6="+carbonPPM+"&field7="+liquidLevel;
//      Serial.println(konten);
      httpPOST(server, APIKey, konten, 120, 80);  }  // Kondisi permintaan data telegram (kalo jadi).
    
    Serial.println ("C6");
    delay (1000);
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
    kondisiTangki = " Penuh";
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

void Pattern(){
  const long interval = 5000;
  currentTime = millis();
  int strobeState;
  if (currentTime - previousLoop >= interval) {
    previousLoop = currentTime;
    if (strobeState == LOW) {
      strobeState = HIGH;
    }else {
     strobeState = LOW;
    }
  digitalWrite(strobeRelay, strobeState);
  }
}
