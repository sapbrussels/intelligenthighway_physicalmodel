const mqtt = require('mqtt')
const fs = require("fs");

const HOST_ADDRESS = "96e9f7bd-c38b-458d-8b98-630be0232fb8.eu10.cp.iot.sap";
const DEVICE_ALTERNATE_ID = "41e27b47-c413-463e-af97-f26658561008"; // IntelligentHighway
const SENSOR1_ALTERNATE_ID = "1e2a24d4-8e3a-4ee7-bbdf-d50d56657ac1"; // StreetLight1
const SENSOR2_ALTERNATE_ID = "07a00235-09e5-4d2b-9d91-0ecfde9c62c3"; // StreetLight2
const SENSOR3_ALTERNATE_ID = "7474a34a-a525-43ce-b01c-5f4f22f72107"; // StreetLight3
const CAPABILITY_ALTERNATE_ID = "8c13dfc2-c75b-4b51-aa64-f2e2186ff280"; // AdaptiveLight
const CERTIFICATE_FILE = "./IntelligentHighway-device_certificate.pem";
const PASSPHRASE = "VS7vvt9o8oh1775xEvwUqs4aroIgK4lCsrH7";

var mqttClient = connectToMQTT()

// This will send data to the 3 sensors each second.
setInterval(() => {
    // StreetLight1
    let status = Math.random() >= 0.5;
    sendDataViaMQTT(SENSOR1_ALTERNATE_ID, status)
    // StreetLight2
    sendDataViaMQTT(SENSOR2_ALTERNATE_ID, status)
    // StreetLight3
    sendDataViaMQTT(SENSOR3_ALTERNATE_ID, status)
}, 1000);

function sendDataViaMQTT(sensor, status) {
    var payload = {
        sensorAlternateId: sensor,
        capabilityAlternateId: CAPABILITY_ALTERNATE_ID,
        measures: [
            status
        ]
    }
    var topicName = 'measures/' + DEVICE_ALTERNATE_ID;
    mqttClient.publish(topicName, JSON.stringify(payload), [], error => {
        if(!error) {
            console.log("Data successfully sent!");
        } else {
            console.log("An unecpected error occurred:", error);
        }
    });
}

function connectToMQTT() {
    var options = {
        keepalive: 10,
        clientId: DEVICE_ALTERNATE_ID,
        clean: true,
        reconnectPeriod: 2000,
        connectTimeout: 2000,
        cert: fs.readFileSync(CERTIFICATE_FILE),
        key: fs.readFileSync(CERTIFICATE_FILE),
        passphrase: PASSPHRASE,
        rejectUnauthorized: false
    };

    var mqttClient = mqtt.connect(`mqtts://${HOST_ADDRESS}:8883`, options);

    mqttClient.subscribe('commands/' + DEVICE_ALTERNATE_ID);
    mqttClient.on('connect', () => console.log("Connection established!"));
    mqttClient.on("error", err => console.log("Unexpected error occurred:", err));
    mqttClient.on('reconnect', () => console.log("Reconnected!"));
    mqttClient.on('close', () => console.log("Disconnected!"));
    mqttClient.on('message', (topic, msg) => console.log("Received acknowledgement for message:", msg.toString()));

    return mqttClient
}

function randomInteger(min, max) {
    min = Math.ceil(min);
    max = Math.floor(max);
    return Math.floor(Math.random() * (max - min + 1)) + min;
}
