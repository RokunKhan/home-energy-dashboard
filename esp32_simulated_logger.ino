#include <WiFi.h>
#include <HTTPClient.h>

// Wi-Fi network credentials
const char* ssid = "Fios-SDzM8";
const char* password = "Rokunkhan21";

// ThingSpeak Write API Key
String apiKey = "583FG9T68JWR8A5G";
const char* server = "http://api.thingspeak.com/update";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Simulated sensor data
    float current = random(0, 1500) / 100.0;  // 0.00 to 15.00 A
    float power = current * 120.0;           // Est. power using 120V

    String url = server;
    url += "?api_key=" + apiKey;
    url += "&field1=" + String(current, 2);
    url += "&field2=" + String(power, 2);

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.println("Data sent successfully!");
      Serial.println("Current: " + String(current, 2) + " A");
      Serial.println("Power: " + String(power, 2) + " W\n");
    } else {
      Serial.println("Failed to send data.");
    }

    http.end();
  }

  delay(15000);  // 15 sec delay (ThingSpeak limit)
}
