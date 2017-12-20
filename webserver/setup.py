#!/usr/bin/python

import paho.mqtt.client as mqtt
from flask import Flask, render_template, request

mqttc = mqtt.Client()
#Local host running MQTT-Mosquitto Broker
mqttc.connect("127.0.0.1", 1883, 60)
mqttc.loop_start()

app = Flask(__name__)

# Load the main form template on webrequest for the root page
@app.route("/")
def main():

    # Create a template data dictionary to send any data to the template
    templateData = {
        'title' : 'IoT Holiday Lights'
        }
    # Pass the template data into the template index.html and return it to the user
    return render_template('index.html', **templateData)

#REST CALL HANDLING
@app.route("/rgb/<value>")
def sendRGB(value):
    #Converting hex value to decimal
    red = int(value[0:2] , 16)
    green = int(value[2:4] , 16)
    blue = int(value[4:6] , 16)

    #Converting them to comma seperated string for MQTT broker
    red = str(red)
    green = str(green)
    blue = str(blue)

    #Concating in comma seperated form
    value = red + ',' + green + ',' + blue

    #Publishing string value under RGB Topic
    mqttc.publish("cafeteria/trees", value)
    return value


if __name__ == "__main__":
    app.run(host='0.0.0.0', port=80, debug=True)
