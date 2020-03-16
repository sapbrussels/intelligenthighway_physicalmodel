# Data model

The physical model has two functions:
- turning on/off the **adaptive lights**,
- and change the status of the **indicative lights**.

For making above two functions available, a data model was created in the SAP IoT platform (https://96e9f7bd-c38b-458d-8b98-630be0232fb8.eu10.cp.iot.sap/iot/cockpit/#/tenant/4). The model looks like this:

- Device: IntelligentHighway (41e27b47-c413-463e-af97-f26658561008)
  - Sensor Type: StreetLight (8c13dfc2-c75b-4b51-aa64-f2e2186ff280)
    - Capability: AdaptiveLight - measure (8c13dfc2-c75b-4b51-aa64-f2e2186ff280)
    - Capability: IndicativeLight - command (df945cb3-70dd-4462-a4a8-45364a5d5cc6)
    - Capability: LightLifeTime - measure (36b597a9-5c3e-45e5-985e-9453ea9c2ba0)

The IntelligentHighway has three (3) street light sensors:

- StreetLight1 (1e2a24d4-8e3a-4ee7-bbdf-d50d56657ac1)
- StreetLight2 (07a00235-09e5-4d2b-9d91-0ecfde9c62c3)
- StreetLight3 (7474a34a-a525-43ce-b01c-5f4f22f72107)

A street light can do two things:
- it turns on the light automatically when an object approaches (adaptive light).
- it turns on a secondary light when a specific event is occurring (indicative light).

# Generate data

You can generate data by executing the generata_data.js script. This will generate data for the AdaptiveLight, for each of the streetlights.

## Run the script

First install the dependencies (make sure you have nodejs and npm installed - https://nodejs.org/en/).

    npm i

Run the script

    node generate_data.js

After you execute you will see data is being send to the IoT platform. Check the sensor data graph on the dashboard.

# Send commands to IndicativeLight

You can send a MQTT message (command) to your sensor by using the  `trigger_commmand.js` script. This will send random statuses.

    node trigger_commmand.js

## (optional) Install mqtt-cli

    brew tap hivemq/mqtt-cli
    brew cask install adoptopenjdk #optional
    brew install mqtt-cli

### (optional) Test

Execute following command. Make sure to select MQTT 3 instead of 5.

    mqtt pub -h 96e9f7bd-c38b-458d-8b98-630be0232fb8.eu10.cp.iot.sap --topic "measures/..." --message ... --cert ./scripts/mqtt.cer --key ./scripts/mqtt.pem -v -p 8883 --mqttVersion 3 -i ..

### Subscribe

Listen to commands.

    mqtt sub -h 96e9f7bd-c38b-458d-8b98-630be0232fb8.eu10.cp.iot.sap --topic "commands/.../#" --cert ./scripts/mqtt.cer --key ./scripts/mqtt.pem -v -p 8883 --mqttVersion 3 -i ...

Listen to ack.

    mqtt sub -h 96e9f7bd-c38b-458d-8b98-630be0232fb8.eu10.cp.iot.sap --topic "ack/.../#" --cert ./scripts/mqtt.cer --key ./scripts/mqtt.pem -v -p 8883 --mqttVersion 3 -i ...


## (Optional) MQTT - Get Decrypted Certificate

Download the certificate for your device from the IoT platform. This will give you a .pem certificate (`adaptive_light-device_certificate.pem`) and a secret key (`secret.txt`).

Before we can use it in our script or a MQTT client like (Paho, MQTT Box) you need to decypt it.

    openssl rsa -in adaptive_light-device_certificate.pem -out adaptive_light-device_certificate_decrypted.pem

Once executed it will ask you for the secret, copy paste it, and you will the decrypted certificate `adaptive_light-device_certificate_decrypted.pem`.

Open the new file, and copy the `-----BEGIN RSA PRIVATE KEY------` section at the bottom of the file. Paste it in a new file `mqtt.pem`.

Copy the `-----BEGIN CERTIFICATE-----` section at the bottom of the `adaptive_light-device_certificate.pem` file and copy it to a new file `mqtt.cert`

Now you have all the files to make a connection to the MQTT broker of SAP IoT.

## (Useful( Trigger commands

Interesting read: https://help.sap.com/doc/459df7cc2db04351b9b27808875d6a6d/Cloud/en-US/iot.cf.send.commands.mqtt.pdf
