# IoT Air Quality Monitoring System

An ESP32-based environmental monitoring project for measuring temperature, humidity, gas levels, and airborne dust, with live cloud monitoring through Blynk.

## Features
- DHT22 temperature and humidity monitoring
- MQ135, MQ7, and MQ2 gas sensing
- GP2Y1014AU dust sensing
- Green / yellow / red air-quality indicators
- Buzzer warning for unsafe conditions
- ESP32 Wi-Fi connectivity
- Blynk dashboard integration

## Technology
ESP32, Arduino/C++, Blynk, DHT22, MQ-series gas sensors, optical dust sensor.

## Structure
```text
Air-Quality/
├── code/
├── component/
└── result/
```

## Run
Open `code/CODE/CODE.ino` in the Arduino IDE, install the required ESP32, Blynk, and DHT libraries, enter your own Wi-Fi/Blynk credentials, select the correct ESP32 board, and upload.

## Security
Public source uses placeholders for credentials. Never commit real Wi-Fi passwords or Blynk tokens.

## Future Improvements
- Add calibrated gas concentration calculations
- Add local data logging
- Add historical charts and alert history
- Add MQTT support and offline fallback
- Enclose the prototype in a calibrated airflow housing

## Author
**Sadik Shaik**
