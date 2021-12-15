
#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 2, 3, 4, 5);

int stepCount = 0;         // number of steps the motor has taken
// int endstop_x = 5;
bool isRunning = true;

void setup() {
  // initialize the serial port:
  Serial.begin(9600);
  // pinMode(endstop_x, INPUT);
}

void loop() {
  // if (stopPressed()) {
	// 	isRunning = false;
  //   Serial.println("Stop");
	// }else{
  //   isRunning = true;
  // }

  // step one step:
  // if (isRunning){
  myStepper.step(3);
  Serial.print("steps:");
  Serial.println(stepCount);
  stepCount++;
  delay(500);
  // }else{
  //   Serial.println("Stopped");
  // }
    
}


// bool stopPressed() {
//   // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
//   return digitalRead(endstop_x) == HIGH;
// }
