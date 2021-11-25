int rotation_step_counter = 0;
string FILENAME = "./pointcloud";

void setup() {
  

}

void loop() {
  // Wait until button push to start scan

  // Create a new point cloud file at FILENAME

while (not interrupted) {
  scan_data_point();
  rotation_step();
  if (rotation_step_counter is at full rotation)
    HEIGHT_STEP
  }

  transmit_file()
  reset()
}

void scan_data_point(){
  // get distance from sensor distance
  // save distance to point cloud file   
}

void rotation_step(){
  // do one rotation step
}


void height_step(){
  // do one height step
}

void transmit_file(){
  // close point cloud file at FILENAME
  // open communication protocol
  // send file
  // close communication protocol
}

void reset(){
  // rotation_step_counter = 0
  // do negative hight step until height_step_counter = 0
  // go back to waiting
}
