// ESP32-S3 Wireless Stick SX1262 OTAA example (EU868)
// Pins: NSS=8, DIO1=14, RST=12, BUSY=13

#include <RadioLib.h>

// SX1262 instance with your board's pinout
SX1262 radio = new Module(8, 14, 12, 13);

// Select the region band (EU868 here)
LoRaWANNode node(&radio, &EU868);



// ---------- OTAA credentials (from TTS) ----------
// LoRaWAN 1.0.x uses AppKey only; set NwkKey to NULL.
// For LoRaWAN 1.1.x, provide both AppKey and NwkKey, and set your device
// in TTS to LoRaWAN 1.1 with matching keys.

uint64_t joinEUI = 0x1234561234561234;        // AppEUI/JoinEUI (MSB), replace
uint64_t devEUI  = 0x70B3D57ED8004CE5;        // DevEUI (MSB), replace

uint8_t  appKey[16] = {
  0x1D, 0x02, 0x21, 0x86, 0xE3, 0x6D, 0xDD, 0x65, 0x69, 0x17, 0x95, 0xFE, 0x17, 0xB7, 0xE6, 0xC9
};




// ------------------------------------------------

void setup() {
  Serial.begin(115200); while(!Serial) {}

  int16_t st = radio.begin();
  if (st != RADIOLIB_ERR_NONE) { Serial.println("radio.begin failed"); while(true){} }

  // LoRaWAN 1.0.x: pass NULL for NwkKey
  st = node.beginOTAA(joinEUI, devEUI, NULL, appKey);
  if (st != RADIOLIB_ERR_NONE) { Serial.println("beginOTAA failed"); while(true){} }

  st = node.activateOTAA();                       // perform join
  // if (st < RADIOLIB_ERR_NONE) { Serial.println("activateOTAA failed");Serial.println(st); while(true){} }
  Serial.print("activateOTAA status = ");
  Serial.println(st);

  delay(5000);

  node.setADR(true);                              // let network manage DR
  Serial.print("DevAddr: 0x"); Serial.println(node.getDevAddr(), HEX);
}

void loop() {
  int randNum = random(50, 101);
  char payload[8];
  snprintf(payload, sizeof(payload), "%d", randNum);

  // Unconfirmed uplink on FPort 1 (avoid FPort 0)
    int16_t st = node.sendReceive(payload, 1, false);
  Serial.print("sendReceive: "); Serial.println(st);

  // Optional: ask for link quality to verify end-to-end path
  node.sendMacCommandReq(RADIOLIB_LORAWAN_MAC_LINK_CHECK);
  uint8_t margin=0, gws=0;
  if (node.getMacLinkCheckAns(&margin, &gws) == RADIOLIB_ERR_NONE) {
    Serial.print("LinkCheck margin="); Serial.print((int)margin);
    Serial.print(" gw="); Serial.println((int)gws);
  }

  // Respect duty cycle
  uint32_t waitMs = max<uint32_t>(node.timeUntilUplink(), 10000);
  delay(waitMs);
}