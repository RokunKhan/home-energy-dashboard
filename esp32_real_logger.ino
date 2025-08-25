#include <WiFi.h>
#include <HTTPClient.h>
#include <EmonLib.h>

const char* WIFI = "Fios-SDzM8";
const char* WIFI_PASS = "Rokunkhan21";
const char* API_KEY = "583FG9T68JWR8A5G";
const double MAINS_V = 120.0;  // assumed voltage
const double PF_GUESS = 1.0;    
const int    POST_PERIOD_MS = 20000;

const int   ADC_PIN = 34;    
double      CALIB   = 100;   
const int   SAMPLES = 4000;  

EnergyMonitor emon1;
unsigned long lastPost = 0;

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  unsigned long t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < 15000) {
    delay(300);
  }
}

void setup() {
  Serial.begin(115200);
  connectWiFi();
  analogSetPinAttenuation(ADC_PIN, ADC_11db);
  emon1.current(ADC_PIN, CALIB);
}

double readIrmsAvg(int n) {
  double s = 0;
  for (int i = 0; i < n; i++) s += emon1.calcIrms(SAMPLES);
  return s / n;
}

void loop() {
  double I_rms = readIrmsAvg(5);        
  double VA    = I_rms * MAINS_V;       
  double W     = VA * PF_GUESS;         

  // Print locally
  Serial.print("I_rms=");
  Serial.print(I_rms, 4);
  Serial.print(" A  VA=");
  Serial.print(VA, 2);
  Serial.print("  W~=");
  Serial.println(W, 2);

  if (millis() - lastPost >= POST_PERIOD_MS && WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://api.thingspeak.com/update";  
    String payload = String("api_key=") + WRITE_API_KEY +
                     "&field1=" + String(I_rms, 6) +
                     "&field2=" + String(VA, 3) +
                     "&field3=" + String(W, 3);

    http.begin(url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int code = http.POST(payload);
    Serial.print("ThingSpeak response: "); Serial.println(code);
    http.end();

    lastPost = millis();
  } else if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  delay(500);
}
