#include <LiquidCrystal.h>

// ========================================
// CONFIGURATION DES PINS
// ========================================

// CAPTEUR MQ-2 (Détecteur de gaz)
#define MQ_PIN A0    // Pin analogique A0

// ÉCRAN LCD 16x2 (6 pins requis)
// Format: LiquidCrystal(RS, Enable, D4, D5, D6, D7)
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//                |   |   |  |  |  |
//                |   |   |  |  |  └── Pin 2 (D7)
//                |   |   |  |  └───── Pin 3 (D6)  
//                |   |   |  └──────── Pin 4 (D5)
//                |   |   └─────────── Pin 5 (D4)
//                |   └───────────── Pin 11 (Enable)
//                └───────────────── Pin 12 (RS)

// ========================================
// BRANCHEMENTS DÉTAILLÉS
// ========================================
/*
ÉCRAN LCD 16x2 → ARDUINO UNO
┌─────────────┬─────────────┬─────────────────┐
│ Pin LCD     │ Arduino     │ Description     │
├─────────────┼─────────────┼─────────────────┤
│ Pin 1 (VSS) │ GND         │ Masse           │
│ Pin 2 (VDD) │ 5V          │ Alimentation    │
│ Pin 3 (V0)  │ GND         │ Contraste ⚠️    │
│ Pin 4 (RS)  │ Pin 12      │ Register Select │
│ Pin 5 (E)   │ Pin 11      │ Enable          │
│ Pin 6-9     │ (vide)      │ Non utilisés    │
│ Pin 10 (D4) │ Pin 5       │ Données bit 4   │
│ Pin 11 (D5) │ Pin 4       │ Données bit 5   │
│ Pin 12 (D6) │ Pin 3       │ Données bit 6   │
│ Pin 13 (D7) │ Pin 2       │ Données bit 7   │
│ Pin 14 (A)  │ 5V          │ Rétroéclairage+ │
│ Pin 15 (K)  │ GND         │ Rétroéclairage- │
└─────────────┴─────────────┴─────────────────┘

CAPTEUR MQ-2 → ARDUINO UNO
┌─────────────┬─────────────┬─────────────────┐
│ MQ-2        │ Arduino     │ Description     │
├─────────────┼─────────────┼─────────────────┤
│ VCC         │ 5V          │ Alimentation    │
│ GND         │ GND         │ Masse           │
│ A0          │ A0          │ Signal analogique│
└─────────────┴─────────────┴─────────────────┘

⚠️ IMPORTANT: Pin 3 (V0) du LCD DOIT être sur GND pour éviter les carrés !
*/

// ========================================
// VARIABLES GLOBALES
// ========================================
unsigned long derniereMesure = 0;
const unsigned long INTERVALLE = 1000;  // 1 seconde

// ========================================
// FONCTION SETUP (EXÉCUTÉE UNE FOIS)
// ========================================
void setup() {
  // === INITIALISATION COMMUNICATION SÉRIE ===
  Serial.begin(9600);
  Serial.println("╔══════════════════════════════════════╗");
  Serial.println("║      CAPTEUR POLLUTION MQ-2          ║");
  Serial.println("║      Arduino UNO + LCD 16x2          ║");
  Serial.println("╚══════════════════════════════════════╝");
  Serial.println();
  
  // === INITIALISATION LCD ===
  Serial.println("🔧 Initialisation de l'écran LCD...");
  delay(1000);        // Attente stabilisation
  lcd.begin(16, 2);   // 16 colonnes, 2 lignes
  delay(500);
  lcd.clear();        // Effacer l'écran
  delay(200);
  
  // === TEST LCD ===
  Serial.println("🧪 Test d'affichage LCD...");
  lcd.setCursor(0, 0);
  lcd.print("Demarrage...");
  delay(1000);
  lcd.clear();
  
  // === AFFICHAGE TITRE PERMANENT ===
  lcd.setCursor(0, 0);
  lcd.print("Pollution:");
  Serial.println("✅ LCD initialisé avec succès");
  
  // === TEST CAPTEUR INITIAL ===
  Serial.println("🔍 Test du capteur MQ-2...");
  int valeurTest = analogRead(MQ_PIN);
  Serial.print("   Valeur initiale: ");
  Serial.println(valeurTest);
  
  if (valeurTest == 0) {
    Serial.println("⚠️  ATTENTION: Capteur peut-être déconnecté");
  } else {
    Serial.println("✅ Capteur MQ-2 détecté");
  }
  
  Serial.println();
  Serial.println("🚀 Système prêt - Début des mesures...");
  Serial.println("=====================================");
}

// ========================================
// FONCTION LOOP (EXÉCUTÉE EN BOUCLE)
// ========================================
void loop() {
  // === LECTURE DU CAPTEUR ===
  int valeurBrute = analogRead(MQ_PIN);
  
  // === CONVERSION EN PPM ===
  // Conversion simple: 0-1023 → 0-500 ppm
  float niveauPollution = map(valeurBrute, 0, 1023, 0, 500);
  
  // === AFFICHAGE SUR LCD ===
  lcd.setCursor(0, 1);  // Ligne 2, colonne 1
  
  // Effacer la ligne en écrivant des espaces
  lcd.print("                ");  // 16 espaces
  lcd.setCursor(0, 1);  // Revenir au début de la ligne 2
  
  // Afficher selon le niveau de pollution
  if (niveauPollution < 100) {
    lcd.print(niveauPollution, 0);  // 0 décimale
    lcd.print(" ppm BON");
  } 
  else if (niveauPollution < 200) {
    lcd.print(niveauPollution, 0);
    lcd.print(" ppm MOYEN");
  } 
  else if (niveauPollution < 300) {
    lcd.print(niveauPollution, 0);
    lcd.print(" ppm MAUVAIS");
  }
  else {
    lcd.print(niveauPollution, 0);
    lcd.print(" ppm DANGER");
  }
  
  // === AFFICHAGE CONSOLE (DEBUG) ===
  Serial.print("📊 Mesure: ");
  Serial.print(valeurBrute);
  Serial.print(" (brut) → ");
  Serial.print(niveauPollution, 0);
  Serial.print(" ppm");
  Serial.print(" [");
  
  // Indicateur qualité air
  if (niveauPollution < 100) {
    Serial.print("BON");
  } else if (niveauPollution < 200) {
    Serial.print("MOYEN");
  } else if (niveauPollution < 300) {
    Serial.print("MAUVAIS");  
  } else {
    Serial.print("DANGER");
  }
  Serial.println("]");
  
  // === ATTENTE AVANT PROCHAINE MESURE ===
  delay(INTERVALLE);
}