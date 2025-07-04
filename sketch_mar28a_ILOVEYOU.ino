#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int MQ2_PIN = A0;

int sensorValue = 0;
float voltage = 0;
float ratio = 0;
float ppm = 0;

const float R0 = 10.0;
const float RL = 5.0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Capteur Pollution");
  lcd.setCursor(0, 1);
  lcd.print("Initialisation...");
  delay(20000);
  lcd.clear();
  lcd.print("Pret!");
  delay(1000);
}

void loop() {
  sensorValue = analogRead(MQ2_PIN);
  voltage = sensorValue * (5.0 / 1023.0);
  float Rs = ((5.0 * RL) / voltage) - RL;
  ratio = Rs / R0;
  ppm = 661.5 * pow(ratio, -1.179);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gaz: ");
  lcd.print(ppm, 1);
  lcd.print(" ppm");
  lcd.setCursor(0, 1);

  if (ppm < 50) {
    lcd.print("Qualite: BONNE");
  } else if (ppm < 100) {
    lcd.print("Qualite: MOYENNE");
  } else if (ppm < 200) {
    lcd.print("Qualite: MAUVAISE");
  } else {
    lcd.print("ALERTE POLLUTION!");
  }

  Serial.print("Valeur capteur: ");
  Serial.print(sensorValue);
  Serial.print(" | Tension: ");
  Serial.print(voltage, 2);
  Serial.print("V | PPM: ");
  Serial.println(ppm, 1);

  delay(2000);
}
