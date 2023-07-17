#include <Wire.h>
#define methaneMQ4          A0

void setup() {
  Serial.begin(9600);  // Inisialisasi komunikasi serial
  
  pinMode(methaneMQ4, INPUT);  // Mengatur pin sebagai input
}

void loop() {
  int sensorValue = analogRead(methaneMQ4);  // Membaca nilai analog dari sensor
  
  // Konversi nilai analog ke konsentrasi gas menggunakan persamaan kalibrasi yang sesuai
  float methanePPM = convertToPPM(sensorValue);
  
  // Menampilkan nilai konsentrasi gas methane di Serial Monitor
  Serial.print("Methane PPM: ");
  Serial.println(methanePPM);
  
  delay(1000);  // Jeda 1 detik antara pembacaan
}

float convertToPPM(int sensorValue) {
  // Lakukan perhitungan konversi sesuai dengan persamaan kalibrasi
  // yang spesifik untuk sensor MQ-4
  
  // Contoh persamaan kalibrasi sederhana:
  // PPM = (sensorValue * 5.0 / 1024.0) * 1000
  
  float ppm = (sensorValue * 5.0 / 1024.0) * 1000;
  return ppm;
}
