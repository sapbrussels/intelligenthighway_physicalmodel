# Generate data

You can generate data by executing the generata_data.py script.

## Install mqtt-cli

    brew tap hivemq/mqtt-cli
    brew cask install adoptopenjdk #optional
    brew install mqtt-cli

### Test

Execute following command. Make sure to select MQTT 3 instead of 5.

    mqtt pub -h 96e9f7bd-c38b-458d-8b98-630be0232fb8.eu10.cp.iot.sap --topic "measures/27f64f84-9657-4ba0-bda0-cee72fca0212" --message lol --cert ./scripts/mqtt.cer --key ./scripts/mqtt.pem -v -p 8883 --mqttVersion 3 -i  27f64f84-9657-4ba0-bda0-cee72fca0212

Would output following:

    Identifier '88a2b0c3-b2b9-435d-b1ad-df59e1691ebb' may be too long (identifier length '36' exceeds 23)
    Identifier '88a2b0c3-b2b9-435d-b1ad-df59e1691ebb' may contain invalid characters ('-')
    Command PublishCommand{host=96e9f7bd-c38b-458d-8b98-630be0232fb8.eu10.cp.iot.sap, port=8883, version=MQTT_3_1_1, identifier=88a2b0c3-b2b9-435d-b1ad-df59e1691ebb, useDefaultSsl=false, sslConfig=com.hivemq.client.internal.mqtt.MqttClientSslConfigImpl@bf531149, useWebSocket=false, topics=[lol], qos=[AT_MOST_ONCE], message=lol}
    Client '88a2b0c3-b2b9-435d-b1ad-df59e1691ebb@96e9f7bd-c38b-458d-8b98-630be0232fb8.eu10.cp.iot.sap' sending CONNECT MqttConnect{keepAlive=60, cleanSession=true}
    Client '88a2b0c3-b2b9-435d-b1ad-df59e1691ebb@96e9f7bd-c38b-458d-8b98-630be0232fb8.eu10.cp.iot.sap' received CONNACK MqttConnAck{returnCode=SUCCESS, sessionPresent=false}
    Client '88a2b0c3-b2b9-435d-b1ad-df59e1691ebb@96e9f7bd-c38b-458d-8b98-630be0232fb8.eu10.cp.iot.sap' sending PUBLISH ('lol') MqttPublish{topic=lol, payload=3byte, qos=AT_MOST_ONCE, retain=false}
    Client '88a2b0c3-b2b9-435d-b1ad-df59e1691ebb@96e9f7bd-c38b-458d-8b98-630be0232fb8.eu10.cp.iot.sap' received PUBLISH acknowledgement MqttPublish{topic=lol, payload=3byte, qos=AT_MOST_ONCE, retain=false}


### Subscribe

Listen to commands.

    mqtt sub -h 96e9f7bd-c38b-458d-8b98-630be0232fb8.eu10.cp.iot.sap --topic "commands/27f64f84-9657-4ba0-bda0-cee72fca0212/#" --cert ./scripts/mqtt.cer --key ./scripts/mqtt.pem -v -p 8883 --mqttVersion 3 -i 27f64f84-9657-4ba0-bda0-cee72fca0212

Listen to ack.

    mqtt sub -h 96e9f7bd-c38b-458d-8b98-630be0232fb8.eu10.cp.iot.sap --topic "ack/27f64f84-9657-4ba0-bda0-cee72fca0212/#" --cert ./scripts/mqtt.cer --key ./scripts/mqtt.pem -v -p 8883 --mqttVersion 3 -i 27f64f84-9657-4ba0-bda0-cee72fca0212

Listen to measures (does not work).

    mqtt sub -h 96e9f7bd-c38b-458d-8b98-630be0232fb8.eu10.cp.iot.sap --topic "measures/27f64f84-9657-4ba0-bda0-cee72fca0212/#" --cert ./scripts/mqtt.cer --key ./scripts/mqtt.pem -v -p 8883 --mqttVersion 3 -i 27f64f84-9657-4ba0-bda0-cee72fca0212

## MQTT - Get Certificate

Download the certificate for your device from the IoT platform. This will give you a .pem certificate (`adaptive_light-device_certificate.pem`) and a secret key (`secret.txt`).

Before we can use it in our script or a MQTT client like (Paho, MQTT Box) you need to decypt it.

    openssl rsa -in adaptive_light-device_certificate.pem -out adaptive_light-device_certificate_decrypted.pem

Once executed it will ask you for the secret, copy paste it, and you will the decrypted certificate `adaptive_light-device_certificate_decrypted.pem`.

Open the new file, and copy the `-----BEGIN RSA PRIVATE KEY------` section at the bottom of the file. Paste it in a new file `mqtt.pem`.

Copy the `-----BEGIN CERTIFICATE-----` section at the bottom of the `adaptive_light-device_certificate.pem` file and copy it to a new file `mqtt.cert`

Now you have all the files to make a connection to the MQTT broker of SAP IoT.

## Run

Install dependencies

    pip install paho-mqtt
