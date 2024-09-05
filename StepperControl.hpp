#pragma once

class StepperControl {

  public:

  // setup up pins
  void init();

  // find origin of stepper motors
  void findOrigin();

  // check if all stepper have made it to their target
  int atTarget();

  // overload of moveTo with targets as parameters
  void moveTo(int targPos0, int targPos1);

  // overload of moveTo with targets and speed/delayTime as parameters
  void moveTo(int targPos0, int targPos1, long delayTime);

  // make arm move to a position using all steppers (full method with all parameters, final parameter is a function pointer)
  void moveTo(int targPos0, int targPos1, long delayTime, int (StepperControl::*atTarget)());



  private:

    const int dirPin0 = 2;
    const int stepPin0 = 3;

    const int dirPin1 = 12;
    const int stepPin1 = 13;

    const int buttonPin0 = 9; // 6
    const int buttonPin1 = 10; // 7

    int currentPosition0 = 0;
    int currentPosition1 = 0;
    int targetPosition0 = 0;
    int targetPosition1 = 0;

    // motor speeds (slowSpeed used in findOrigin())
    int defaultSpeed = 500;
    int slowSpeed = 1000;

    // generalize control per motor (use pointers to modify individual properties for each motor)
    void spinMotor(int stepPin, int* currentPositionPtr, int increment, int targetPosition, long* startTime, long delayTime);

    // check if button0 is pressed (wrapper of digitalRead will be used with function pointer system)
    int atButton0();


    // check if button1 is pressed (wrapper of digitalRead will be used with function pointer system)
    int atButton1();
};
