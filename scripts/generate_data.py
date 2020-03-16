#!/usr/bin/python
# -*- coding: utf-8 -*-
import time
import requests
import paho.mqtt.client as paho
import json
import random

def create_client(device_alternate_id, name):
    client = paho.Client(client_id=device_alternate_id)
    client.tls_set(certfile="./mqtt.cer", keyfile="./mqtt.pem")
    client.on_message = on_message
    client.on_connect = on_connect(device_alternate_id)
    print('Connecting')
    client.connect("96e9f7bd-c38b-458d-8b98-630be0232fb8.eu10.cp.iot.sap", 8883, 60)
    return client


def on_message(client, userdata, msg):
    global machineMode
    print(msg.topic + " " + str(msg.payload))
    try:
        machineMode = json.loads(msg.payload)["command"]['Intensity']
    except Exception:
        print("Couldn't find Intensity")


def on_connect(device_id):
    def on_connect_device(client, userdata, flags, rc):
        print("Connected to " + device_id + " with result code " + str(rc))
        # Subscribing in on_connect() means that if we lose the connection and
        # reconnect then subscriptions will be renewed.
        client.subscribe("commands/" + device_id + "/#")
        # client.subscribe("ack/"+device_id+"/#") # Debug the messages sent

    return on_connect_device


adaptive_streetlight = create_client('27f64f84-9657-4ba0-bda0-cee72fca0212', 'adaptive_streetlight')


def send_data():

    measuresBHC ={
        'IR': "true",
    }

    data = json.dumps({
        "capabilityAlternateId": "125923c8-f797-4997-98cf-4434fc9d7c79",
        "measures": measuresBHC,
        "sensorAlternateId": "26ddca77-0936-4c73-bc63-125d19d3223d"
    })

    print('Sending data to adaptive_streetlight')
    adaptive_streetlight.publish("measures/27f64f84-9657-4ba0-bda0-cee72fca0212", data)


while True:
    send_data()
    time.sleep(1)

print('Done')
