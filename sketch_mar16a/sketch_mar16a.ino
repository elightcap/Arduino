#include <ESP8266WebServer.h>

#define HTTP_REST_PORT 80
#define WIFI_RETRY_DELAY 500
#define MAX_WIFI_INIT_RETRY 50

const char* wifi_ssid = "Home";
const char* wifi_passwd = "";

const int potPin = A0;    // Analog pin connected to the potentiometer
const int pwmPin = D8;   

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
    digitalWrite(D2, HIGH);
    Serial.println("ON");
    delay(1000);            
    digitalWrite(D2, LOW);
    Serial.println("OFF");  
    delay(1000);            
    http_rest_server.send(200);

}

void config_rest_server_routing() {
    http_rest_server.on("/", HTTP_GET, []() {
        http_rest_server.send(200, "text/html",
            "Welcome to the ESP8266 REST Web Server");
    });
    http_rest_server.on("/unlock", HTTP_POST, post_unlock);
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

  config_rest_server_routing();

  http_rest_server.begin();
  Serial.println("HTTP REST Server Started");
}

void loop(){
  http_rest_server.handleClient();
  int potValue = analogRead(potPin);
  int pwmValue = map(potValue, 350, 700, 0, 255);
  //Serial.print("Duty Cycle: ");
  //Serial.println(potValue);
  if(pwmValue < 10) {
    analogWrite(pwmPin, 0);
  }
  else if(pwmValue >= 250) {
    bool unlocked = true;
    post_unlock();
    analogWrite(pwmPin, pwmValue);
    while (unlocked) {
      int potValue = analogRead(potPin);
      int pwmValue = map(potValue, 20, 900, 0, 255);
      if(pwmValue < 50) {
        unlocked = false;
      }
      delay(100);
    }
  }
  else {
    analogWrite(pwmPin, pwmValue);
  }
  delay(100);
}
