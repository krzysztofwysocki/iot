/*
 ESP8266 --> ThingSpeak Channel
 
 This sketch sends the value of Analog Input (A0) to a ThingSpeak channel
 using the ThingSpeak API (https://www.mathworks.com/help/thingspeak).
 
 Requirements:
 
   * ESP8266 Wi-Fi Device
   * Arduino 1.8.8+ IDE
   * Additional Boards URL: http://arduino.esp8266.com/stable/package_esp8266com_index.json
   * Library: esp8266 by ESP8266 Community
   * Library: ThingSpeak by MathWorks
 
 ThingSpeak Setup:
 
   * Sign Up for New User Account - https://thingspeak.com/users/sign_up
   * Create a new Channel by selecting Channels, My Channels, and then New Channel
   * Enable one field
   * Enter SECRET_CH_ID in "secrets.h"
   * Enter SECRET_WRITE_APIKEY in "secrets.h"

 Setup Wi-Fi:
  * Enter SECRET_SSID in "secrets.h"
  * Enter SECRET_PASS in "secrets.h"
  
 Tutorial: http://nothans.com/measure-wi-fi-signal-levels-with-the-esp8266-and-thingspeak
   
 Created: Feb 3, 2017 by Hans Scharler (http://nothans.com)
*/

#include "ThingSpeak.h"
#include "secrets.h"

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

#include <ESP8266WiFi.h>

char ssid[] = SECRET_SSID;   // your network SSID (name)
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key index number (needed only for WEP)
WiFiClient  client;

#include "DHT.h"

#define DHTPIN D4
#define DHTTYPE DHT11 
 
DHT dht(DHTPIN, DHTTYPE);


void setup() {
  Serial.begin(115200);
  delay(100);

  WiFi.mode(WIFI_STA);

  ThingSpeak.begin(client);
}

void loop() {

  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  // Measure Analog Input (A0)
  // Option 1 A0 connected to LM35
  // int rawvoltage= analogRead(A0);
  // float millivolts= (rawvoltage/1024.0) * 3300;
  // float fsoiltemp= millivolts/10;

  // Measure Analog Input (A0)
  // Option 2 A0 connected to Soil Humidity Sensor (keyebase)
   int rawvoltage= analogRead(A0);
   float fsoilmoist= rawvoltage/10.0;


 // Measure Air Temp Input (D4)
float fhumidity = dht.readHumidity();
float fcelsius = dht.readTemperature();

Serial.print("Temperature: ");
Serial.print(fcelsius);
Serial.print("Humidity: ");
Serial.print(fhumidity);
Serial.print("Soil Moisture: ");
Serial.print(fsoilmoist);
Serial.println("");
Serial.println("Sending data to Thingspeak");

  // Write value to Field 1 of a ThingSpeak Channel
  //int httpCode = ThingSpeak.writeField(myChannelNumber, 1, fcelsius, myWriteAPIKey);
  // Write value to Field 2 of a ThingSpeak Channel
  //httpCode = ThingSpeak.writeField(myChannelNumber, 2, fhumidity, myWriteAPIKey);

ThingSpeak.setField(1, fcelsius);
ThingSpeak.setField(2, fhumidity);
ThingSpeak.setField(3, fsoilmoist);
int httpCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);


  if (httpCode == 200) {
    Serial.println("Channel write successful.");
  }
  else {
    Serial.println("Problem writing to channel. HTTP error code " + String(httpCode));
  }

  // Wait 20 seconds to uodate the channel again
  delay(20000);
}
