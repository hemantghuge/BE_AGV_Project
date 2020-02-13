/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-input-data-html-form/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

#ifdef ESP32
  #include <WiFi.h>
  #include <ESPAsyncWebServer.h>
  #include <SPIFFS.h>
#else
  #include <Arduino.h>
  #include <ESP8266WiFi.h>
  #include <Hash.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #include "FS.h"
#endif

AsyncWebServer server(80);

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "test_network";
const char* password = "12345678";

const char* PARAM_INPUT_1 = "input1";
const char* PARAM_INPUT_2 = "input2";

// HTML web page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
  body {
    background-color: lightblue;
  }
  p{
  color: Black;
  text-align: center;
  }
  p{
    font-family: Times new roman;
    font-size: 20px;
  } 
  </style>
  </head><body>

  <center><h1>Automated Guided Vehicle</h1><center>

  <form action="/get">
    input1: <input type="text" name="input1">
    input2: <input type="text" name="input2">
    <input type="submit" value="Submit">
  </form>

  <img src="agv">
  <img src="sun">
  
</body></html>)rawliteral";

// HTML web page
const char index_html_final[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
  body {
    background-color: lightblue;
  }
  p{
  color: Black;
  text-align: center;
  }
  p{
    font-family: Times new roman;
    font-size: 20px;
  } 
  </style>
  </head><body>

  <center><h1>Automated Guided Vehicle</h1><center>

  <form action="/get">
    input1: <input type="text" name="input1">
    <input type="text" value=inputMessage1>
    input2: <input type="text" name="input2">
    <input type="text" value=inputMessage2>
  </form>
  
</body></html>)rawliteral";


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage1;
    String inputParam1;
    String inputMessage2;
    String inputParam2;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputParam1 = PARAM_INPUT_1;
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      inputParam2 = PARAM_INPUT_2;
    }
    /*
      // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
      else if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
      }
    */
    else {
      inputMessage1 = "No message sent";
      inputParam1 = "none";
      inputMessage2 = "No message sent";
      inputParam2 = "none";
    }
    Serial.print("Start Point");
    Serial.println(inputMessage1);
    Serial.print("End Point");
    Serial.println(inputMessage2);
    request->send(200, "text/html", index_html);
    /*"Start Point ("
                  + inputParam1 + ") with value: " + inputMessage1 +
                  "End Point ("
                  + inputParam2 + ") with value: " + inputMessage2 +
                  "<br><a href=\"/\">Return to Home Page</a>"
    */
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {

}
