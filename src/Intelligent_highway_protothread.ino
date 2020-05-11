/*
 * Made by Nicolas Tate-Smith in SAP Belux innovation lab 
 * date : 11 May 2020
 * 
 * please take a look at the README before jumping into the code.
 * 
 * Description :  
 * 
 * This  code is built to control the intelligent highways physical model. The physical model display a car driving down the road, when the car triggers 
 * the sensors the streetlights are activated. we use protothreading in this code to allow multitasking ( for more details on how this library works and 
 * how to use it check the documention in the github repo ). 
 * 
 * The status of the lights are sent to an MQTT broker. We also control an LED with an MQTT message. 
 * 
 * 
 * PIN  layout 
 * --------------------------------------------------------------------------------------------------
 * | LED_1_ADAPTIVELIGHT - Arduino | LED_2_ADAPTIVELIGHT - Arduino | LED_3_ADAPTIVELIGHT - Arduino | 
 * |-------------------------------------------------------------------------------------------------
 * | anode               - A5      | anode               - 5       | anode               - A4      |
 * | cathode             - GND     | cathode             - GND     | cathode             - GND     |                                
 * -------------------------------------------------------------------------------------------------------
 * | LED_1_INDICATIVELIGHT - Arduino | LED_2_INDICATIVELIGHT - Arduino | LED_3_INDICATIVELIGHT - Arduino | 
 * |------------------------------------------------------------------------------------------------------
*  | anode                 - A3      | anode                 - A2      | anode                 - A1      |
 * | cathode               - GND     | cathode               - GND     | cathode               - GND     |
 * -------------------------------------------------------------------------------------------------------
 * | Sensor_PIN1 - Arduino
 * |------------------------------------------------------------------------------------------------------
 * | VCC         - 5V
 * | GND         - GND
 * | OUTPUT      - A0
 * -------------------------------------------------------------------------------------------------------
 * | Steppermotor1 - Arduino | Steppermotor1 - Arduino |
 * |------------------------------------------------------------------------------------------------------
 * | +             - Vin     | +             - Vin     |
 * | -             - GND     | -             - GND     |
 * | IN1           - 10      | IN1           - 6       |
 * | IN2           - 11      | IN2           - 7       |
 * | IN3           - 12      | IN3           - 8       |
 * | IN4           - 13      | IN4           - 9       |  
*/



//libraries used
#include <pt.h> // library that allows protothreading
#include <Stepper.h> // library that handles the stepper motors
#include <WiFiNINA.h> // library that handles the wifi connectivity 
#include <PubSubClient.h> // library that allows mqtt communication

// Set the pins connected to the arduino
#define LED_1_ADAPTIVELIGHT A5
#define LED_2_ADAPTIVELIGHT 5
#define LED_3_ADAPTIVELIGHT A4
#define SENSOR_PIN1 A0
#define LED_3_INDICATORLIGHT A1
#define LED_2_INDICATORLIGHT A2
#define LED_1_INDICATORLIGHT A3

const int stepsPerRevolution = 2048; // the motor does 2048 steps when turning 360°
Stepper myStepper1 = Stepper(stepsPerRevolution, 10, 12, 11, 13); // output pins
Stepper myStepper2 = Stepper(stepsPerRevolution, 6, 8, 7, 9);

const char* ssid = "Attick Wifi"; // Enter your WiFi name
const char* password =  "IORYEAW5AF"; // Enter WiFi password
const char* mqttServer = "35.195.197.206"; // enter the mqtt broker
const int mqttPort =1883; 
WiFiClient espClient;
PubSubClient client(espClient);


// Start our protothreads

static struct pt pt1, pt2, pt3; // we have to declare tha mount before starting



