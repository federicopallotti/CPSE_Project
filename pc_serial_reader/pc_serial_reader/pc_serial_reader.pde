import processing.serial.*;
Serial mySerial; 
PrintWriter output;

Boolean waiting = true;
int file_number = 1;
 
//find port with: ls /dev/tty.*

String start = "$";
String end = "&";
String close = "@";
    
void setup(){
  mySerial = new Serial(this, "/dev/tty.usbmodem143401", 9600);
  output = createWriter(get_filename());
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
      waiting = false;
    } else if (!waiting && value.equals(end)){
      waiting = true;
      println(" -END");
    } else if (!waiting && value != null){
      output.println(value);
      print("- stored");
      println(value);
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
  
  // Create the next file;
  file_number++;
  output = createWriter(get_filename());
}

String get_filename(){
  // Format: "./pointcloudX.xyz";
  String name = "./pointcloud"+str(file_number)+".xyz";
  print("Filename: ");
  println(name);
  return name;
}
