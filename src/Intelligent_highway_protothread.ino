#include <pt.h>
#include <Stepper.h> // library that handles the stepper motors
#include <WiFiNINA.h> // library that handles the wifi connectivity 
#include <PubSubClient.h> // library that allows mqtt communication
#define LED_1_PIN A5 // Set the pins connected to the arduino
#define LED_2_PIN 5
#define LED_3_PIN A4
#define SENSOR_PIN1 A0
#define LED_3_INDICATOR A1
#define LED_2_INDICATOR A2
#define LED_1_INDICATOR A3

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
      digitalWrite(LED_1_PIN, HIGH);
      digitalWrite(LED_2_PIN, HIGH);
        client.publish("streetlight","{\"id\":\"1\", \"status\":\"on\"}");  //format to send mqtt message ( the (/"/") is used to exit the message and continue writting
        client.publish("streetlight","{\"id\":\"2\", \"status\":\"on\"}");
    lastTimeBlink = millis();
     PT_WAIT_UNTIL(pt,millis() - lastTimeBlink > 4000); // Wait 4sec ( millis acts like delay but doesn't block the code when activated )
      digitalWrite(LED_1_PIN, LOW);
        client.publish("streetlight","{\"id\":\"1\", \"status\":\"off\"}");
      digitalWrite(LED_3_PIN,HIGH);
       client.publish("streetlight","{\"id\":\"3\", \"status\":\"on\"}");
    lastTimeBlink = millis();
     PT_WAIT_UNTIL(pt,millis() - lastTimeBlink > 4000);
      digitalWrite(LED_2_PIN, LOW);
       client.publish("streetlight","{\"id\":\"2\", \"status\":\"off\"}");
    lastTimeBlink = millis();
     PT_WAIT_UNTIL(pt,millis() - lastTimeBlink > 3000);
      digitalWrite(LED_3_PIN, LOW);
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
  if(message == "on") {digitalWrite(LED_1_INDICATOR,HIGH), digitalWrite(LED_2_INDICATOR,HIGH), digitalWrite(LED_3_INDICATOR,HIGH);}   // When you receive mqtt message, "on", turn on the LED 
  if(message == "off") {digitalWrite(LED_1_INDICATOR,LOW);} // When you receive mqtt message, "off" turn on the LED
 
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

 client.subscribe("LEDON");
}
void loop() {
  // Execute your protothread(s)
   protothreadSENSOR(&pt1);
   protothreadMOTOR(&pt2);
   protothreadMQTTControl(&pt3);
}
