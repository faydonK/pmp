#include <LiquidCrystal.h>

// Configuration
#define MQ_PIN A0
#define R0 10.0

// Grove LCD - pins standards
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Variables
int loopCount = 0;
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 2000; // 2 secondes

void setup() {
  // Console de debug Arduino IDE
  Serial.begin(9600);
  Serial.println("=== ARDUINO UNO - CAPTEUR POLLUTION ===");
  Serial.println("Grove LCD + MQ-2 initialisé");
  Serial.println("Debug: Ouvrir Moniteur Série (Ctrl+Shift+M)");
  Serial.println("=====================================");
  
  // LCD
  Serial.println("DEBUG: Initialisation LCD...");
  lcd.begin(16, 2);
  
  // Message fixe sur l'écran
  lcd.setCursor(0, 0);
  lcd.print("Pollution:");
  Serial.println("DEBUG: 'Pollution:' affiché ligne 1");
  
  // Test initial capteur
  int testValue = analogRead(MQ_PIN);
  Serial.print("DEBUG: Test capteur initial = ");
  Serial.println(testValue);
  
  delay(1000);
  Serial.println("DEBUG: Setup terminé, début loop...");
  Serial.println("");
}

void loop() {
  loopCount++;
  
  // DEBUG: Info loop
  if (loopCount % 20 == 1) { // Tous les 20 loops
    Serial.print("--- LOOP #");
    Serial.print(loopCount);
    Serial.println(" ---");
  }
  
  // === LECTURE CAPTEUR ===
  Serial.println("DEBUG: Lecture capteur MQ-2...");
  int sensorValue = analogRead(MQ_PIN);
  Serial.print("  Valeur brute: ");
  Serial.println(sensorValue);
  
  float voltage = sensorValue * (5.0 / 1023.0);
  Serial.print("  Tension: ");
  Serial.print(voltage, 3);
  Serial.println(" V");
  
  // === CALCULS ===
  Serial.println("DEBUG: Calculs de pollution...");
  
  // Vérification erreur
  if (voltage <= 0.1) {
    Serial.println("  ERREUR: Tension trop faible!");
    lcd.setCursor(0, 1);
    lcd.print("ERREUR CAPTEUR  ");
    delay(1000);
    return;
  }
  
  // Calcul résistance
  float RS = ((5.0 * 10.0) / voltage) - 10.0;
  Serial.print("  Résistance: ");
  Serial.print(RS, 2);
  Serial.println(" kΩ");
  
  float ratio = RS / R0;
  Serial.print("  Ratio RS/R0: ");
  Serial.println(ratio, 3);
  
  // Calcul pollution générale (moyenne des gaz)
  float pollution = 0;
  pollution += 1000 * pow(ratio, -1.29);  // CH4
  pollution += 1000 * pow(ratio, -1.21);  // Butane
  pollution += 1000 * pow(ratio, -1.16);  // Propane
  pollution += 1000 * pow(ratio, -1.24);  // H2
  pollution += 1000 * pow(ratio, -1.25);  // Alcool
  pollution += 1000 * pow(ratio, -1.29);  // Fumée
  pollution = pollution / 6.0; // Moyenne
  
  Serial.print("  Pollution calculée: ");
  Serial.print(pollution, 1);
  Serial.println(" ppm");
  
  // === MISE À JOUR ÉCRAN ===
  unsigned long now = millis();
  if (now - lastUpdate >= updateInterval) {
    Serial.println("DEBUG: Mise à jour écran LCD...");
    
    // Ligne 2: valeur de pollution
    lcd.setCursor(0, 1);
    
    if (pollution < 1000) {
      lcd.print(pollution, 0);
      lcd.print(" ppm      "); // Espaces pour effacer ancien texte
      Serial.print("  Affiché: ");
      Serial.print(pollution, 0);
      Serial.println(" ppm");
    } else {
      lcd.print("DANGER!       ");
      Serial.println("  Affiché: DANGER!");
    }
    
    lastUpdate = now;
    Serial.println("DEBUG: Écran mis à jour");
  }
  
  // === ANALYSE QUALITÉ AIR ===
  Serial.print("DEBUG: Qualité air = ");
  if (pollution < 50) {
    Serial.println("BONNE");
  } else if (pollution < 100) {
    Serial.println("MOYENNE");
  } else if (pollution < 200) {
    Serial.println("MAUVAISE");
  } else {
    Serial.println("DANGEREUSE");
  }
  
  Serial.println(""); // Ligne vide pour lisibilité
  
  delay(500); // Attente 0.5 seconde
}