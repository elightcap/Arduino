#include <ESP8266WebServer.h>

#define HTTP_REST_PORT 80
#define WIFI_RETRY_DELAY 500
#define MAX_WIFI_INIT_RETRY 50


const char* wifi_ssid = "Home";
const char* wifi_passwd = "R00thecat18";
const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0
int sensorValue = 0;  // value read from the pot

ESP8266WebServer http_rest_server(HTTP_REST_PORT);

int init_wifi() {
    int retries = 0;

    Serial.println("Connecting to WiFi AP..........");

    WiFi.mode(WIFI_STA);
    WiFi.begin(wifi_ssid, wifi_passwd);
    // check the status of WiFi connection to be WL_CONNECTED
    while ((WiFi.status() != WL_CONNECTED) && (retries < MAX_WIFI_INIT_RETRY)) {
        retries++;
        delay(WIFI_RETRY_DELAY);
        Serial.print("#");
    }
    return WiFi.status(); // return the WiFi connection status
}

void post_unlock() {
    Serial.print("hello");
     if (http_rest_server.method() == HTTP_POST) {
        Serial.print("post");
        digitalWrite(D8, HIGH); // sets the digital pin 13 on
        delay(1000);            // waits for a second
        digitalWrite(D8, LOW);  // sets the digital pin 13 off
        delay(1000);            // waits for a second
        http_rest_server.send(200);
     }
    else {
        http_rest_server.send(404);
    }
}

void config_rest_server_routing() {
    http_rest_server.on("/", HTTP_GET, []() {
        http_rest_server.send(200, "text/html",
            "Welcome to the ESP8266 REST Web Server");
    });
    http_rest_server.on("/unlock", HTTP_POST, post_unlock);
}

void setup(void) {
    Serial.begin(115200);

    pinMode(D8, OUTPUT);
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

    config_rest_server_routing();

    http_rest_server.begin();
    Serial.println("HTTP REST Server Started");
}

void loop(void) {
    http_rest_server.handleClient();
    sensorValue = analogRead(analogInPin);
    Serial.print("sensor = ");
    Serial.println(sensorValue);
    delay(100);
    if (sensorValue> 700) {
      digitalWrite(D8, HIGH);
    }
    else {
      digitalWrite(D8, LOW);
    }
}
