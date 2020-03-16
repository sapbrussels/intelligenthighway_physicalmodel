# Generate data

You can generate data by executing the generata_data.py script.

## MQTT - Get Certificate

Download the certificate for your device from the IoT platform. This will give you a .pem certificate (`adaptive_streetlight-device_certificate.pem`) and a secret key (`secret.txt`).

Before we can use it in our script or a MQTT client like (Paho, MQTT Box) you need to decypt it.

    openssl rsa -in adaptive_streetlight-device_certificate.pem -out adaptive_streetlight-device_certificate_decrypted.pem

Once executed it will ask you for the secret, copy paste it, and you will the decrypted certificate `adaptive_streetlight-device_certificate_decrypted.pem`.

Open the new file, and copy the `-----BEGIN RSA PRIVATE KEY------` section at the bottom of the file. Paste it in a new file `mqtt.pem`.

Copy the `-----BEGIN CERTIFICATE-----` section at the bottom of the `adaptive_streetlight-device_certificate.pem` file and copy it to a new file `mqtt.cert`

Now you have all the files to make a connection to the MQTT broker of SAP IoT.
