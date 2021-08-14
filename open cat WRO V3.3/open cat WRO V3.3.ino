
#include <Servo.h>
void setup() {
  // put your setup code here, to run once:
#define bat1C 13
#define bat1D 12
#define bat2C 11
#define bat2D 10
#define ledC 9
#define TMPa A0
#define Servpin 8
#define DHTd 7
  pinMode(bat1C, OUTPUT);
  pinMode(bat1D, OUTPUT);
  pinMode(bat2C, OUTPUT);
  pinMode(bat2D, OUTPUT);
  pinMode(ledC, OUTPUT);
  pinMode(TMPa, INPUT);
  pinMode(Servpin, OUTPUT);
  pinMode(DHTd, INPUT);
  Servo myservo;
  myservo.attach(Servpin);
}

void loop() {
  // put your main code here, to run repeatedly:

}
float TMPtemp() {

  float analog = analogRead(TMPa);
  float voltage = analog * (3000.0 / 1023.0);
  float temp = (voltage - 500) / 10;
  return temp;
}
void change1to2D() {
  digitalWrite(bat2C, LOW);
  digitalWrite(bat2D, HIGH);
  digitalWrite(bat1D, LOW);
  digitalWrite(bat1C, HIGH);
}
void change2to1D() {
  digitalWrite(bat1C, LOW);
  digitalWrite(bat1D, HIGH);
  digitalWrite(bat2D, LOW);
  digitalWrite(bat2D, HIGH);
}
int timekeeper() {
  while (true) {
    long day = 86400000; // 86400000 milliseconds in a day
    long hour = 3600000; // 3600000 milliseconds in an hour
    long TimeNOW = millis();
    int hours = (TimeNOW % day) / hour;
    return hours ;
  }
}
