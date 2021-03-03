//pir sensor project 
//Project By Prathyusha Nareshkumar 

#include <Adafruit_MQTT.h>                                  // Adafruit MQTT library
#include <Adafruit_MQTT_Client.h>                           // Adafruit MQTT library
//*****************************************************************************

#include <ESP8266WiFi.h>                                  // ESP8266 library     
#include <SoftwareSerial.h>                                 // Software Serial Library so we can use Pins for communication with the GPS module

//*********************** PIR Motion Sensor Setup *****************************                                                        
 
int buzzerpin = 2;                                            // choose the pin for the BUZZER (Pin D4, GPIO 2) 
int ledpin = 4;                                             // choose the pin for the LED (Pin D2, GPIO 4)
int inputpin = 13;                                          // choose the input pin (for PIR sensor) (Pin D7, GPIO 13)
int pirState = LOW;                                         // we start, assuming no motion detected
float readval = 0;
float writevalue=0;      

//************************* WiFi Access Point *********************************

#define WLAN_SSID       "Naresh"                        // Enter Your router SSID
#define WLAN_PASS       "snk@1964"                        // Enter Your router Password
WiFiClient client;
//************************* Adafruit.io Setup *********************************

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                                // use 8883 for SSL
#define AIO_USERNAME    "prathyu13"                      // Enter Your Adafruit IO Username
#define AIO_KEY         "aio_fiMZ09RH3pkJ7WplapdhjQ0qLgcV"                      // Enter Your Adafruit IO Key


Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish motion = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/pir-sensor-feed");
// connect to adafruit io via MQTT
void connect() {
  Serial.print("Connecting to Adafruit IO... ");
  int ret;
  while ( (ret= mqtt.connect()) != 0) {
         Serial.println(F("Connection failed")); break;
  if(ret >= 0)
      mqtt.disconnect();
      Serial.println(F("Retrying connection..."));
    delay(10000);
  }
  Serial.println("Adafruit IO Connected!");
}
void setup()
{
  Serial.begin(9600);
  pinMode(buzzerpin,OUTPUT);
  pinMode(ledpin,OUTPUT);
  pinMode(inputpin,INPUT);
  delay(100);
  Serial.println("Detecting the sensitivity of pir sensor using ,Adafruit IO,IFTTT,ESP8266");
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // connect to adafruit io
  connect();
}




void loop()
{ 
  readval = digitalRead(inputpin);
   delay(500);
  Serial.print("The value read from the pir sensor is :");
  Serial.println(readval);
  digitalWrite(buzzerpin,LOW);
  digitalWrite(ledpin,LOW);
  
  if( readval == HIGH){  
  digitalWrite(buzzerpin,HIGH);
  digitalWrite(ledpin,HIGH);  
  Serial.println("Motion Detected");
  if (! motion.publish(readval)) {                     //Publish to Adafruit
      Serial.println("Failed");
      Serial.println("Not updated");
    } 
    else {
      Serial.println("Sent!");
      
    }
    delay(5000);                    
       
  }
  else if(readval == LOW)
  { digitalWrite(ledpin,LOW);
    digitalWrite(ledpin,LOW);
    
  Serial.println("Motion Not Detected");
  if (! motion.publish(readval)) {                     //Publish to Adafruit
      Serial.println("Failed");
      Serial.println("Not updated");
    } 
    else {
      Serial.println("Sent!");
      
    }
    delay (4000);
  }
  
  
}
