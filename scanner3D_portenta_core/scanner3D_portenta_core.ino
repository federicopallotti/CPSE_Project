#include <Stepper.h>

// Number of steps to do one full plate rotation
const int stepper_speed = 2000;
const int steps_per_rotation = 2025; // 2048 best for points_per_rotation=90


const int points_per_rotation = 90;

// Number of steps to rotate the plate to the next scan point
// there are actually 32*63.68395 steps per revolution = 2037.8864 ~ 2038 steps
const int stepsPerRotationIncrement = steps_per_rotation/points_per_rotation;
// Number of steps to raise the sensor to the next scan level
// 14.3cm every 10'000 steps. ~350steps/0.5mm
const int stepsPerHeightIncrement = 350;

// Ports attribution
Stepper myHeightStepper(stepper_speed, 0, 1, 2, 3);
// 20cm height
const int endstopButtonTop = 4;
const int endstopButtonBottom = 5;
const int toggleStartButton = 6;
Stepper myRotationStepper(stepper_speed, 7, 8, 9, 10);
// // 11 and 12 are for range
// const int rangeSDA = 11;
// const int rangeSCL = 12;
struct datapoint
{
  int x;
  int y;  
  int z;   
};

struct datapoint slice [91];  

int rotation_step_counter = 0;
int height_step_counter = 0;
bool partial_empty_tour = true;
bool empty_tour = false;

// Sensor-plate center distance
int fixed_dist = 191;

void setup() {
  pinMode(toggleStartButton, INPUT);
  pinMode(endstopButtonTop, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);  

  Serial.begin(9600);
  Serial1.begin(9600);            // initialize UART with baud rate of 9600 for Arduino Nano
  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  Serial.println("End of Setup");
}

void loop() {
  // Wait until button push to start scan

	static bool isRunning = false;
  

	if (!isRunning && startPressed()) {
    // Create a new point cloud file at FILENAME
		isRunning = true;
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Starting...");
    delay(200);
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

//******************* GROUP ACTIVATIONS *******************

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
  // Delay to compensate for the rotation wobble of the rotation plate  
  delay(200);
  
  while (Serial1.available() >= 0) {
    // empty Serial1 until you see "0"
    while(Serial1.parseInt()!=0){
      delay(1);
    }
    
    // Signal to Arduino Nano to start the range computation
    Serial1.println('1');
    int value = Serial1.parseInt();
    int wait_count=1;
    // Wait for a response that is not 0
    while(value==0){
      value = Serial1.parseInt();
      wait_count++;
      // Get the first 3 instances before discarding the result 
      if (wait_count>=3) break;
    }

    if(value>100 && value<300){
      Serial.print("Result: ");
      Serial.println(value);
      partial_empty_tour = false;
      
      //output equations
      int output_x = - (fixed_dist - value)*sin(get_angle());
      int output_y = get_height();
      int output_z = (fixed_dist - value)*cos(get_angle());

      // Store the data as a datapoint struct in the slice
      int index = rotation_step_counter/stepsPerRotationIncrement;
      slice[index].x = output_x;
      slice[index].y = output_y;
      slice[index].z = output_z;
      break;

    } else {
      // Dont store these results
      Serial.print("Ignored result: ");
      Serial.println(value);
      break;
    }
  }
}

//******************* DATA CONVERSION *******************

// Angle in degrees
int get_angle(){
  return (360*rotation_step_counter/2048);
}

// Height in mm
int get_height (){
  return (height_step_counter*143/10000);
}




//******************* STEPPER MOTORS *******************

bool reachedFullRotation(){
  if (rotation_step_counter >= steps_per_rotation){
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
    
  // Send slice to file
  send_slice();

  // If the last slice was completely empty, then stop
  if(partial_empty_tour) empty_tour=true;
  partial_empty_tour = true;
}

//******************* COMMUNICATION *******************

void send_slice(){
  // Send the start symbol "$"
  delay(150);
  Serial.println("$");
  for(int i = 0; i < 90; i++)    {
    // TODO: check if it's not missing a point
    if (slice[i].x!=0 && slice[i].z!=0){
      delay(150);
      Serial.print(slice[i].x);
      Serial.print(" ");
      Serial.print(slice[i].y);
      Serial.print(" ");
      Serial.println(slice[i].z);
    }
  }
  delay(150);
  // Send slice termination character
  Serial.println("&");
  delay(150);
  // Reset array
  for (int i = 0; i < 90; i++){
    if (slice[i].x == 0 && slice[i].y == 0 && slice[i].z == 0) break;
    slice[i].x = 0;
    slice[i].y = 0;
    slice[i].z = 0;
  }
}

void transmit_file(){
  Serial.println("Transmitting file...");
  delay(100);

  // Send char to close point cloud file
  Serial.println("@");
  delay(100);
}

//******************* RESET *******************

void reset(){

  Serial.println("Resetting...");

  // do negative hight step until hitting the endstop or until stopped manually
  while(!bottomEndstopPressed() && !startPressed()){
    myHeightStepper.step(-1);
    delay(5);
  }
  // complete the plate rotation, aka "Victory Lap"
  for(; rotation_step_counter < steps_per_rotation; rotation_step_counter++){
    myRotationStepper.step(1);
    delay(5);
  }
  rotation_step_counter = 0;
  height_step_counter = 0;
  empty_tour = false;
  Serial.println("Reset ended.");
  // go back to waiting
}
