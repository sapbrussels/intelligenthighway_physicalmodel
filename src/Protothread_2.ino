#include <pt.h>
#include <Stepper.h>
#define LED_1_PIN A5
#define LED_2_PIN A3
#define LED_3_PIN A4
#define SENSOR_PIN1 A0
#define SENSOR_PIN2 A1
#define SENSOR_PIN3 A3

const int stepsPerRevolution = 2048;
Stepper myStepper1 = Stepper(stepsPerRevolution, 8, 10, 9, 11);
Stepper myStepper2 = Stepper(stepsPerRevolution, 4, 5, 6, 7);
// Create your protothread(s)
static struct pt pt1, pt2, pt3, pt4;
// A protothread function
static int protothreadSENSOR(struct pt *pt)
{
  // Start a protothread
  static unsigned long lastTimeBlink =0;
  PT_BEGIN(pt);
  while (1) {
    lastTimeBlink = millis();
    PT_WAIT_UNTIL(pt, digitalRead(SENSOR_PIN1) == LOW);
    digitalWrite(LED_1_PIN, HIGH);
    digitalWrite(LED_2_PIN, HIGH);
    lastTimeBlink = millis();
    PT_WAIT_UNTIL(pt,millis() - lastTimeBlink > 5000);
    digitalWrite(LED_1_PIN, LOW);
    lastTimeBlink = millis();
    PT_WAIT_UNTIL(pt,millis() - lastTimeBlink > 5000);
    digitalWrite(LED_2_PIN, LOW);
    digitalWrite(LED_3_PIN,HIGH);
    lastTimeBlink = millis();
    PT_WAIT_UNTIL(pt,millis() - lastTimeBlink > 5000);
    digitalWrite(LED_3_PIN, LOW);
    
  }
  // Stop a protothread
  PT_END(pt);
}


static int protothreadMOTOR ( struct pt *pt)
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
void setup() {
  // Init your protothread(s)
  PT_INIT(&pt1);
  PT_INIT(&pt2);
  PT_INIT(&pt3);
  PT_INIT(&pt4);
  //initialise LEDs & sensor
  pinMode (LED_1_PIN, OUTPUT);
  pinMode (LED_2_PIN, OUTPUT);
  pinMode (LED_3_PIN, OUTPUT);
  pinMode (SENSOR_PIN1, INPUT);
  //initialise motors
  myStepper1.setSpeed(22);
  myStepper2.setSpeed(22);
  
}
void loop() {
  // Execute your protothread(s)
   protothreadSENSOR(&pt1);
   protothreadMOTOR(&pt2);
}


 
