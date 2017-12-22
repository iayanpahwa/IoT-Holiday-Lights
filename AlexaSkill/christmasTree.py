import logging
import os
import paho.mqtt.client as mqtt

from flask import Flask
from flask_ask import Ask, request, session, question, statement
from time import sleep

app = Flask(__name__)
ask = Ask(app, "/")
logging.getLogger('flask_ask').setLevel(logging.DEBUG)

mqttc = mqtt.Client()
mqttc.connect("192.168.31.36", 1883, 60)
mqttc.loop_start()


@ask.launch
def launch():
    speech_text = 'Welcome to the IoT christmas holiday lights by S.D.I.O.T.!. You can ask me to control your smart christmas tree'
    return question(speech_text).reprompt(speech_text).simple_card('christmasTree', speech_text)
    #return statement(speech_text).simple_card('christmasTree', speech_text)

@ask.intent('OnIntent')
def on():
    value = '50' + ',' + '50' + ',' + '50'
    mqttc.publish("cafeteria/trees", value)
    sleep(1)
    value = '100' + ',' + '100' + ',' + '100'
    mqttc.publish("cafeteria/trees", value)
    sleep(1)
    value = '255' + ',' + '255' + ',' + '255'
    mqttc.publish("cafeteria/trees", value)
    speech_text = 'Christmas tree lights turned on!'
    return question(speech_text).reprompt(speech_text).simple_card('christmasTree', speech_text)

    #return statement(speech_text).simple_card('christmasTree', speech_text)

@ask.intent('OffIntent')
def off():
    value = '0' + ',' + '0' + ',' + '0'
    mqttc.publish("cafeteria/trees", value)
    speech_text = 'Christmas tree lights turned off!'
    return statement(speech_text).simple_card('christmasTree', speech_text)

@ask.intent('BlueIntent')
def off():
    value = '0' + ',' + '0' + ',' + '255'
    mqttc.publish("cafeteria/trees", value)
    speech_text = 'Christmas tree lights turned Blue!'
    return question(speech_text).reprompt(speech_text).simple_card('christmasTree', speech_text)

@ask.intent('PurpleIntent')
def off():
    value = '128' + ',' + '0' + ',' + '128'
    mqttc.publish("cafeteria/trees", value)
    speech_text = 'Christmas tree lights turned Purple!'
    return question(speech_text).reprompt(speech_text).simple_card('christmasTree', speech_text)

@ask.intent('GreenIntent')
def off():
    value = '0' + ',' + '255' + ',' + '0'
    mqttc.publish("cafeteria/trees", value)
    speech_text = 'Christmas tree lights turned Green!'
    return question(speech_text).reprompt(speech_text).simple_card('christmasTree', speech_text)

@ask.intent('RedIntent')
def off():
    value = '255' + ',' + '0' + ',' + '0'
    mqttc.publish("cafeteria/trees", value)
    speech_text = 'Christmas tree lights turned Red!'
    return question(speech_text).reprompt(speech_text).simple_card('christmasTree', speech_text)

@ask.intent('OrangeIntent')
def off():
    value = '255' + ',' + '165' + ',' + '0'
    mqttc.publish("cafeteria/trees", value)
    speech_text = 'Christmas tree lights turned Orange!'
    return question(speech_text).reprompt(speech_text).simple_card('christmasTree', speech_text)

@ask.intent('YellowIntent')
def off():
    value = '255' + ',' + '255' + ',' + '0'
    mqttc.publish("cafeteria/trees", value)
    speech_text = 'Christmas tree lights turned Yellow!'
    return question(speech_text).reprompt(speech_text).simple_card('christmasTree', speech_text)

@ask.intent('AMAZON.HelpIntent')
def help():
    speech_text = 'You can start SDIoT christmas tree lights!'
    return question(speech_text).reprompt(speech_text).simple_card('christmasTree', speech_text)


@ask.session_ended
def session_ended():
    return "{}", 200


if __name__ == '__main__':
    if 'ASK_VERIFY_REQUESTS' in os.environ:
        verify = str(os.environ.get('ASK_VERIFY_REQUESTS', '')).lower()
        if verify == 'false':
            app.config['ASK_VERIFY_REQUESTS'] = False
app.run(debug=True)