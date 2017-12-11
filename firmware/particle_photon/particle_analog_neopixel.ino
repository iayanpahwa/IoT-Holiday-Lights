/***
Particle-Photon sketch to control RGB LED strips over the wifi local network using OpenHAb
Author: Ayan Pahwa
Date  : Decebmber 26th 2016
***/


//Importing Necessary Libraries
#include "neopixel/neopixel.h"
#include "MQTT/MQTT.h"

//Defining Pins for analog RGB strip control
#define RED D0
#define GREEN D1
#define BLUE A5

//For neopixel strip
SYSTEM_MODE(AUTOMATIC);
// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN D2
#define PIXEL_COUNT 24
#define PIXEL_TYPE WS2812B

//Instance of neopixel strip
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

//Callback function declaration
void callback(char* topic, byte* payload, unsigned int length);

/**
 * if want to use IP address,
 * byte server[] = { XXX,XXX,XXX,XXX };
 * MQTT client(server, 1883, callback);
 * want to use domain name,
 * MQTT client("www.sample.com", 1883, callback);
MQTT client("server_name", 1883, callback);
**/

byte server[] = { 192,168,1,112 }; //Server where Mosquitto MQTT broker is running
MQTT client(server, 1883, callback); //1883 default port for MQTT



void callback(char* topic, byte* payload, unsigned int length) {

    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;
    String message(p);

    //Parseing String to fetch comma seperated RGB color values independently in each variable:
    int c1 = message.indexOf(',');
    int c2 = message.indexOf(',',c1+1);

    int red = message.toInt();
    int green =  message.substring(c1+1).toInt();
    int blue =  message.substring(c2+1).toInt();

     for(int i=0; i<PIXEL_COUNT;i++)
  {
  strip.setPixelColor(i,red,green,blue); //Set color to neopixel strip
  strip.show();
  }
    //Generating 8-bit PWM corresponding to the RGB values and writing to analog RGB strip
    analogWrite(RED,red);
    analogWrite(GREEN,green);
    analogWrite(BLUE,blue);

}


void setup() {

    //If RGB strip
    pinMode(RED,OUTPUT);
    pinMode(GREEN,OUTPUT);
    pinMode(BLUE,OUTPUT);

    strip.begin(); //Initially all off
    strip.show();
    
    // connect to the particle server
    client.connect("sparkclient");

    // publish/subscribe
    if (client.isConnected()) {

        client.subscribe("RGB"); //Topic <--- Subscribed
    }
}

void loop() {

    if (client.isConnected())
        client.loop();
}
