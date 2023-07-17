#include <SoftwareSerial.h>

SoftwareSerial espSerial(1, 0); // RX, TX

void setup() {
  Serial.begin(9600); // Inisialisasi komunikasi serial Arduino Mega
  espSerial.begin(115200); // Inisialisasi komunikasi serial ESP01
  
  // Mengirim perintah AT ke ESP01
  espSerial.println("AT");
  delay(500);
  
  // Memeriksa respons dari ESP01
  if (espSerial.find("OK")) {
    Serial.println("ESP01 siap.");
  } else {
    Serial.println("ESP01 tidak merespon. Periksa koneksi dan konfigurasi.");
    while (1);
  }
  
  // Menghubungkan ESP01 ke jaringan WiFi
  espSerial.println("AT+CWJAP=\"Rumah sakit\",\"k0stput1h\"");
  delay(5000);
  
  // Memeriksa status koneksi WiFi
  if (espSerial.find("OK")) {
    Serial.println("Terhubung ke WiFi.");
  } else {
    Serial.println("Gagal terhubung ke WiFi. Periksa SSID dan kata sandi WiFi.");
    while (1);
  }
}

void loop() {
  // Mengirim data dari Arduino Mega ke ESP01
  espSerial.println("Data yang ingin dikirim");
  delay(1000);
  
  // Menerima data dari ESP01 ke Arduino Mega
  while (espSerial.available()) {
    char receivedData = espSerial.read();
    Serial.print(receivedData);
  }
}
