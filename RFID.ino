#include <SPI.h>
#include <MFRC522.h>
constexpr uint8_t RST_PIN = 22;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 21;     // Configurable, see typical pin layout above
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;
String tag;
void setup() {
  Serial.begin(115200);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
  //pinMode(D8, OUTPUT);
}
void loop() {
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    Serial.println(tag);
  
    if (tag == "Enter the Respective ID")//Ex94534224//
    {
      Serial.println("Access Granted!");
      rfid.PICC_HaltA();
      digitalWrite(D5, HIGH);
      delay(100);
      
    } else {
      Serial.println("Access Denied!");
      /*digitalWrite(D8, HIGH);
      delay(2000);
      digitalWrite(D8, LOW);*/
    }
    tag = "";
   // rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
}