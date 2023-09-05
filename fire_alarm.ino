#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial SIM900A(2, 3);

const int redLed = 4;
int greenLed = 6;
const int buzzer = 5;
int smokeA0 = A0;

const int flame = 2;
int Flame = HIGH;


int sensorThres = 100;

void setup() {

  pinMode(redLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(smokeA0, INPUT);
  pinMode(flame, INPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  Serial.begin(9600);
  lcd.begin(16, 2);

  SIM900A.begin(9600);
  Serial.begin(9600);
}

void loop() {
  int analogSensor = analogRead(smokeA0);
  Serial.print("Pin A0: ");
  Serial.println(analogSensor);
  lcd.print("Smoke Level: ");
  lcd.print(analogSensor - 50);
  lcd.setCursor(0, 1);
  lcd.print("Flame low");

  Flame = digitalRead(flame);
  if (Flame == LOW)
  {
    lcd.setCursor(0, 1);
    lcd.print("Flame High");
    digitalWrite(redLed, HIGH);
    delay(5000);
    digitalWrite(buzzer, HIGH);
    delay(5000);
    digitalWrite(greenLed, LOW);
  }

  if (analogSensor - 50 > sensorThres)
  {
    digitalWrite(redLed, HIGH);
    digitalWrite(buzzer, HIGH);

    SIM900A.println("AT+CMGF=1");
    delay(1000);
    SIM900A.print("AT+CMGS=\"+918960471289\"\r");
    delay(1000);
    SIM900A.print("Flre And Smoke Alert");
    delay(100);
    SIM900A.println((char)26);

    digitalWrite(greenLed, LOW);

  }
  else
  {
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
    digitalWrite(buzzer, LOW);

  }
  delay(500);
  lcd.clear();
}
