#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial espSerial(0, 1); // RX, TX

void setup() {
  Serial.begin(9600); // Inisialisasi komunikasi serial Arduino Mega
  espSerial.begin(115200); // Inisialisasi komunikasi serial dengan ESP01
  
  delay(1000); // Jeda 1 detik
  
  sendCommand("AT+RST\r\n", 2000); // Mengirim perintah reset ke ESP01
  
  sendCommand("AT+CWMODE=1\r\n", 1000); // Mengatur mode ESP01 sebagai mode Station (klien)
  
  // Mengatur SSID (nama jaringan) dan password WiFi yang akan dihubungkan
  String ssid = "Rumah sakit";
  String password = "k0stput1h";
  
  // Membuat perintah untuk menghubungkan ke jaringan WiFi
  String connectCommand = "AT+CWJAP=\"" + ssid + "\",\"" + password + "\"\r\n";
  sendCommand(connectCommand, 5000); // Mengirim perintah untuk terhubung ke jaringan WiFi
  
  // Memeriksa status koneksi WiFi
  if (sendCommand("AT+CWJAP?\r\n", 2000).indexOf("OK") != -1) {
    Serial.println("WiFi Terhubung");
  } else {
    Serial.println("Gagal Terhubung ke WiFi");
  }
}

void loop() {
  // Kode program utama
  
}

String sendCommand(String command, int timeout) {
  String response = "";
  espSerial.print(command);
  
  long startTime = millis();
  while ((millis() - startTime) < timeout) {
    while (espSerial.available()) {
      char c = espSerial.read();
      response += c;
    }
  }
  
  return response;
}
