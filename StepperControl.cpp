#include "StepperControl.hpp"
#include <Arduino.h>

// setup up pins
void StepperControl::init() {
  // Declare pins as Outputs
	pinMode(stepPin0, OUTPUT);
	pinMode(dirPin0, OUTPUT);
  pinMode(stepPin1, OUTPUT);
	pinMode(dirPin1, OUTPUT);
  pinMode(buttonPin0, INPUT);
  pinMode(buttonPin1, INPUT);

  // Set motor direction clockwise
	digitalWrite(dirPin0, HIGH);
  digitalWrite(dirPin1, LOW);
}


// find origin of stepper motors
void StepperControl::findOrigin() {
  int fullRotation = 10000; // how many steps is full joint rotation?
  
  // search for motor0 origin while motor1 is not moving (force to not move by tricking it into thinking its at its target)
  currentPosition0 = 0;
  currentPosition1 = fullRotation;
  moveTo(fullRotation, fullRotation, slowSpeed, &atButton0);

  // search for motor1 origin while motor0 is not moving (force to not move by tricking it into thinking its at its target)
  currentPosition0 = fullRotation;
  currentPosition1 = 0;
  moveTo(fullRotation, fullRotation, slowSpeed, &atButton1);
  
  // origins have been found so set new 0
  currentPosition0 = 0;
  currentPosition1 = 0;
}


// check if all stepper have made it to their target
int StepperControl::atTarget() {
  return (int) (currentPosition0 == targetPosition0 && currentPosition1 == targetPosition1);
}


// check if button0 is pressed (wrapper of digitalRead will be used with function pointer system)
int StepperControl::atButton0() {
  return digitalRead(buttonPin0);
}


// check if button1 is pressed (wrapper of digitalRead will be used with function pointer system)
int StepperControl::atButton1() {
  return digitalRead(buttonPin1);
}


// overload of moveTo with targets as parameters
void StepperControl::moveTo(int targPos0, int targPos1) {
  moveTo(targPos0, targPos1, defaultSpeed);
}


// overload of moveTo with targets and speed/delayTime as parameters
void StepperControl::moveTo(int targPos0, int targPos1, long delayTime) {
  moveTo(targPos0, targPos1, delayTime, &StepperControl::atTarget);
}


// make arm move to a position using all steppers (full method with all parameters, final parameter is a function pointer)
void StepperControl::moveTo(int targPos0, int targPos1, long delayTime, int (StepperControl::*atTarget)()) {
  // set global scope target values
  targetPosition0 = targPos0;
  targetPosition1 = targPos1;

  // will use to add or subtract from current position
  int positionInc0 = 1;
  int positionInc1 = 1;

  // set direction of motors based on where targ and curr are relative to each other
  if (targetPosition0 > currentPosition0) {
    // Set motor0 direction clockwise
    positionInc0 = 1;
    digitalWrite(dirPin0, HIGH);
  }
  else {
    // Set motor0 direction counter-clockwise
    positionInc0 = -1;
    digitalWrite(dirPin0, LOW);
  }
  if (targetPosition1 > currentPosition1) {
    // Set motor1 direction clockwise
    positionInc1 = 1;
    digitalWrite(dirPin1, LOW);
  }
  else {
    // Set motor1 direction counter-clockwise
    positionInc1 = -1;
    digitalWrite(dirPin1, HIGH);
  }

  // reference time
  long startTime0 = micros();
  long startTime1 = micros();

  // pointers for current position
  int* currentPosPtr0 = &currentPosition0;
  int* currentPosPtr1 = &currentPosition1;

  // pointers for start time
  long* startTimePtr0 = &startTime0;
  long* startTimePtr1 = &startTime1;
  
  // continue motion until all motors are at their target position
  while (!(this->*atTarget)()) {
    // check if motor is at target, if not then spin motor
    // Serial.begin(500000);
    // Serial.println((this->*atTarget)());
    spinMotor(stepPin0, currentPosPtr0, positionInc0, targetPosition0, startTimePtr0, delayTime);
    spinMotor(stepPin1, currentPosPtr1, positionInc1, targetPosition1, startTimePtr1, delayTime);
  }
}


// generalize control per motor (use pointers to modify individual properties for each motor)
void StepperControl::spinMotor(int stepPin, int* currentPositionPtr, int increment, int targetPosition, long* startTime, long delayTime) {  
  // check if motor is at target, if not do digitalWrite
  if (*currentPositionPtr != targetPosition) {
    long currentTime = micros() - *startTime;

    // run motor for delayTime on HIGH
    if (currentTime < delayTime) {
      digitalWrite(stepPin, HIGH);
    }
    // run motor for an additional delayTime but on LOW
    else if (currentTime < delayTime * 2) {
      digitalWrite(stepPin, LOW);
    }
    else {
      *currentPositionPtr += increment;
      *startTime = micros();
    }
  }
}