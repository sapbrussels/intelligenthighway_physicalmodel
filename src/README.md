# The Arduino source code

In this code we use a protothreading library that allow to run two task simultaniously.

In the first thread we turn an LED on when the sensors is activated, and the second one drives the motors.

The libraries used are the following : 

* pt.h ( for protothreading )
* stepper.h ( for the motors )

## Protothreading

What is protothreading  :

Protothreading is the solution to multithreading which can be used with the Arduino microprocessor, it doesn’t actually run two tasks at the same time it just simulates the feeling of it.

### How to use it
Follow the code to understand better :

#include <pt.h> // include protothreading library

// Create your protothread(s) static struct pt pt1; // ad pt2, pt3 depending on how many threads you have in your code

// A protothread function static int protothreadFunction(struct pt *pt) { // Start a protothread PT_BEGIN(pt);

/* Your code inside this thread */

// Stop a protothread PT_END(pt); }

void setup() { // Init your protothread(s) PT_INIT(&pt1); }

void loop() { // Execute your protothread(s) protothreadFunction(&pt1); } }

Link to the tutorial I followed : https://roboticsbackend.com/arduino-protothreads-tutorial/

## Stepper motor

To control the stepper motors the library stepper.h is used. You need a 12v power source for it to work.
Here is a link to the exact motor : https://www.amazon.com/ELEGOO-28BYJ-48-ULN2003-Stepper-Arduino/dp/B01CP18J4A
link to the code used : https://www.makerguides.com/28byj-48-stepper-motor-arduino-tutorial/

