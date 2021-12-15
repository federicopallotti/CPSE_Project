#include <Stepper.h>

const int STEPS_PER_ROTATION = 200;
Stepper myRotationStepper(STEPS_PER_ROTATION, 2, 3, 4, 5);
const int startButton = 6;//TODO
const int stopButton = 7;//TODO
Stepper myHeightStepper(STEPS_PER_ROTATION, 8, 9, 10, 11);


String FILENAME = "./pointcloud";
int rotation_step_counter = 0;
int height_step_counter = 0;

void setup() {
  pinMode(startButton, INPUT);
  pinMode(stopButton, INPUT);
  Serial.begin(9600);
}

void loop() {
  // Wait until button push to start scan

	static bool isRunning = false;

	// TODO implement startPressed()
	if (!isRunning && startPressed()) {
    // Create a new point cloud file at FILENAME
		isRunning = true;
    Serial.println("Starting...");
    delay(2000);
	}

	// TODO implement stopPressed()
	if (isRunning && stopPressed()) {
		isRunning = false;
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
  return digitalRead(startButton)== HIGH;
}

bool stopPressed() {
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  return digitalRead(stopButton)== HIGH;
}

void doStuff(){
  scan_data_point();
  rotation_step();
  if (reachedFullRotation(rotation_step_counter))
    height_step();  
}

void stop(){
  transmit_file();
  reset();
}


void scan_data_point(){
  // TODO
  // get distance from sensor distance
  // save distance to point cloud file   
  Serial.println("Scanning...");
  delay(100);
}

bool reachedFullRotation(int current_rotation_step){
  return ((current_rotation_step % STEPS_PER_ROTATION) == 0);
}

void rotation_step(){
  // TODO
  // do one rotation step

  myRotationStepper.step(1);
  Serial.print("Rotation steps:");
  Serial.println(rotation_step_counter);
  rotation_step_counter++;
}


void height_step(){
  // TODO
  // do one height step

  myHeightStepper.step(1);
  Serial.print("Height steps:");
  Serial.println(height_step_counter);
  height_step_counter++;
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
  myRotationStepper.step(-rotation_step_counter);
  myHeightStepper.step(-height_step_counter);
  rotation_step_counter = 0;
  height_step_counter = 0;
  Serial.println("Reset ended.");
}
