#include <LiquidCrystal.h>

// Capteur MQ-2 sur A0
#define MQ_PIN A0

// LCD sur pins 12,11,5,4,3,2
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // Console debug
  Serial.begin(9600);
  Serial.println("=== CAPTEUR POLLUTION DÉMARRÉ ===");
  
  // LCD - initialisation lente pour éviter les carrés
  delay(1000);
  lcd.begin(16, 2);
  delay(500);
  lcd.clear();
  delay(200);
  
  // Test LCD avec caractères simples
  lcd.setCursor(0, 0);
  lcd.print("Test...");
  delay(1000);
  lcd.clear();
  
  // Message final
  lcd.setCursor(0, 0);
  lcd.print("Pollution:");
  
  Serial.println("LCD initialisé - Prêt à mesurer");
  
  // Test capteur initial
  int test = analogRead(MQ_PIN);
  Serial.print("Capteur MQ-2 test: ");
  Serial.println(test);
}

void loop() {
  // Lecture capteur
  int valeur = analogRead(MQ_PIN);
  
  // Conversion simple en pollution (0-500 ppm)
  float pollution = map(valeur, 0, 1023, 0, 500);
  
  // Affichage sur LCD ligne 2
  lcd.setCursor(0, 1);
  if (pollution < 100) {
    lcd.print(pollution, 0);
    lcd.print(" ppm BON   ");
  } else if (pollution < 200) {
    lcd.print(pollution, 0);
    lcd.print(" ppm MOYEN ");
  } else {
    lcd.print(pollution, 0);
    lcd.print(" ppm DANGER");
  }
  
  // Debug console
  Serial.print("Val
