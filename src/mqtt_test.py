import paho.mqtt.client as mqtt

mqttc = mqtt.Client()
mqttc.connect("iot.eclipse.org", 1883, 60)
mqttc.loop_start()

for i in range(10):
    mqttc.publish("RGB", i)
