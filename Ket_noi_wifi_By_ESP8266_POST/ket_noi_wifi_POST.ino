#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <string.h>
//#include <ESP8266WiFiMulti.h>
//ESP8266WiFiMulti WiFiMulti;

//const char* ssid="Realme3pro";
//const char* password = "144000cc";

const char* ssid = "D-Link_DIR-600";
const char* password = "";

int ledPin = 2;
String inputData = "";
bool stringComplete = false;

void initWiFi() {
  Serial.println();
  Serial.print("Wifi connecting to ");
  Serial.println( ssid );

  //  WiFi.begin(ssid,password);
  WiFi.begin(ssid);
  Serial.println();
  Serial.print("Connecting");

  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
  }
  digitalWrite( ledPin , HIGH);
  Serial.println();

  Serial.println("Wifi Connected Success!");
  Serial.print("NodeMCU IP Address : ");
  Serial.println(WiFi.localIP() );
  // fix connect lost
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
}

void setup() {

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.begin(115200);
  initWiFi();
}

void loop() {
  // put your main code here, to run repeatedly:
  Read_UART();
}

void Read_UART() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputData += inChar;
    if ( inChar == '\n') {
      stringComplete = true;
    }

    if ( stringComplete) { //after get all character
      //send Data to server
      sendDataFromWiFi(inputData);
      // reset data
      inputData = "";
      stringComplete = false;
    }
  }
}

void sendDataFromWiFi(String inputData) {
  int pos = inputData.indexOf("*");
  String url = inputData.substring(0, pos);
  inputData =  inputData.substring( pos + 1);
  if ( WiFi.status() == WL_CONNECTED) {

    WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin ... \n");

    Serial.print(url +"?"+ inputData);
    http.begin(client, url.c_str());
    Serial.println("[HTTP] POST ... \n");
    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    // Data to send with HTTP POST
    String httpRequestData = inputData;

    //start connnection add send HTTP header
    int httpCode = http.POST(httpRequestData);

    if (httpCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpCode);
      String payload = http.getString();
      Serial.println(payload);
      
      
    }
    else {
      //          Serial.print("Error code: ");
      //          Serial.println(httpCode);
      Serial.printf("[HTTP] POST ... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    // Free resources
    http.end();

  } else {
    Serial.println("WiFi Disconnected");
  }

}
