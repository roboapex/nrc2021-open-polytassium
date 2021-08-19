#include <DHT.h>
#include <DHT_U.h>
#include <Servo.h>
int LDR1;
int LDR2;
int changesolar;
int plusorminus;
Servo myservo;
int haveornot;
DHT dht(11, 11);
float h;
float TMParray[100] = {};
float DHTarray[100] = {};
int tilt;
void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT); // led
  pinMode(12, OUTPUT); // servo
  pinMode(11, INPUT); // DHT
  pinMode(A0, INPUT); // LDR1
  pinMode(A1, INPUT); // LDR2
  pinMode(A2, INPUT); // TMP
  myservo.attach(12);
  myservo.write(180);
  float TMParray[1000] = {};
  int count = 0;
  Serial.begin(9600);
  dht.begin();
}
void loop() {
  // put your main code here, to run repeatedly:
  delay(5000);
  int day = millis() / 86400000;
  int hour = millis() % 86400000 / 3600000;
  int mins = millis() % 86400000 % 3600000 / 1000;
  int count;
  int i = 0;
  int x = 0;
  int doornot = count % 720;
  pinMode(13, OUTPUT); // led
  pinMode(12, OUTPUT); // servo
  pinMode(11, INPUT); // DHT
  pinMode(A0, INPUT); // LDR1
  pinMode(A1, INPUT); // LDR2
  pinMode(A2, INPUT); // TMP
  Servo myservo;
  LDR1 = analogRead(A0);// readings for tilt
  LDR2 = analogRead(A1);
  changesolar = LDR1 - LDR2;
  plusorminus = map(changesolar, -1023, 1023, 1, 2);
  if (plusorminus == 1) {
    tilt = myservo.read();
    myservo.write(tilt + 5);
  }
  else if (plusorminus == 2) {
    tilt = myservo.read();
    myservo.write(tilt - 5);
  }
  // the changes so it will auto tune to optimum angle
  myservo.attach(12);
  myservo.write(180);
  int tilt;
  if (doornot == 0) {
    int h = dht.readHumidity();
    int t = dht.readTemperature();
    float hic = dht.computeHeatIndex(t, h, false);
    DHTarray[count] = h;
    float analog = analogRead(A2);
    float voltage = analog * (3000.0 / 1023.0);
    float temp = (voltage - 500) / 10;
    TMParray[count] = temp;

  }
  haveornot = Serial.available();
  if (haveornot > 0) {
    Serial.println("Temperature");

    while (i < count) {
      Serial.println(TMParray[i]);
      Serial.print("C");
      i = i + 1;
    }
    Serial.println("Humidity");
    while (x < count) {
      Serial.println(DHTarray[i]);
      Serial.print("%");
    }
    Serial.println("Information taken over a period of");
    Serial.println(day);
    Serial.print("days");
    Serial.println(hour);
    Serial.print("hours");
    Serial.println(mins);
    Serial.print("mins");

  }
  count = count + 1;
}
