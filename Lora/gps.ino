#include <TinyGPSPlus.h>     // Bibliothèque à installer : "TinyGPSPlus" (via le gestionnaire Arduino)
#define GNSS_RX 18           // RX de l’ESP32-S3 (connecté au TX du module u-blox)
#define GNSS_TX -1           // pas utilisé (pas de retour vers le GNSS)

HardwareSerial GNSS_Serial(1);   // UART1 sur ESP32-S3
TinyGPSPlus gps;                 // Objet GPS

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("\n=== Test GNSS ESP32-S3 ===");

  GNSS_Serial.begin(9600, SERIAL_8N1, GNSS_RX, GNSS_TX);
  Serial.println("Lecture du GNSS à 9600 baudskkk...");
}

void loop() {
  while (GNSS_Serial.available()) {
    char c = GNSS_Serial.read();
    gps.encode(c);    // Envoie chaque caractère au décodeur GPS
  }

  // Dès qu’une nouvelle position est disponible
  if (gps.location.isUpdated()) {
    Serial.print("Latitude : ");
    Serial.println(gps.location.lat(), 6); // 6 décimales = ~10 cm de précision
    Serial.print("Longitude : ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Altitude : ");
    Serial.println(gps.altitude.meters());
    Serial.print("Satellites : ");
    Serial.println(gps.satellites.value());
    Serial.println("---------------------------");
  }
}
