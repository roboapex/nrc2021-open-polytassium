#include <Servo.h>
#include <DHT.h>
#include <DHT_U.h>

/*
  Adafruit IO with MQTT

  For the SSTuino boards.

  This example sketch publishes to Adafruit IO every 7.5 seconds using MQTT.
  This can be a foundation to use for uploading sensor data to the cloud for
  an IoT sensor.

  This example code is in the public domain.

  https://d3lta-v.github.io/SSTuino/
*/

#include "SSTuino_Companion.h"

#define SSID         "CASE"
#define PASSWORD     "CYBORCASE"
#define IO_USERNAME  "LOZachary"
#define IO_KEY       "aio_AvDO23K1wqUyy8rSI8ujfpRKxayQ"
#define FEED_KEY     "dht-dot-robo-dot-open"
#define FEED_KEY2     "tmp-dot-robo-dot-open"
SSTuino wifi = SSTuino();
int LDR1 = A2;
int LDR2 = A1;
int  TMP = A0;
int DHTs = 13;
int SMP = 11;
int LED = 12;
int i;
int tempArray[48] = {};
int humidArray[48] = {};
int temp;
int actemp;
int humn;
long tim;
long tims;
long timc;
long timh;
long timhh;
int x;
/*
  Insert your custom variable declarations here
*/
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
    while (true) {


    };
  }

  wifiConnect();

  setupMQTT();

  /*
    Insert your setup code here
  */
  Servo servo_8;
  servo_8.attach(8, 500, 2500);
  pinMode(LDR1, INPUT);
  pinMode(LDR2, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(DHTs, INPUT);
  pinMode(TMP, INPUT);
}
void whenNoWifi() {
  DHT dht(DHTs, 11);
  Servo servo_8;
  servo_8.attach(8, 500, 2500);
  digitalWrite(LED, HIGH);
  while (true) {
    if (analogRead(LDR1) > analogRead(LDR2)) {
      servo_8.write(servo_8.read() - 1);
    }
    else if (analogRead(LDR2) > analogRead(LDR1)) {
      servo_8.write(servo_8.read() + 1);
    }
    tim = millis();
    timh = tim / 3600;
    timhh = (tim * 10) % 10;
    if (i > 48) {
      i = 0;
    }
    if (timhh == 0) {
      temp = analogRead(TMP);
      actemp = map(temp, 0, 1023, -40, 125);
      humn = dht.readHumidity();
      tempArray[i] = actemp;
      humidArray[i] = humn;
      i = i + 1;
    }
    int gae = Serial.available();
    if (gae == 1) {
      Serial.println("Temp data");
      for (x = 1; x <= i; x++) {
        Serial.println(tempArray[x]);
      }
      x = 0;
      Serial.println("Humidity Data");
      for (x = 1; x <= i; x++) {
        Serial.println(tempArray[x]);
      }
      Serial.println("Data transfer over");

    }
    delay(100);
    if(gae == 1){
      break;
    }
  }
  Serial.println("Re-initialize the robot");
}
void transmitData(const String& value)
{
  if (wifi.mqttPublish(F(IO_USERNAME "/feeds/" FEED_KEY), value)) {
    Serial.println(F("Successfully published data!"));
  } else {
    Serial.println(F("Failed to publish data!"));
  }
}
void transmitData2(const String& value) {

  if (wifi.mqttPublish(F(IO_USERNAME "/feeds/" FEED_KEY2), value)) {
    Serial.println(F("Successfully published data!"));
  } else {
    Serial.println(F("Failed to publish data!"));
  }
}


void loop()
{
  /*
    Insert your loop code here and change "data" to be the data you wish to send
    For example, the new line should look like:

    transmitData(String(yourVariableHere));

    where yourVariableHere is the name of your own variable
  */
  Servo servo_8;
  servo_8.attach(8, 500, 2500);
  DHT dht(DHTs, 11);
  if (analogRead(LDR1) > analogRead(LDR2)) {
    servo_8.write(servo_8.read() - 1);
  }
  else if (analogRead(LDR2) > analogRead(LDR1)) {
    servo_8.write(servo_8.read() + 1);
  }
  temp = analogRead(TMP);
  actemp = map(temp, 0, 1023, -40, 125);
  humn = dht.readHumidity();

  transmitData(String(humn));
  transmitData2(String(actemp));
  tim = millis();
  tims = (tim % 60000) / 1000;
  timc = tims / 10;

  if (timc = 1) {
    transmitData(String(humn));
    transmitData2(String(actemp));
  }
  delay(100); // you can replace this delay with something longer or shorter,
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
    while (true) {

    };
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
    while (true) {

    };
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
