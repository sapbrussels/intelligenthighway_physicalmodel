# Intelligent Highway Physical Model

This physical model helps to tell the story of the intelligent highway.

Here is a brief explanation of what the physical model displays:

When a car passes by a sensor, the streetlights are tuned on when the car doesn't need the light it is switched off. This is what the physical model displays. There is an additional light on the streetlight, this is called an indicative light, it notifies the drivers when there is an issue ahead.

Lets see in detail each part of the physical model:

## How do the cars move? 

The car is pulled thanks to the help of two 28BYJ-48 arduino stepper motor, attached to these motors are 2 pulleys and a belt that runs through the pulleys, on the belt there are magnets that pulls the car.  

The magnet is placed on an L bracket and fixed onto the belt, this in beneficial for two reasons: prevent the belt from coming off the gear and adapt the height of the magnet. 


## Microcontroller 

I used the arduino uno wifi, the advantage of this arduino is that it has more pins than an ESP and can be connected to the wifi which is a key feature to connect to the webapp.

### How to set it up 

#### a) Install board

* Launch Arduino
* Open "Tools/Board/Boards Manager"
* Put arduino "Arduino Uno WiFi Rev2" in "Filter your search..." and install "Arduino megaAVR Boards"
* Click "Close" when the board is installed
* Click "Tools/Board/Arduino Uno WiFi Rev2"
* Then you should see Arduino Uno WiFi Rev2 in Tools/Board
* Click "Tools/Port" and select the port that the arduino is connected to


#### b) Test your board

* Click on "Upload". "Upload" is a button under "Edit" normally
* Wait until you see "Done compiling" (it takes some time the first time)
* You should see a message like that: "Verify successful, done in 0.012 seconds, CPU reset."

### Libraries used 

* pt.h
* Stepper.h

### How to use protothreading 

### What is protothreading 

Protothreading is the solution to multithreading which can be used with the Arduino microprocessor, it doesn’t actually run two tasks at the same time it just simulates the feeling of it.

### How to use it 


Follow the code to understand better : 

#include <pt.h> // include protothreading library 

// Create your protothread(s)
static struct pt pt1; // ad pt2, pt3 depending on how many threads you have in your code

// A protothread function
static int protothreadFunction(struct pt *pt)
{
  // Start a protothread
  PT_BEGIN(pt);

  /* Your code inside this thread */

  // Stop a protothread
  PT_END(pt);
}

void setup() {
  // Init your protothread(s)
  PT_INIT(&pt1);
}

void loop() {
  // Execute your protothread(s)
  protothreadFunction(&pt1);
}
}

Link to the tutorial I followed : https://roboticsbackend.com/arduino-protothreads-tutorial/


## Car detection 

To detect when a car passes I am using a IR sensor



## Folders

In the `src` directory you can find the Arduino code for controlling the different leds and motor board of the physical model.

In the `scripts` directory you can find scripts to simulate the physical model, this is great for testing.
