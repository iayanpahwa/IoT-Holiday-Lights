#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Pin Mapping definition of analog RGB LED strip connected to NodeMCU board
#define RED D8
#define GREEN D5
#define BLUE D6

// Variable to hold network parameters
const char* ssid = "........";
const char* password = "........";
const char* mqtt_server = "xxx.xxx.xxx.xxx"; //Address or IP
const char* TOPIC = "cafeteria/trees";

// Class Instances
WiFiClient espClient;
PubSubClient client(espClient);

// Declare setup WiFi and MQTT Callback functions
void setup_wifi();
void callback(char* , byte* , unsigned int); // MQTT callback function

// Program starts here
void setup() {

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT); // Declare pins as output for PWM
  pinMode(BLUE, OUTPUT);
  // For Serial debugging
  Serial.begin(115200);
  setup_wifi(); // Start by connecting to WiFi
  client.setServer(mqtt_server, 1883); // Set MQTT broker IP/Hostname and Port
  client.setCallback(callback);

}

void setup_wifi() {

  delay(10); // Small delay before starting WiFi activity
  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password); // This will conenct ESP to WiFi with given SSID and PASSWORD

  while (WiFi.status() != WL_CONNECTED) {

    delay(500); // Retry connection after 500 ms
    Serial.print(".");

}

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // Prints IP Address on serial console

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

  int red = 4*(message.toInt());
  int green = 4*((message.substring(c1+1).toInt())); // Value sent is 8-bit but ESP can do 10 bits PWM hence x By 4
  int blue =  4*((message.substring(c2+1).toInt()));

  //Generating PWM corresponding to the RGB values
   Serial.println(red);
   Serial.println(green);
   Serial.println(blue);

   analogWrite(RED,red);
   analogWrite(GREEN,green);
   analogWrite(BLUE,blue);

  //In case of NeoPixel Strip
  //for(int i=0;i<=PIXEL_COUNT;i++)
  //strip.setPixelColor(i, red, green, blue);

  delay(1); // Short delay for smoother animation

 }

// If MQTT connection breaks, reconnect to the broker
void reconnect() {

  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {

      Serial.println("connected");
      client.subscribe(TOPIC); // Subscribe to topic defined above

    }

    else {

      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
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
