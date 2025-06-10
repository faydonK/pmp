#include "rgb_lcd.h"  // Gardons seulement cette bibliothèque

#define MQ_PIN A0
#define R0 10.0  // Résistance de référence en air propre (à calibrer)

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

int currentGas = 0;  // Index du gaz actuellement affiché
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 3000;  // 3 secondes entre chaque gaz

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
  
  // Message initial
  lcd.setCursor(0, 0);
  lcd.print("Pollution d'air:");
  lcd.setCursor(0, 1);
  lcd.print("Initialisation..");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Lecture du capteur
  int sensorValue = analogRead(MQ_PIN);
  float sensorVoltage = sensorValue * (5.0 / 1023.0);
  
  // Calcul de la résistance du capteur
  float RS_gas = ((5.0 * 10.0) / sensorVoltage) - 10.0;  // Avec une résistance de charge de 10kΩ
  float ratio = RS_gas / R0;
  
  // Calcul des concentrations (formules approximatives)
  float concentrations[6];
  concentrations[0] = 1000 * pow(ratio, -1.29);  // CH4 (Méthane)
  concentrations[1] = 1000 * pow(ratio, -1.21);  // C4H10 (Butane)
  concentrations[2] = 1000 * pow(ratio, -1.16);  // C3H8 (Propane)
  concentrations[3] = 1000 * pow(ratio, -1.24);  // H2 (Hydrogène)
  concentrations[4] = 1000 * pow(ratio, -1.25);  // Alcool
  concentrations[5] = 1000 * pow(ratio, -1.29);  // Fumée
  
  String gasNames[6] = {"Methane", "Butane", "Propane", "Hydrogene", "Alcool", "Fumee"};
  
  // Affichage rotatif toutes les 3 secondes
  if (millis() - lastUpdate >= updateInterval) {
    lcd.clear();
    
    // Première ligne : nom du gaz
    lcd.setCursor(0, 0);
    lcd.print(gasNames[currentGas]);
    
    // Deuxième ligne : concentration
    lcd.setCursor(0, 1);
    lcd.print(concentrations[currentGas], 1);
    lcd.print(" ppm");
    
    // Affichage série pour débogage
    Serial.print(gasNames[currentGas]);
    Serial.print(": ");
    Serial.print(concentrations[currentGas]);
    Serial.println(" ppm");
    
    currentGas = (currentGas + 1) % 6;  // Passer au gaz suivant
    lastUpdate = millis();
  }
  
  delay(100);  // Petit délai pour éviter la surcharge
}