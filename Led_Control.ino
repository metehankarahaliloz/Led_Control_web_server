
#include <WiFi.h>


const char* ssid = "FiberHGW_TP56D0_2.4GHz";
const char* password = "CYngjPXk";


WiFiServer server(80);


String header;


String pin32State = "off";
String pin33State = "off";
String pin26State="off";

const int output32 = 32;
const int output33 = 33;
const int output26 = 26;

unsigned long currentTime = millis();

unsigned long previousTime = 0; 

const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
 
  pinMode(output32, OUTPUT);
  pinMode(output33, OUTPUT);
  pinMode(output26, OUTPUT);
  digitalWrite(output32, LOW);
  digitalWrite(output33, LOW);
  digitalWrite(output26, LOW);
  
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   

  if (client) {                            
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");        
    String currentLine = "";               
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  
      currentTime = millis();
      if (client.available()) {             
        char c = client.read();            
        Serial.write(c);                   
        header += c;
        if (c == '\n') {                   
         
          if (currentLine.length() == 0) {
           
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
          
            if (header.indexOf("GET /32/on") >= 0) {
              Serial.println("GPIO 32 on");
              pin32State = "on";
              digitalWrite(output32, HIGH);
            } else if (header.indexOf("GET /32/off") >= 0) {
              Serial.println("GPIO 32 off");
              pin32State = "off";
              digitalWrite(output32, LOW);
            } else if (header.indexOf("GET /33/on") >= 0) {
              Serial.println("GPIO 33 on");
              pin33State = "on";
              digitalWrite(output33, HIGH);
            } else if (header.indexOf("GET /33/off") >= 0) {
              Serial.println("GPIO 33 off");
              pin33State = "off";
              digitalWrite(output33, LOW);
            }else if (header.indexOf("GET /26/on") >= 0) {
              Serial.println("GPIO 26 on");
              pin26State = "on";
              digitalWrite(output26, HIGH);
            } else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              pin26State = "off";
              digitalWrite(output26, LOW);
            }
            
           
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
           
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
          
            client.println("<body><h1 style=\"color:blue;\">Metehan Karahalilöz</h1>");
            
           
            client.println("<p >GPIO 32 - State " + pin32State + "</p>");
            
            if (pin32State=="off") {
              client.println("<p ><a href=\"/32/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/32/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            
            client.println("<p >GPIO 33 - State " + pin33State + "</p>");
                   
            if (pin33State=="off") {
              client.println("<p><a href=\"/33/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/33/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("<p >GPIO 26 - State " + pin26State + "</p>");
            
            if (pin26State=="off") {
              client.println("<p ><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
            client.println("</body></html>");
            
            
            client.println();
            
            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      
        }
      }
    }
    
    header = "";
    
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}