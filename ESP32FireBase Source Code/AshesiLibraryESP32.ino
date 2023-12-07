#include <SPI.h>
#include <MFRC522.h>
#include <FirebaseESP32.h>

#define SS_PIN 5
#define RST_PIN 22
#define Buzzer 2

MFRC522 rfid(SS_PIN, RST_PIN);

/* Firebase Configuration */
#define WIFI_SSID "###"
#define WIFI_PASSWORD "###"
#define API_KEY "###"
#define DATABASE_URL "###"
#define USER_EMAIL "###"
#define USER_PASSWORD "###"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  Serial.begin(9600);
  Serial.println("Please put your card..");

  SPI.begin();
  rfid.PCD_Init();
  pinMode(Buzzer, OUTPUT);

  // Firebase setup
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  Firebase.reconnectWiFi(true);
  Firebase.begin(&config, &auth);
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  Serial.print("NUID tag is :");
  tone(Buzzer, 1000);
  delay(1000);
  noTone(Buzzer);

  String cardID = "";
  cardID.reserve(16);
  for (byte i = 0; i < rfid.uid.size; i++) {
    cardID += (rfid.uid.uidByte[i] < 0x10 ? "0" : "");
    cardID += String(rfid.uid.uidByte[i], HEX);
  }
  cardID.toUpperCase();

  // Check Firebase for book approval
  String path = "/LibraryRecords/" + cardID;
  if (Firebase.getBool(fbdo, path + "/approval")) {
    bool approval = fbdo.to<bool>();
    
    if (Firebase.getBool(fbdo, path + "/buzzerStatus")) {
      bool buzzerStatus = fbdo.to<bool>();
      // Serial.println("buzzerStatus");
      // Serial.println(buzzerStatus);
      // Serial.println("approval");
      // Serial.println(approval);
      
      if (approval == 1 && buzzerStatus == 0) {
        Serial.println("Approved");
      } else {
        Serial.println("Person taking book out of the library");
        tone(Buzzer, 1000);
        delay(5000);
        noTone(Buzzer);
      }
    }
  } else {
    Serial.println("Unknown book");
  }
}
