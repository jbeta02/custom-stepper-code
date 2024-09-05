#include "StepperControl.hpp"

StepperControl stepperControl;

void setup() {
  // setup stepper pins
  stepperControl.init();

  // find origin of stepper motors
  //stepperControl.findOrigin();
}

void loop() {

  // move to a position then turn back abound
  stepperControl.moveTo(200, 1000);
  stepperControl.moveTo(-200, -1000);
}