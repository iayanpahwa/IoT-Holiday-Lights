// This #include statement was automatically added by the Particle IDE.
#include "neopixel/neopixel.h"

// This #include statement was automatically added by the Particle IDE.
#include "MQTT/MQTT.h"

//Using Neopixel strip define following
//#define PIXEL_PIN    2
//#define PIXEL_COUNT 30    // 30 pixels in the NeoPixel strip
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

//Callback function declaration

SYSTEM_MODE(AUTOMATIC);

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D2
#define PIXEL_COUNT 24
#define PIXEL_TYPE WS2812B

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

void callback(char* topic, byte* payload, unsigned int length);

/**
 * if want to use IP address,
 * byte server[] = { XXX,XXX,XXX,XXX };
 * MQTT client(server, 1883, callback);
 * want to use domain name,
 * MQTT client("www.sample.com", 1883, callback);
MQTT client("server_name", 1883, callback);
**/

byte server[] = { 192,168,1,119 }; //Server where Mosquitto MQTT broker is running
MQTT client(server, 1883, callback); //1883 default port for MQTT



void callback(char* topic, byte* payload, unsigned int length) {

    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;
    String message(p);

    //Parseing String to fetch RGB values independently in each variable:
    int c1 = message.indexOf(',');
    int c2 = message.indexOf(',',c1+1);
    int red = message.toInt();
    int green =  message.substring(c1+1).toInt();
    int blue =  message.substring(c2+1).toInt();

//Setting value to neopixel strip
    for(int i=0; i<PIXEL_COUNT;i++)
  {
  strip.setPixelColor(i,red,green,blue);
  strip.show();
}

}


void setup() {

 strip.begin(); //Initially all off
  strip.show();
    // connect to the particle server
    client.connect("sparkclient");

    // publish/subscribe
    if (client.isConnected()) {
        //Successfully Publish Message
        client.subscribe("RGB"); //Topic <--- Subscribed
    }
}

void loop() {

    if (client.isConnected())
        client.loop();
}
