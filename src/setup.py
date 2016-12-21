#!/usr/bin/python

import paho.mqtt.client as mqtt
from flask import Flask, render_template, request

mqttc = mqtt.Client()
mqttc.connect("iot.eclipse.org", 1883, 60)
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
    mqttc.publish("RGB", value)
    return value


if __name__ == "__main__":
    app.run(host='0.0.0.0', port=80, debug=True)
