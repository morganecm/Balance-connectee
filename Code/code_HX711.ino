#include "HX711.h"   
// Bibliothèque HX711 permettant de communiquer
// avec le module HX711 (amplificateur + convertisseur ADC)

// ================== CONNEXIONS HX711 -> ARDUINO UNO ==================
// DT  (DOUT) : sortie données du HX711 vers l'Arduino
// SCK        : horloge envoyée par l'Arduino au HX711
#define DT  3     // Broche numérique 3 de l'Arduino connectée à DOUT
#define SCK 2     // Broche numérique 2 de l'Arduino connectée à SCK

// Création de l'objet HX711
HX711 scale;

// ================== FACTEUR DE CALIBRATION ==================
// Ce facteur permet de convertir la valeur brute du HX711
// en une valeur réelle de poids (en grammes).
// Il dépend de la cellule de charge utilisée.
float calibration_factor = -459.542;

void setup() {
  // Initialisation de la communication série
  // Permet d'afficher les résultats sur le moniteur série
  Serial.begin(9600);

  // Initialisation du HX711 avec les broches définies
  scale.begin(DT, SCK);

  // Application du facteur de calibration
  scale.set_scale(calibration_factor);

  Serial.println("=== BALANCE HX711 ===");
  Serial.println("Retirez tout poids de la balance...");
  delay(2000);

  // Mise à zéro (tare)
  // Permet de considérer le poids actuel comme référence zéro
  scale.tare();
  Serial.println("Tare OK !");
  Serial.println();
}

void loop() {

  // Vérifie si le HX711 est prêt à envoyer des données
  if (!scale.is_ready()) {
    Serial.println("ERREUR : HX711 non detecte !");
    delay(500);
    return;
  }

  // ================== LECTURE BRUTE ==================
  // Lecture directe du convertisseur ADC (valeur brute)
  // Utile pour le debug ou la calibration
  long brut = scale.read();

  // ================== LECTURE CALIBRÉE ==================
  // Lecture moyenne de 5 mesures, convertie en grammes
  float poids_g = scale.get_units(5);

  // Conversion du poids en kilogrammes
  float poids_kg = poids_g / 1000.0;

  // ================== AFFICHAGE DES RÉSULTATS ==================
  Serial.print("Brut : ");
  Serial.print(brut);
  Serial.print(" | Poids : ");
  Serial.print(poids_g, 2);
  Serial.print(" g  |  ");
  Serial.print(poids_kg, 3);
  Serial.println(" kg");

  // Délai entre deux mesures
  delay(300);
}
