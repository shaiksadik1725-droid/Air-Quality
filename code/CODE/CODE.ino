/* Blynk Configuration - MUST be at the very top */
#define BLYNK_TEMPLATE_ID "TMPL6QoweDJzS"
#define BLYNK_TEMPLATE_NAME "Air Quality"
#define BLYNK_AUTH_TOKEN "xGYT7YfSqaFX_2YWvXUJOhMSf1q-kn_g"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

// --- WiFi Credentials ---
char ssid[] = "HUAWEI_H112_DEA2";
char pass[] = "17ARQN13NFE";

// --- Pin Definitions ---
#define DHTPIN 33          // DHT22 data pin
#define DHTTYPE DHT22      // DHT 22 (AM2302)
#define MQ135_PIN 35       // MQ135 Analog pin
#define MQ7_PIN 32         // MQ7 Analog pin
#define MQ2_PIN 36         // MQ2 Analog pin
#define LED_G_AIR 14
#define LED_Y_AIR 27
#define LED_R_AIR 26
#define Buzzer 2

// GP2Y1014AU Dust Sensor Pins
#define DUST_LED_PIN 4     // Digital pin for the sensor's internal LED
#define DUST_OUT_PIN 34    // Analog pin for reading the dust value

// --- Initialize Components ---
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// --- Dust Sensor Timing Variables ---
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

// --- Sensor Reading Function ---
// This function replaces the loop() logic so Blynk doesn't disconnect
void sendSensorData() {
  Serial.println("----------------------------------------");

  // 1. Read and Send DHT22
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Celsius
  
  if (!isnan(humidity) && !isnan(temperature)) {
    Blynk.virtualWrite(V0, temperature);
    Blynk.virtualWrite(V1, humidity);
    
    Serial.print("Temperature: "); Serial.print(temperature);
    Serial.print(" °C  |  Humidity: "); Serial.print(humidity); Serial.println(" %");
  } else {
    Serial.println("Failed to read from DHT sensor!");
  }

  // 2. Read and Send MQ135
  int mq135_raw = analogRead(MQ135_PIN);
  Blynk.virtualWrite(V2, mq135_raw);
  Serial.print("MQ135 Raw: "); Serial.println(mq135_raw);

  // 3. Read and Send MQ7
  int mq7_raw = analogRead(MQ7_PIN);
  Blynk.virtualWrite(V3, mq7_raw);
  Serial.print("MQ7 Raw:   "); Serial.println(mq7_raw);

  // 4. Read and Send MQ2
  int mq2_raw = analogRead(MQ2_PIN);
  Blynk.virtualWrite(V4, mq2_raw);
  Serial.print("MQ2 Raw:   "); Serial.println(mq2_raw);

  // 5. Read and Send Dust Sensor
  digitalWrite(DUST_LED_PIN, LOW); 
  delayMicroseconds(samplingTime);
  
  int dust_raw = analogRead(DUST_OUT_PIN); 
  
  delayMicroseconds(deltaTime);
  digitalWrite(DUST_LED_PIN, HIGH); 
  delayMicroseconds(sleepTime);

  float dust_voltage = dust_raw * (3.3 / 4095.0);
  float dust_density = 0.17 * dust_voltage - 0.1;
  
  if (dust_density < 0) {
    dust_density = 0.00;
  }
  if((mq135_raw > 2100 && mq135_raw<2500) || (mq7_raw > 2100 && mq7_raw < 2500) || (mq2_raw > 2100 && mq2_raw < 2500) ){
    digitalWrite(LED_Y_AIR, HIGH);
    digitalWrite(LED_G_AIR, LOW);
    digitalWrite(LED_R_AIR, LOW);
    digitalWrite(Buzzer, LOW);
  }
  else if(mq135_raw > 2500 || mq7_raw > 2500 || mq2_raw > 2500 ){
    digitalWrite(LED_Y_AIR, LOW);
    digitalWrite(LED_G_AIR, LOW);
    digitalWrite(LED_R_AIR, HIGH);
    digitalWrite(Buzzer, HIGH);
  }
  else{
  digitalWrite(LED_Y_AIR, LOW);
    digitalWrite(LED_G_AIR, HIGH);
    digitalWrite(LED_R_AIR, LOW);
    digitalWrite(Buzzer, LOW);
  }

  Blynk.virtualWrite(V5, dust_density);
  
  Serial.print("Dust Density: "); Serial.print(dust_density); Serial.println(" mg/m3");
  Serial.println("----------------------------------------\n");
}

void setup() {
  // Start Serial Monitor
  Serial.begin(115200);
  
  // Initialize DHT
  dht.begin();
  pinMode(LED_G_AIR, OUTPUT);
pinMode(LED_Y_AIR, OUTPUT);
pinMode(LED_R_AIR, OUTPUT);
  pinMode(Buzzer, OUTPUT);
digitalWrite(Buzzer, LOW);

  // Initialize Dust Sensor LED pin
  pinMode(DUST_LED_PIN, OUTPUT);
  digitalWrite(DUST_LED_PIN, HIGH); 
  
  // Connect to WiFi and Blynk
  Serial.println("Connecting to Blynk...");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Set the timer to call sendSensorData() every 2000 milliseconds (2 seconds)
  timer.setInterval(2000L, sendSensorData);
}

void loop() {
  // Keep the Blynk connection and Timer running
  Blynk.run();
  timer.run();
}