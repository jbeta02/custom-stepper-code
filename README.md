# Descripton
Custom stepper code for robot arm project controlled by an Arduino.

# Features

- Contains functions for finding and setting joint origins. Push buttons are used to set the zero position of the stepper motors. 

- Custom movement targets can be set by users using junction pointers. With this approach, a target of a stepper motor can be when a button is pressed, a position is reached, or any other method of setting a target. 

- Open loop position tracking. The position of the stepper mototrs are tracked independently and updated every loop cycle. 

- Many online tutorials for controlling stepper motors with Arduino use delayMicroseconds() to set the frequency of pulses sent to the stepper motors but this creates a problems where only one stepper can be run at a time since the delay function essentially pauses the code flow. 
The solution in this software uses a loop to pass over each motor then tracks the elapsed time since the last pulse. No delay function is used so the motors can be moved at the same time. 

The software was tested on a 2 degree of freedom robot arm but is scalable to match any number of degrees of freedom. 
