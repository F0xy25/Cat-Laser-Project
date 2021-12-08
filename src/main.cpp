#include <Arduino.h>
#include <Servo.h>
#include <HID.h>
//https://www.arduino.cc/en/Tutorial/BuiltInExamples/JoystickMouseControl
//https://github.com/arduino-libraries/Servo/blob/master/examples/Sweep/Sweep.ino

// set pin numbers for switch, joystick axes, and LED:
const int switchPin = 2;      // switch to turn on and off mouse control
const int mouseButton = 3;    // input pin for the mouse pushButton
const int xAxis = A1;         // joystick X axis
const int yAxis = A2;         // joystick Y axis
//const int ledPin = 5;         // Mouse control LED

// parameters for reading the joystick:
int range = 12;               // output range of X or Y movement
int responseDelay = 5;        // response delay of the mouse, in ms
int threshold = range / 4;    // resting threshold
int center = range / 2;       // resting position value

bool mouseIsActive = false;    // whether or not to control the mouse
int lastSwitchState = LOW;        // previous switch state

bool isAutomated = false;      //current state if automation code is active
//Servos

Servo panServo;
Servo tiltServo;

int panPos = 0; //position in degrees of pan servo
int tiltPos = 0; //position in degrees of tilt servo

int serspeedfact = 0; //the factor that allows to compute for relative movement speed of 

//write the script for randomized movements of the pan/tilt mechanism with the
void automated(){
  
  int randomPan = random(40,140);
  int randomTilt = random(40,140);
  panServo.write(randomPan);
  tiltServo.write(randomTilt);
  delay(1000);
}

//take raw analog from joystick, convert into usable values
int readAxis(int pin) {
  // read the analog input:
  int reading = analogRead(pin);
  // map the reading from the analog input range to the output range:
  reading = map(reading, 0, 1023, 40, 140);
  // if the output reading is outside from the rest position threshold, use it:
  int distance = reading - center;
  if (abs(distance) < threshold) {
    distance = 0;
  }

  // return the distance for this axis:
  return distance;
}

//for a specific servo, take distance from joystick and convert to movement speed in a given direction
void moveServo(int servoNum, int distance){
//take distance, divide by possible range
    int distfact = distance/range;
  if (servoNum == 0){
    //map distance factor to one of three speeds, slow, medium, fast
    if (distfact > 0 && distfact <= 1){
      serspeedfact = 1;
      //move at 1 deg/interval
      panServo.write(panPos+1);
    }
    else if (distfact > 1 && distfact <= 2 ){
      serspeedfact = 2;
      //move at 2 deg/interval
      panServo.write(panPos+2);
    }
    else{
      serspeedfact = 3;
      //move at 3 deg/interval
      panServo.write(panPos+3);
    }
  }
  else if (servoNum == 1){
    //map distance factor to one of three speeds, slow, medium, fast
    if (distfact > 0 && distfact <= 1){
      serspeedfact = 1;
      //move at 1 deg/interval
      panServo.write(panPos+1);
    }
    else if (distfact > 1 && distfact <= 2 ){
      serspeedfact = 2;
      //move at 2 deg/interval
      panServo.write(panPos+2);
    }
    else{
      serspeedfact = 3;
      //move at 3 deg/interval
      panServo.write(panPos+3);
    }

  }
  else{
    Serial.println("Not a valid Servo ID");
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(switchPin, INPUT_PULLUP);
  //set servo pins, 
  panServo.attach(4);
  tiltServo.attach(3);

  pinMode(xAxis, INPUT);
  pinMode(yAxis, INPUT);

  Serial.println("Setup Complete");
}

void loop() {
  // read and scale the two axes:
  int xReading = readAxis(xAxis);
  int yReading = readAxis(yAxis);

  //For testing, print ouput of joystick readings
  Serial.println(xReading);
  Serial.println(yReading);

  panServo.write(xReading);
  tiltServo.write(yReading);

  // read the mouse button and click or not click:
  // if the mouse button is pressed:
  Serial.println(digitalRead(switchPin));
  if (digitalRead(switchPin) == LOW) {
    // if the mouse is not pressed, press it:
    delay(1000);
    while (digitalRead(switchPin) == HIGH){
      Serial.println("Now Automating");
      isAutomated = true;
      automated();
    }
    delay(1000);
  }

  delay(responseDelay);
}

/*
  reads an axis (0 or 1 for x or y) and scales the analog input range to a range
  from 0 to <range>
*/

