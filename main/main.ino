#include <Stepper.h>
//#include "Adafruit_VL53L0X.h"
//#include <VL53L0X.h>
//Adafruit_VL53L0X rangeSensor = Adafruit_VL53L0X();

// Number of steps to do one full plate rotation
const int STEPS_PER_ROTATION = 2000; //2000
// Number of steps to rotate the plate to the next scan point
const int stepsPerRotationIncrement = STEPS_PER_ROTATION/16;
// Number of steps to raise the sensor to the next scan level
const int stepsPerHeightIncrement = 200;

// Ports attribution
Stepper myHeightStepper(STEPS_PER_ROTATION, 0, 1, 2, 3);
const int endstopButtonTop = 4;
const int endstopButtonBottom = 5;
const int toggleStartButton = 6;
Stepper myRotationStepper(STEPS_PER_ROTATION, 7, 8, 9, 10);
String FILENAME = "./pointcloud";
int rotation_step_counter = 0;
int height_step_counter = 0;

void setup() {
  pinMode(toggleStartButton, INPUT);
  pinMode(endstopButtonTop, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);  
  Serial.begin(9600);

}

void loop() {
  // Wait until button push to start scan

	static bool isRunning = false;


  // DEGUG CORNER
  // END DEGUG CORNER

	if (!isRunning && startPressed()) {
    // Create a new point cloud file at FILENAME
		isRunning = true;
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Starting...");
    delay(1000);
	}

	if (isRunning && (startPressed()||stopPressed())) {
		isRunning = false;
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Stopping...");
    delay(2000);
    stop();
	}

	if (isRunning){
		doStuff();
	}
}

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

void scan_data_point(){
  // TODO
  Serial.print("Scanning...");
  // get distance from sensor distance
//  VL53L0X_RangingMeasurementData_t measure;
//    
//  Serial.print("Reading a measurement... ");
//  rangeSensor.rangingTest(&measure, false); // pass in 'true' to get debug data printout!
//
//  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
//    Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
//  } else {
//    Serial.println(" out of range ");
//  }
    
//  delay(100);
  // save distance to point cloud file   
  delay(1000);
}

bool reachedFullRotation(){
  if (rotation_step_counter >= STEPS_PER_ROTATION){
    rotation_step_counter = 0;
    return true;
  }
  return false;
}

void rotation_step(){
  // do one rotation step
  for(int i=0; i<stepsPerHeightIncrement; i++){
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
}

void transmit_file(){
  // TODO
  // close point cloud file at FILENAME
  // open communication protocol
  // send file
  // close communication protocol

  Serial.println("Transmitting file...");
  delay(2000);
}

void reset(){
  // TODO
  // rotation_step_counter = 0
  // do negative hight step until height_step_counter = 0
  // go back to waiting

  Serial.println("Resetting...");
  while(!bottomEndstopPressed() && !startPressed()){
    myHeightStepper.step(-1);
    delay(5);
  }
  for(; rotation_step_counter < STEPS_PER_ROTATION; rotation_step_counter++){
    myRotationStepper.step(1);
    Serial.print("Roration steps:");
    Serial.println(rotation_step_counter);
    delay(5);
  }
  rotation_step_counter = 0;
  height_step_counter = 0;
  Serial.println("Reset ended.");
}
