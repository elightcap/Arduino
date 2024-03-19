#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>


const int analogPin = D6;  // Define the analog input pin you want to use
const int digitalPin = D1; // Define the digital pin you want to use
const String server = "http://10.0.40.30/unlock";

const char* wifi_ssid = "Home";
const char* wifi_passwd = "";

void setup() {
  Serial.begin(9600);
  pinMode(analogPin, INPUT_PULLUP);    // Configure the analog pin as input
  pinMode(digitalPin, OUTPUT);  // Configure the digital pin as output

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
}

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

void send_request(){
    WiFiClient client;
    HTTPClient http;

    http.begin(client, server.c_str());
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String httpRequestData = "api_key=";
    int httpRes = http.POST(httpRequestData);
    Serial.println(httpRes);
    http.end();
    delay(100);
}

void loop() {
  int inputValue = digitalRead(analogPin); // Read analog value if needed
  Serial.println(inputValue);
  if(inputValue == 0) {
    bool setOn = true;
    send_request();
    while(setOn){
      int inputValue = digitalRead(analogPin);
      if( inputValue == 1){
        bool setOn = false;
      }
      delay(100);
    }
  }
  else if(inputValue == 1) {
    digitalWrite(digitalPin, HIGH);
  }
}
