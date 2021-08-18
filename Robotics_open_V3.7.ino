#include <Servo.h>
int LDR1;
int LDR2;
int changesolar;
int plusorminus;
Servo myservo;
int haveornot;
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
}
void loop() {
  // put your main code here, to run repeatedly:
  int count;
  int i = 0;
  pinMode(13, OUTPUT); // led
  pinMode(12, OUTPUT); // servo
  pinMode(11, INPUT); // DHT
  pinMode(A0, INPUT); // LDR1
  pinMode(A1, INPUT); // LDR2
  pinMode(A2, INPUT); // TMP
  Servo myservo;
  myservo.attach(12);
  myservo.write(180);
  float TMParray[1000] = {};
  int tilt;
  LDR1 = analogRead(A0);
  LDR2 = analogRead(A1);
  changesolar = LDR1 - LDR2;
  plusorminus = map(changesolar, -1023, 1023, 1, 2);
  if (plusorminus == 1) {
    tilt = myservo.read();
    myservo.write(tilt + 5);
  }
  else if (plusorminus == 2) {
    tilt = myservo.read();
    myservo.write(tilt + 5);
  }

  float analog = analogRead(A2);
  float voltage = analog * (3000.0 / 1023.0);
  float temp = (voltage - 500) / 10;
  TMParray[count] = temp;
  haveornot = Serial.available();
  if (haveornot > 0) {
    while (i < count) {
      Serial.println(TMParray[i]);
      i = i + 1;
      delay(5000);
    }
  }
  count = count + 1;
}
