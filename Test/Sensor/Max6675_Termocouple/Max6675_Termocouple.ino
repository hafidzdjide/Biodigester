#include <max6675.h>


// ThermoCouple
int thermo_gnd_pin = 45;
int thermo_vcc_pin = 47;
int thermo_so_pin  = 4;
int thermo_cs_pin  = 5;
int thermo_sck_pin = 3;
  
MAX6675 thermocouple(thermo_sck_pin, thermo_cs_pin, thermo_so_pin);
  
void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print("Temp: ");
  Serial.println(thermocouple.readCelsius());
  
  delay(1000);
}