static int protothreadSENSOR(struct pt *pt) // This is our first protothread 
{
  // Start a protothread
  static unsigned long lastTimeBlink =0;
  PT_BEGIN(pt);
  while (1) {
    lastTimeBlink = millis();
    PT_WAIT_UNTIL(pt, digitalRead(SENSOR_PIN1) == LOW); // The thread is put to sleep, when sensors activated the thread is woken up
      digitalWrite(LED_1_ADAPTIVELIGHT, HIGH);
      digitalWrite(LED_2_ADAPTIVELIGHT, HIGH);
        client.publish("streetlight","{\"id\":\"1\", \"status\":\"on\"}");  //format to send mqtt message ( the (/"/") is used to exit the message and continue writting
        client.publish("streetlight","{\"id\":\"2\", \"status\":\"on\"}");
    lastTimeBlink = millis();
     PT_WAIT_UNTIL(pt,millis() - lastTimeBlink > 4000); // Wait 4sec ( millis acts like delay but doesn't block the code when activated )
      digitalWrite(LED_1_ADAPTIVELIGHT, LOW);
        client.publish("streetlight","{\"id\":\"1\", \"status\":\"off\"}");
      digitalWrite(LED_3_ADAPTIVELIGHT,HIGH);
       client.publish("streetlight","{\"id\":\"3\", \"status\":\"on\"}");
    lastTimeBlink = millis();
     PT_WAIT_UNTIL(pt,millis() - lastTimeBlink > 4000);
      digitalWrite(LED_2_ADAPTIVELIGHT, LOW);
       client.publish("streetlight","{\"id\":\"2\", \"status\":\"off\"}");
    lastTimeBlink = millis();
     PT_WAIT_UNTIL(pt,millis() - lastTimeBlink > 3000);
      digitalWrite(LED_3_ADAPTIVELIGHT, LOW);
       client.publish("streetlight","{\"id\":\"3\", \"status\":\"off\"}");
    
  }
  // Stop a protothread
  PT_END(pt);
}



static int protothreadMOTOR ( struct pt *pt) // This is our second protothread
{ 

  
  // Start a protothread
  static unsigned long lastTimeBlink =0;
  PT_BEGIN(pt);
  
  while (1) {
    lastTimeBlink = millis();
      PT_WAIT_UNTIL(pt,millis() - lastTimeBlink > 1);
        for(int s=0; s<stepsPerRevolution;s++)
         {
             myStepper1.step(1);
             myStepper2.step(1);
         }
  }
  // Stop a protothread
  
  PT_END(pt);



}


static int protothreadMQTTControl ( struct pt *pt) // This is our third protothread
{ 

  
  // Start a protothread
  static unsigned long lastTimeBlink =0;
  PT_BEGIN(pt);
  
  while (1) {
    lastTimeBlink = millis();
    PT_WAIT_UNTIL(pt,millis() - lastTimeBlink > 1);
    client.loop();
  }
  // Stop a protothread
  
  PT_END(pt);

}


// This allows to use an mqtt message to control an output pin on the arduino

void MQTTcallback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");

  String message;
  for (int i = 0; i < length; i++) {
    message = message + (char)payload[i];  //Conver *byte to String
  }
   Serial.print(message);
  if(message == "on")  {digitalWrite(LED_1_INDICATORLIGHT,HIGH), digitalWrite(LED_2_INDICATORLIGHT,HIGH), digitalWrite(LED_3_INDICATORLIGHT,HIGH);}   // When you receive mqtt message, "on", turn on the LED 
  if(message == "off") {digitalWrite(LED_1_INDICATORLIGHT,LOW),  digitalWrite(LED_2_INDICATORLIGHT,LOW),  digitalWrite(LED_3_INDICATORLIGHT,LOW);} // When you receive mqtt message, "off" turn on the LED
 
  Serial.println();
  Serial.println("-----------------------");  
}
 




void setup() {
  // Init your protothread(s)
  PT_INIT(&pt1);
  PT_INIT(&pt2);
  PT_INIT(&pt3);
  //initialise LEDs & sensor
  pinMode (LED_1_PIN, OUTPUT);
  pinMode (LED_2_PIN, OUTPUT);
  pinMode (LED_3_PIN, OUTPUT);
  pinMode (LED_1_INDICATOR, OUTPUT);
  pinMode (LED_2_INDICATOR, OUTPUT);
  pinMode (LED_3_INDICATOR, OUTPUT);
  pinMode (SENSOR_PIN1, INPUT);
  //initialise motors and set the speed 
  myStepper1.setSpeed(15);
  myStepper2.setSpeed(15);


  //MQTT

  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 

  client.setServer(mqttServer, mqttPort);
  client.setCallback(MQTTcallback);

  while (!client.connected()) { // send a message in the serial monitor to check is connection is ok
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client")) {
 
      Serial.println("connected");  
 
    }
 }

 client.subscribe("indicativelight");
}
void loop() {
  // Execute your protothread(s)
   protothreadSENSOR(&pt1);
   protothreadMOTOR(&pt2);
   protothreadMQTTControl(&pt3);
}
