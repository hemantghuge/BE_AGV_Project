#include <ESP8266WiFi.h>
 
const char* ssid = "bumblebee";
const char* password = "revati@123";
 
int ledPin = 13; // GPIO13
WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
 
  int value = LOW;
  if (request.indexOf("/result1") != -1)  {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/result2") != -1)  {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }
 
// Set ledPin according to the request
//digitalWrite(ledPin, value);
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("Led pin is now: ");
 
  if(value == HIGH) {
    client.print("start");
  } else {
    client.print("end");
  }
  client.println("<br><br>");
  client.println("<h1>Automated Guided Vehicle</h1>");
  client.println("<button onclick="myFunction1()">Start</button>"); 
  
  client.println("<p id="id1"></p>");  
  
  client.println("<script>");  
  
  client.println("function myFunction1(){
  document.getElementById("id1").innerHTML = "Enter Starting Station";}  
  
  client.println("</script>");
    
  client.println("<input type="number" id="myNumber" value="0">");  
  
  client.println("<button onclick="myFunction2()">Result1</button>");  
  
  client.println("<p id="demo"></p>");  
  
  client.println("<script>"); 
  
  client.println("function myFunction2() {
  confirm("Start journey from entered station");}
  
  client.println("<button onclick="myFunction3()">End</button>"); 
   
  client.println("<p id="id2"></p>");  

   client.println("</script>");  

   client.println("function myFunction3() {
  document.getElementById("id2").innerHTML = "Enter Destination Station";}  
   
  client.println("</script>"); 
  
  client.println("<input type="number" id="myNumber" value="0">");  
  
  client.println("<button onclick="myFunction4()">Result2</button>"); 
   
  client.println("<p id="demo"></p>"); 
   
  client.println("</script>");
   
  client.println("function myFunction4() {
  confirm("End journey to entered station");}
  
  client.println("</script>");  
     
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
 
