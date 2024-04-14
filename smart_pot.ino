#define BLYNK_TEMPLATE_ID "TMPL6r7KMFYUA"
#define BLYNK_TEMPLATE_NAME "plant3"
#define BLYNK_AUTH_TOKEN "dxGnJyksuaq0Prhgh99kkXZsPlIFWU9p"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <WiFiManager.h> // Include the WiFiManager library

char auth[] = BLYNK_AUTH_TOKEN;
BlynkTimer timer;

int pot=32;
int pot2=35;
int pot3=34;
int ldr=33;
#define TRIGGER_PIN 23
bool wifiConfigRequested = false;

void setup()
{
  //Serial.begin(9600);
    // Set up pins
    pinMode(pot, INPUT);
    pinMode(pot2, INPUT);
    pinMode(pot3, INPUT);
    pinMode(ldr, INPUT);
    pinMode(TRIGGER_PIN, INPUT_PULLUP);
    
    connectToWifi();
}

void loop()
{
//Serial.println(digitalRead(TRIGGER_PIN));
    if ( digitalRead(TRIGGER_PIN) == LOW) {
    wifiConfigRequested = true; // Set the flag to indicate WiFi configuration is requested
  }

  if(WiFi.status() !=WL_CONNECTED){
    //Serial.println("connection lost and i will reconnect");
    wifiConfigRequested = true;
    }

  if (wifiConfigRequested) {
    // If WiFi configuration is requested, invoke the connectToWiFi function
    connectToWifi();
    wifiConfigRequested = false; // Reset the flag after WiFi configuration
  }
    
    Blynk.run();

    int lower = ((4095-analogRead(pot))/40.95)*1.25;
    Blynk.virtualWrite(V1, lower);

    int mid = ((4095-analogRead(pot2))/40.95)*1.25;
    Blynk.virtualWrite(V2, mid);

    int up = ((4095-analogRead(pot3))/40.95)*1.25;
    Blynk.virtualWrite(V3, up);

    int avg = ((lower + mid + up) / 3);
    Blynk.virtualWrite(V4, avg);

    int ldrval = analogRead(ldr)/40.95;
    Blynk.virtualWrite(V0, ldrval);

    if (avg < 15) {
        Blynk.logEvent("i_need_water");
    } else if (mid < 20) {
        Blynk.logEvent("middle_sensor_is_thirsty");
    }
}

void connectToWifi(){
  // Start WiFiManager
    WiFiManager wm;
    wm.setConfigPortalTimeout(150);
    if (!wm.autoConnect("yousefpot3")) {
        ESP.restart();
    }

    // Initialize Blynk
    Blynk.begin(auth, WiFi.SSID().c_str(), WiFi.psk().c_str());
  }
