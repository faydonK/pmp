#include <LiquidCrystal.h>
#define MQ_PIN A0 // Broche analogique à laquelle le MQ-2 est connecté
#include "rgb_lcd.h"

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

void setup() {
  Serial.begin(9600);
   lcd.begin(16, 2);

    lcd.setRGB(colorR, colorG, colorB);
    // Print a message to the LCD.
    lcd.print("Pollution d'air:");

    delay(1000);
}

void loop() {
  float sensorVoltage = analogRead(MQ_PIN) * (5.0 / 1023.0); // Conversion de la valeur analogique en tension
  float RS_air = (5.0 - sensorVoltage) / sensorVoltage; // Résistance du capteur en air propre
  float ratio = RS_air / ((5.0 / sensorVoltage) - 1.0); // Ratio RS/R0

  // Utilisation du ratio pour estimer les concentrations de gaz
  float CH4_Concentration = 2.5 * pow(ratio, -1.29);
  float C4H10_Concentration = 2.8 * pow(ratio, -1.21);
  float C3H8_Concentration = 2.1 * pow(ratio, -1.16);
  float H2_Concentration = 2.0 * pow(ratio, -1.24);
  float Alcohol_Concentration = 3.5 * pow(ratio, -1.25);
  float Smoke_Concentration = 2.5 * pow(ratio, -1.29);

  // Affichage des concentrations estimées
  lcd.print("Méthane: ");
  lcd.println(CH4_Concentration);
  delay(100000);
  lcd.setCursor(0, 1);
  lcd.print("Butane: ");
  lcd.println(C4H10_Concentration);
  delay(100000);
  lcd.setCursor(0, 1);
  lcd.print("GPL (Propane + Butane) : ");
  lcd.println(C3H8_Concentration);
  delay(100000);
  lcd.setCursor(0, 1);
  lcd.print("Hydrogene: ");
  lcd.println(H2_Concentration);
  delay(100000);
  lcd.setCursor(0, 1);
  lcd.print("Alcools: ");
  lcd.println(Alcohol_Concentration);
  delay(100000);
  lcd.setCursor(0, 1);
  lcd.print("Fumees: ");
  lcd.println(Smoke_Concentration);
  delay(100000); // Attendre 1 seconde entre chaque lecture
}
