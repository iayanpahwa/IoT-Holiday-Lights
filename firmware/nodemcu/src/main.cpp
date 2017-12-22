#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>

#define SERIAL_DEBUG   1
#define NEOPIXEL       1
#define ANALOG_RGB     0 

#define NEOPIXEL_PIN   D8 // Pin on which Neopicel LED strip is connected
#define NUMPIXELS      60 // Number of Neopixels LEDs in strip

#define RED            D2
#define GREEN          D5 // Pin mapping for Analog RGB LED strip
#define BLUE           D6 

// Variable to hold network parameters
const char* ssid = "Xiaomi_3CF1";
const char* password = "sska1234";
const char* mqtt_server = "192.168.31.36"; //Address or IP
const char* TOPIC = "cafeteria/trees";

#if NEOPIXEL
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
#endif

// Class Instances
WiFiClient espClient;
PubSubClient client(espClient);

// Declare setup WiFi and MQTT Callback functions
void setup_wifi();
void callback(char* , byte* , unsigned int); // MQTT callback function

// Program starts here
void setup() {

  #if SERIAL_DEBUG
  Serial.begin(115200); // For Serial debugging
  #endif

  #if ANALOG_RGB
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT); // Declare pins as output for PWM
  pinMode(BLUE, OUTPUT);
  #endif

  setup_wifi(); // Start by connecting to WiFi
  client.setServer(mqtt_server, 1883); // Set MQTT broker IP/Hostname and Port
  client.setCallback(callback);
  strip.begin(); // Initialize Neopixel strip

}

void setup_wifi() {

  delay(10); // Small delay before starting WiFi activity

  #if SERIAL_DEBUG
  Serial.println("Connecting to ");
  Serial.println(ssid);
  #endif

  WiFi.begin(ssid, password); // This will conenct ESP to WiFi with given SSID and PASSWORD

  while (WiFi.status() != WL_CONNECTED) {

    delay(500); // Retry connection after 500 ms

    #if SERIAL_DEBUG
    Serial.print(".");
    #endif

}

  #if SERIAL_DEBUG
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // Prints IP Address on serial console
  #endif

}

// MQTT callback function, executed whenever a payload is received on topic node is subscribed to
void callback(char* topic, byte* payload, unsigned int length) {

  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String message(p);

  //Parsing String to fetch RGB values independently in each variable:
  int c1 = message.indexOf(',');
  int c2 = message.indexOf(',',c1+1); // RGB values are sent in comma seperated order RR,GG,BB

  int red = message.toInt();
  int green = message.substring(c1+1).toInt(); // Value sent is 8-bit but ESP can do 10 bits PWM hence x By 4
  int blue =  message.substring(c2+1).toInt();

  //Generating PWM corresponding to the RGB values
  #if SERIAL_DEBUG
  Serial.println(red);
  Serial.println(green);
  Serial.println(blue);
  #endif

   #if NEOPIXEL
   for(int i=0;i<=NUMPIXELS;i++){

      strip.setPixelColor(i, red, green, blue); // Write data to Neopixel stri[]
      strip.show();
      delay(1);

   }
   #endif

  #if ANALOG_RGB 
  analogWrite(RED,red);
  analogWrite(GREEN,green); // Write data on analog RGB LED strip
  analogWrite(BLUE,blue);
  #endif

  delay(1); // Short delay for smoother animation

 }

// If MQTT connection breaks, reconnect to the broker
void reconnect() {

  // Loop until we're reconnected
  while (!client.connected()) {
    
    #if SERIAL_DEBUG
    Serial.print("Attempting MQTT connection..."); 
    #endif
    
    if (client.connect("ESP8266Client")) {

      #if SERIAL_DEBUG
      Serial.println("connected"); // Attempt to connect
      #endif
      client.subscribe(TOPIC); // Subscribe to topic defined above

    }

    else {

      #if SERIAL_DEBUG
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      #endif
      // Wait 5 seconds before retrying
      delay(5000);

    }
  }
}

void loop() {

  if (!client.connected()) {

    reconnect();

  }

  client.loop();

}
