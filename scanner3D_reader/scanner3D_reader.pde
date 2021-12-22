import processing.serial.*;
Serial mySerial; 
PrintWriter output;

Boolean waiting = true;
int file_number = 1;
 
//find port with: ls /dev/tty.*

String start = "$";
String end = "&";
String close = "@";
Boolean file_started = true;
    
void setup(){
  size(800, 400);
  textSize(18);
  text("Starting scanner...", 40, 40);
  
  println("Select port:");
  printArray(Serial.list());
  for (int i = 0; i < Serial.list().length; i++){
    String[] matching_port = match(Serial.list()[i], "tty.usbmodem");
    if (matching_port != null) {
      mySerial = new Serial(this, Serial.list()[i], 9600);
      text("Using port: "+Serial.list()[i], 40, 80);
    }
  }
  //mySerial = new Serial(this, "/dev/tty.usbmodem143401", 9600);
  output = createWriter(get_filename());  
  
  text("Scanner ready. Push the blue button on the scanner to start.", 40, 120);
}

void draw(){
  if (mySerial.available() > 0){
    String value = trim(mySerial.readString());
    println(value);
    
    if (waiting && value.equals(close)){
      terminate();
      waiting = true;
    } else if (waiting && value.equals(start)){
      println(" -START ");
      if (!file_started){
        // Create the next file;
        output = createWriter(get_filename());
        file_started = true;
      }
      waiting = false;
    } else if (!waiting && value.equals(end)){
      waiting = true;
      println(" -END");
    } else if (!waiting && value != null){
      output.println(value);
    }
  }
}

void terminate(){
  output.flush();
  // Writes the remaining data to the file
  output.close();
  // Finishes the file
  print("Produced file: ");
  println(get_filename());
  text("Produced file: "+get_filename(), 40, 120+40*file_number);
  
  // Reset flag for file creation
  file_number++;
  file_started = false;
}

String get_filename(){
  // Format: "./pointcloudX.xyz";
  String name = "./pointcloud"+str(file_number)+".xyz";
  print("Filename: ");
  println(name);
  return name;
}
