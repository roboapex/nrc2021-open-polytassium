#include <DHT.h>
#include <DHT_U.h>
#include <Servo.h>
int LDR1 = A0;
int LDR2 = A1;
int  TMP = A2;
int DHTs = 11;
int SMP = 12;
int LED = 13;
int tmpv;
int tmpc[100] = {};
long hum[100] = {};
long humn;
int i;
int x;
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  Servo servo_8;
  servo_8.attach(8, 500, 2500);
  pinMode(LDR1, INPUT);
  pinMode(LDR2, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(DHTs, INPUT);
  pinMode(TMP, INPUT);
  if (analogRead(LDR1) > analogRead(LDR2)) {
    servo_8.write(((analogRead(LDR1) - analogRead(LDR2)) / 4 + 90));
  } else {
    servo_8.write(((analogRead(LDR2) + analogRead(LDR1)) / 4 - 100));
  }
  DHT dht(DHTs, 11);
  Servo myservo;

  long tim = millis();
  long timh = ((tim % 86400000) / 3600000);

  if (timh % 2 == 0) {
    tmpv = analogRead(TMP);
    tmpc[i] = map(tmpv, 0, 1023, -40, 125);
    humn = dht.readHumidity();
    hum[i] = humn;
  }
  if (Serial.available() > 0) {
    Serial.println("Data collected over");
    Serial.print(timh);
    Serial.print("hours");
    Serial.println("Humidity");
    while (x < sizeof(hum)) {
      Serial.println(hum[x]);
      delay(10);
      x = x + 1;
    }
    x = 0;
    Serial.println("Temperature");
    while (x < sizeof(tmpc)) {
      Serial.println(tmpc[x]);
      delay(10);
      x = x + 1;
    }
    Serial.println("over");
  }
  int i = i + 1;
  delay(5000);
}
