// Author - Aditya Raj
// Only for ESP-8266 ONLY...
// If needed, change the libraries compatible with ESP32...

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <FirebaseESP8266.h>

//Wi-Fi credentials
struct WiFiCredentials {
  const char* ssid;
  const char* password;
};

//Wi-Fi networks
WiFiCredentials wifiNetworks[] = {
  {"Aditya-Desk", "12312345"},
  {"Galgotias-H", ""},
  {"GCET-122", "korean@122"}
};

//Firebase
#define API_KEY "AIzaSyCOXPhU-zqTFjuAzEejmhl_-6iXhCO6aiU"
#define DATABASE_URL "https://esp-8266-wifi-rgb-default-rtdb.asia-southeast1.firebasedatabase.app"
#define USER_EMAIL "adityaraj94505@gmail.com"
#define USER_PASSWORD "espwifirgb"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

const int red = D1;
const int blue = D2;
const int green = D3;

int currentEffect = 0;

void connectToWiFi() {
  for (int i = 0; i < 3; i++) {
    WiFi.begin(wifiNetworks[i].ssid, wifiNetworks[i].password);
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(wifiNetworks[i].ssid);

    int timeout = 0;
    while (WiFi.status() != WL_CONNECTED && timeout < 20) {
      delay(500);
      Serial.print(".");
      timeout++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println();
      Serial.print("Connected to ");
      Serial.println(wifiNetworks[i].ssid);
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
      return;
    } else {
      Serial.println();
      Serial.print("Failed to connect to ");
      Serial.println(wifiNetworks[i].ssid);
    }
  }

  Serial.println("Unable to connect to any Wi-Fi network");
}

void setup() {
  Serial.begin(9600);
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(green, OUTPUT);

  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(blue, LOW);

  connectToWiFi();
  Serial.println("");
  Serial.println("WiFi connected");

  WiFiClientSecure client;
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  Firebase.begin(&config, &auth);
}

void updateEffect(int value) {
  switch (value) {
    case 1:
      digitalWrite(red, HIGH);
      digitalWrite(blue, LOW);
      digitalWrite(green, LOW);
      Serial.println("red on");
      break;
    case 2:
      digitalWrite(red, LOW);
      digitalWrite(blue, HIGH);
      digitalWrite(green, LOW);
      Serial.println("blue on");
      break;
    case 3:
      digitalWrite(red, LOW);
      digitalWrite(blue, LOW);
      digitalWrite(green, HIGH);
      Serial.println("green on");
      break;
    case 4:
      digitalWrite(blue, HIGH);
      digitalWrite(green, LOW);
      digitalWrite(red, HIGH);
      Serial.println("red fade");
      break;
    case 5:
      digitalWrite(blue, LOW);
      digitalWrite(green, HIGH);
      digitalWrite(red, HIGH);
      Serial.println("blue fade");
      break;
    case 6:
      digitalWrite(blue, HIGH);
      digitalWrite(green, HIGH);
      digitalWrite(red, LOW);
      Serial.println("green fade");
      break;
    case 7:
    digitalWrite(blue, LOW);
      digitalWrite(green, LOW);
      digitalWrite(red, LOW);
    default:
      digitalWrite(red, LOW);
      digitalWrite(blue, LOW);
      digitalWrite(green, LOW);
      Serial.println("all off");
      break;
  }
}

void loop() {
  if (Firebase.getInt(fbdo, "/Value1")) {
    if (fbdo.dataType() == "int") {
      int newEffect = fbdo.intData();
      if (newEffect != currentEffect) {
        currentEffect = newEffect;
        updateEffect(currentEffect);
      }
    } else {
      Serial.println("Data is invalid or not an integer");
    }
  } else {
    Serial.print("Failed to get value: ");
    Serial.println(fbdo.errorReason());
  }

  delay(500); //delay
}
