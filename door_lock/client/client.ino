#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>


const char* wifi_ssid = "Home";
const char* wifi_passwd = "R00thecat18";

const int potPin = A0;    // Analog pin connected to the potentiometer
const int pwmPin = D8;   

String serverName = "http://10.0.40.30/unlock";

int init_wifi() {
    int retries = 0;

    Serial.println("Connecting to WiFi AP..........");

    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, wifi_passwd);
    // check the status of WiFi connection to be WL_CONNECTED
    while ((WiFi.status() != WL_CONNECTED)) {
        retries++;
        delay(200);
        Serial.print("#");
    }
    return WiFi.status(); // return the WiFi connection status
}

void setup(){
  Serial.begin(9600);
  pinMode(D2, OUTPUT);
  
  digitalWrite(D2, HIGH);
  delay(1000); 
  digitalWrite(D2, LOW);
  analogWrite(pwmPin, 512);  /* set initial 50% duty cycle */
  if (init_wifi() == WL_CONNECTED) {
        Serial.print("Connetted to ");
        Serial.print(wifi_ssid);
        Serial.print("--- IP: ");
        Serial.println(WiFi.localIP());
  }
  else {
        Serial.print("Error connecting to: ");
        Serial.println(wifi_ssid);
  }

  Serial.println("HTTP REST Server Started");
}

void loop(){
  if (WiFi.status()==WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverName.c_str());
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String httpRequestData = "api_key=";
    int httpRes = http.POST(httpRequestData);
    Serial.println(httpRes);
    http.end();
    delay(100);
  }
}