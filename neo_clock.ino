#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>

#define PIN            0
#define NUMPIXELS      24



// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

unsigned long lastTime;
int seconds; 
int minutes; 
int hours; 

const char* ssid = "FRITZ!Box 7490";
const char* password = "54162624600392969345";

#define WLAN_SSID       "FRITZ!Box 7490"
#define WLAN_PASS       "54162624600392969345"
#define AIO_SERVER      "192.168.178.40"
#define AIO_SERVERPORT  1883  
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, "", "");
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, "/time/hours");


void setup()
{
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.setBrightness(190);
  lastTime = millis();
  seconds = 0; 
  minutes = 30; 
  hours = 3; 
  Serial.begin(115200);
  Serial.println("trying to setup wifi");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&onoffbutton);

  
}

void loop() 
{
    // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &onoffbutton) {
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton.lastread);
    }
  }

//  int currentTime = millis();
//  if (currentTime - lastTime > 1000)
//  {
//    if (seconds == 59)
//    {
//      seconds = 0;
//      if (minutes == 59)
//      {
//        minutes = 0;
//        if (hours == 11)
//        {
//          hours = 0;
//        } else 
//        {
//          hours++;
//        }
//      } else
//      {
//        minutes++;
//      }
//    } else 
//    {
//      seconds++; 
//    }
//    lastTime = currentTime; 
//    
//    Serial.println("test");
//    setLeds();
//  }
}

void setLeds()
{
  int secondLed = seconds*2 / 5;
  int minuteLed = minutes*2 / 5;
  int hourLed = hours*2; 
  for (int i=0; i<NUMPIXELS; i++)
  {
    //set second
    if (i==secondLed)
    {
      pixels.setPixelColor(i, 255, 0, 255);
    } else if(i==minuteLed)
    {
        pixels.setPixelColor(i, 0, 23, 255);     
    } else if (i== hourLed)
    {
      pixels.setPixelColor(i, 255, 23, 0);
    } else
    {
       pixels.setPixelColor(i, 1, 3, 5);
    } 
  }
  pixels.show();
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() 
{
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}

