# Biodigester
Program
 - [x] Sistem
 - [ ] Kalibrasi Sensor
 - [ ] Test Kabel
 - [ ] Test Relay dan Output
 - [ ] Test Sensor

IoT
  - [ ] Waktu
  - [ ] ThingSpeak
  - [ ] Telegram

## Notifikasi Telegram
- Warning 1,
- Warning 2,
- Warning 3,
- Warning 4,
- Warning 5,
- Update 1, Water Level, MotorDC
- Update 2, Waktu, MotorDC
- Update 3, Suhu + pH, Valve
- Update 4, Tekanan Tangki Limbah, Strobe (Opsional)
- Update 5, Tekanan Tangki Gas, Buzzer(Opsional)

## Dokumentasi Pin

### IoT
#### Upload ESP
| Pin ESP | Pin Mega |
|:--------|:-------|
|`RX`|`RX0(0)`|
|`TX`|`TX0(1)`|

#### Software Serial
SoftwareSerial (2, 3) // (RX,TX)
| Pin ESP | Pin Mega |
|:--------|:-------|
|`RX`|`D3`|
|`TX`|`D2`|

SoftwareSerial (18,19) // (RX,TX) ARDUINO --> ESP BERHASIL
| Pin ESP | Pin Mega |
|:--------|:-------|
|`RX`|`RX1(19)`|
|`TX`|`TX1(18)`|

####
| Pin ESP | Pin Mega |
|:--------|:-------|
|`D1`|`D3`|
|`D2`|`D2`|

### Relay
Active Low
- Strobe

Active High

| Pin Relay | Output | Pin Mega| 
|:--------|:------- |:-------|
|`IN1`|`Buzzer`|`37`|
|`IN2`|`Motor`|`35`|
|`IN3`|`Valve`|`33`|
|`IN4`|`Strobe`|`31`|

### Sensor
#### Analog
| Sensor | Pin Mega |
|:------- |:-------|
|`Methana`|`A8`|
|`Pressure1`|`A9`|
|`Pressure2`|`A10`|
|`pH`|`A11`|
|`CO2`|`A12`|

#### Digital
| Sensor | Pin Mega |
|:------- |:-------|
|`Flow`|`D22`|
|`Temp(SCK)`|`D24`|
|`Temp(CS)`|`D26`|
|`Temp(SO)`|`D28`|
|`Water Level`|`D30`|

### LCD
| Pin | Pin Mega |
|:------- |:-------|
|`SDA`|`D20`|
|`SCL`|`D21`|
