#include <LiquidCrystal.h>

// Configuration de l'écran LCD 16x2
// Connexions: RS, Enable, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Pin du capteur MQ-2
const int MQ2_PIN = A0;

// Variables pour la calibration
int sensorValue = 0;
float voltage = 0;
float ratio = 0;
float ppm = 0;

// Valeurs de référence pour la calibration (à ajuster selon votre environnement)
const float R0 = 10.0; // Résistance de référence en air propre (à calibrer)
const float RL = 5.0;  // Résistance de charge (5kΩ)

void setup() {
  // Initialisation du moniteur série
  Serial.begin(9600);
  
  // Initialisation de l'écran LCD
  lcd.begin(16, 2);
  lcd.print("Capteur Pollution");
  lcd.setCursor(0, 1);
  lcd.print("Initialisation...");
  
  // Temps de chauffe du capteur MQ-2 (recommandé: 20 secondes minimum)
  delay(20000);
  
  lcd.clear();
  lcd.print("Pret!");
  delay(1000);
}

void loop() {
  // Lecture de la valeur analogique du capteur
  sensorValue = analogRead(MQ2_PIN);
  
  // Conversion en tension (0-5V)
  voltage = sensorValue * (5.0 / 1023.0);
  
  // Calcul du ratio Rs/R0
  float Rs = ((5.0 * RL) / voltage) - RL;
  ratio = Rs / R0;
  
  // Estimation approximative en ppm (cette formule est simplifiée)
  // Pour une mesure précise, utilisez les courbes de calibration du fabricant
  ppm = 661.5 * pow(ratio, -1.179);
  
  // Affichage sur l'écran LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Gaz: ");
  lcd.print(ppm, 1);
  lcd.print(" ppm");
  
  lcd.setCursor(0, 1);
  
  // Indication du niveau de pollution
  if (ppm < 50) {
    lcd.print("Qualite: BONNE");
  } else if (ppm < 100) {
    lcd.print("Qualite: MOYENNE");
  } else if (ppm < 200) {
    lcd.print("Qualite: MAUVAISE");
  } else {
    lcd.print("ALERTE POLLUTION!");
  }
  
  // Affichage sur le moniteur série pour debugging
  Serial.print("Valeur capteur: ");
  Serial.print(sensorValue);
  Serial.print(" | Tension: ");
  Serial.print(voltage, 2);
  Serial.print("V | PPM: ");
  Serial.println(ppm, 1);
  
  delay(2000); // Mise à jour toutes les 2 secondes
}