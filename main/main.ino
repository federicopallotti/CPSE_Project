#include <Stepper.h>
// #include "Adafruit_VL53L0X.h"

// Adafruit_VL53L0X lox = Adafruit_VL53L0X();

// Number of steps to do one full plate rotation
const int STEPS_PER_ROTATION = 2000;
// Number of steps to rotate the plate to the next scan point
// there are actually 32*63.68395 steps per revolution = 2037.8864 ~ 2038 steps!
const int Actual_STEPS_PER_ROTATION = 2048;
const int stepsPerRotationIncrement = Actual_STEPS_PER_ROTATION/90;
// Number of steps to raise the sensor to the next scan level
const int stepsPerHeightIncrement = 200;

// Ports attribution
Stepper myHeightStepper(STEPS_PER_ROTATION, 0, 1, 2, 3);
// 20cm height
const int endstopButtonTop = 4;
const int endstopButtonBottom = 5;
const int toggleStartButton = 6;
Stepper myRotationStepper(STEPS_PER_ROTATION, 7, 8, 9, 10);
// // 11 and 12 are for range
// const int rangeSDA = 11;
// const int rangeSCL = 12;
String FILENAME = "./pointcloud";
int rotation_step_counter = 0;
int height_step_counter = 0;
bool partial_empty_tour = true;
bool empty_tour = false;

void setup() {
  pinMode(toggleStartButton, INPUT);
  pinMode(endstopButtonTop, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);  

  Serial.begin(9600);
  Serial1.begin(9600);            // initialize UART with baud rate of 9600 for Arduino Nano

}

void loop() {
  // Wait until button push to start scan

	static bool isRunning = false;

	if (!isRunning && startPressed()) {
    // Create a new point cloud file at FILENAME
		isRunning = true;
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Starting...");
    delay(500);
	}

	if (isRunning && (startPressed()||stopPressed()||empty_tour)) {
		isRunning = false;
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Stopping...");
    delay(500);
    stop();
	}

	if (isRunning){
		doStuff();
	}
}

//******************* BUTTONS *******************

bool startPressed() {
// check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  return digitalRead(toggleStartButton)== HIGH;
}

bool stopPressed() {
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  return digitalRead(endstopButtonTop)== HIGH;
}

bool bottomEndstopPressed() {
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  return digitalRead(endstopButtonBottom)== HIGH;
}

//******************* ACTIVATIONS *******************

void doStuff(){
  scan_data_point();
  rotation_step();
  if (reachedFullRotation())
    height_step();  
}

void stop(){
  transmit_file();
  reset();
}

//******************* SCANNING *******************

void scan_data_point(){
  delay(500);
  Serial.print("Scanning...");
  
  while (Serial1.available() >= 0) {
    // start range computation
    Serial1.println('1');
    int value = Serial1.parseInt();
    int wait_count=1;
    // Wait for a response that is not 0
    while(value==0){
      Serial.print("\t Skipped ");
      Serial.print(wait_count);
      Serial.print(": ");
      Serial.println(value);
      value = Serial1.parseInt();
      wait_count++;
      if (wait_count>=10) break;
    }

    if(value>100 && value<300){
      Serial.print("Result: ");
      Serial.println(value);
      partial_empty_tour = false;
      break;
    } else {
      Serial.print("Ignored result: ");
      Serial.println(value);
      break;
    }
  }
  delay(200);
}

//******************* STEPPER MOTORS *******************

bool reachedFullRotation(){
  if (rotation_step_counter >= Actual_STEPS_PER_ROTATION){
    rotation_step_counter = 0;
    return true;
  }
  return false;
}

void rotation_step(){
  // do one rotation step
  for(int i=0; i<stepsPerRotationIncrement; i++){
    myRotationStepper.step(1);
    rotation_step_counter++;
    delay(5);
  }
  Serial.print("Rotation steps:");
  Serial.println(rotation_step_counter);
}


void height_step(){
  // do one height step
  for(int i=0; i<stepsPerHeightIncrement; i++){
    myHeightStepper.step(1);
    height_step_counter++;
    delay(5);
  }
  Serial.print("Height steps:");
  Serial.println(height_step_counter);
  if(partial_empty_tour) empty_tour=true;
}

//******************* WIFI *******************

void transmit_file(){
  // TODO
  // close point cloud file at FILENAME
  // open communication protocol
  // send file
  // close communication protocol

  Serial.println("Transmitting file...");
  delay(2000);
}

//******************* RESET *******************

void reset(){
  // do negative hight step until height_step_counter = 0
  // go back to waiting

  Serial.println("Resetting...");
  while(!bottomEndstopPressed() && !startPressed()){
    myHeightStepper.step(-1);
    delay(5);
  }
  for(; rotation_step_counter < Actual_STEPS_PER_ROTATION; rotation_step_counter++){
    myRotationStepper.step(1);
    delay(5);
  }
  rotation_step_counter = 0;
  height_step_counter = 0;
  Serial.println("Reset ended.");
}
