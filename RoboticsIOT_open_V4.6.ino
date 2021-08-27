/*
  Adafruit IO with MQTT

  For the SSTuino boards.

  This example sketch publishes to Adafruit IO every 7.5 seconds using MQTT.
  This can be a foundation to use for uploading sensor data to the cloud for
  an IoT sensor.

  This example code is in the public domain.

  https://d3lta-v.github.io/SSTuino/
*/
#include <DHT.h>
#include <DHT_U.h>
#include <Servo.h>
#include "SSTuino_Companion.h"

#define SSID         "SSID_GOES_HERE"
#define PASSWORD     "WIFI_PASSWORD_GOES_HERE"
#define IO_USERNAME  "LOZachary"
#define IO_KEY       "aio_zBAp466wNUshWoZlFBak61GUBK8x"
#define FEED_KEYTMP     "tmp-dot-robo-dot-open"
#define FEED_KEYDHT "dht-dot-robo-dot-open"
SSTuino wifi = SSTuino();
const int Servpin = 8;
int LDR1 = A0;
int LDR2 = A1;
int  TMP = A2;
int DHTs = 11;
int SMP = 12;
int LED = 13;
long tmpv;
long tmpc;
long hum;
long humn;
int i;
int x;

void setup()
{
  Serial.begin(9600);

  // Open the link between the two devices
  wifi.openLink();

  // Reset the Wi-Fi chip to clear any previous settings
  wifi.reset();

  // Verify that the link is ok between the two devices
  if (!wifi.smokeTest()) {
    Serial.println(F("Unable to establish link with Wi-Fi chip. Halting."));
    while (true) {};
  }

  wifiConnect();

  setupMQTT();
  Servo servo_8;
  servo_8.attach(8, 500, 2500);
  pinMode(LDR1, INPUT);
  pinMode(LDR2, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(DHTs, INPUT);
  pinMode(TMP, INPUT);
  DHT dht(DHTs, 11);

}

void loop()
{
  DHT dht(DHTs, 11);
  Servo servo_8;
  servo_8.attach(8, 500, 2500);
  if (analogRead(LDR1) > analogRead(LDR2)) {
    servo_8.write(((analogRead(LDR1) - analogRead(LDR2)) / 4 + 90));
  } else {
    servo_8.write(((analogRead(LDR2) + analogRead(LDR1)) / 4 - 100));
  }
  tmpv = analogRead(TMP);
  tmpc = map(tmpv, 0, 1023, -40, 125);
  humn = dht.readHumidity();

  transmitDataTMP(String(tmpc));
  transmitDataDHT(String(humn));
  delay(7500); // you can replace this delay with something longer or shorter,
  // but 7.5s interval is preferred to prevent flooding Adafruit IO
}

void wifiConnect(void)
{
  // Connects to Wifi and displays connection state
  wifi.connectToWifi(F(SSID), F(PASSWORD));
  Serial.println(F("Connecting to Wi-Fi..."));

  delay(10000); // 10 seconds optimal for wifi connection to fully establish

  Status wifiStatus = wifi.getWifiStatus();
  if (wifiStatus != SUCCESSFUL) {
    Serial.println(F("Failed to connect to Wi-Fi"));
    while (true) {};
  } else {
    Serial.println(F("Wi-Fi connected"));
  }
}

void setupMQTT(void)
{
  // Setup MQTT
  Serial.println(F("Setting up MQTT..."));
  bool mqttSuccess = wifi.enableMQTT(F("io.adafruit.com"), true, IO_USERNAME, IO_KEY);
  if (!mqttSuccess) {
    Serial.println(F("Failed to enable MQTT. Halting."));
    while (true) {};
  }
  delay(10000); // Wait for MQTT to fully connect

  // Check if MQTT is connected
  if (!wifi.isMQTTConnected()) {
    Serial.println(F("MQTT did not connect successfully!"));
    while (true) {};
  } else {
    Serial.println(F("MQTT connected!"));
  }
}

void transmitDataTMP(const String& value)
{
  if (wifi.mqttPublish(F(IO_USERNAME "/feeds/" FEED_KEYTMP), value)) {
    Serial.println(F("Successfully published data for TMP!"));
  } else {
    Serial.println(F("Failed to publish data for TMP!"));
  }
}
void transmitDataDHT(const String& value)
{
  if (wifi.mqttPublish(F(IO_USERNAME "/feeds/" FEED_KEYDHT), value)) {
    Serial.println(F("Successfully published data for DHT!"));
  } else {
    Serial.println(F("Failed to publish data for DHT!"));
  }
}
