const axios = require('axios')

const username = 'garage'
const password = 'Welcome1'
const token = Buffer.from(`${username}:${password}`, 'utf8').toString('base64')

// This will send data to the 3 sensors each second.
setInterval(() => {
    const indication = getRandomInt(4)
    sendCommand("1e2a24d4-8e3a-4ee7-bbdf-d50d56657ac1", indication)
    sendCommand("07a00235-09e5-4d2b-9d91-0ecfde9c62c3", indication)
    sendCommand("7474a34a-a525-43ce-b01c-5f4f22f72107", indication)
}, 1000);

function sendCommand(sensorId, indication) {
  axios.post('https://96e9f7bd-c38b-458d-8b98-630be0232fb8.eu10.cp.iot.sap/iot/core/api/v1/devices/41e27b47-c413-463e-af97-f26658561008/commands', {
    "capabilityId": "df945cb3-70dd-4462-a4a8-45364a5d5cc6",
    "command": { "IndicativeLight": indication },
    "sensorId": sensorId
  },
  {
    headers: {
      'Authorization': `Basic ${token}`
    }
  })
  .then((res) => {
    console.log(`statusCode: ${res.statusCode}`)
    console.log(res)
  })
  .catch((error) => {
    console.error(error)
  })
}

function getRandomInt(max) {
  return Math.floor(Math.random() * Math.floor(max));
}
