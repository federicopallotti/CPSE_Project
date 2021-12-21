#include<string.h>
#include<stdlib.h>

//Sender
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);      // set LED pin as output
  digitalWrite(LED_BUILTIN, LOW);    // switch off LED pin

  Serial.begin(9600);               // initialize serial communication at 9600 bits per second:
  Serial1.begin(9600);            // initialize UART with baud rate of 9600
}
void loop() {
  int value = Serial.read();
  if (value >= 2){
    Serial1.println(value);
    for(int i=0; i<=value; i++){
      Serial1.println(value);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}
//  float number = strtof(value,NULL));
//  for(int i=0; i<value; i++){
//
//
//
//    
//  }
//  
//  if (value == '1'){
//    Serial1.println('1');
//    digitalWrite(LED_BUILTIN, HIGH);
//    Serial.println("LEDS ON");
//  }
//  else if (Serial.read() == '0'){
//    Serial1.println('0');
//    digitalWrite(LED_BUILTIN, LOW);
//    Serial.print("LEDS OFF");
//  }




//
//
//void setup() {
//  // Open serial communications and wait for port to open:
//  Serial.begin(115200);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for Native USB only
//  }
//  Serial.println("Goodnight moon!");
//
//  // set the data rate for the SoftwareSerial port
//  mySerial.begin(38400);
//  mySerial.println("Hello, world?");
//  
//
//}
//
//void loop() {
//  if (mySerial.available())
//    Serial.write(mySerial.read());
//  if (Serial.available())
//    mySerial.write(Serial.read());
//
//
//  // if there's any serial available, read it:
//  while (Serial.available() > 0) {
//
//    // look for the next valid integer in the incoming serial stream:
//    int red = Serial.parseInt();
//    // do it again:
//    int green = Serial.parseInt();
//    // do it again:
//    int blue = Serial.parseInt();
//
//    // look for the newline. That's the end of your sentence:
//    if (Serial.read() == '\n') {
//      // constrain the values to 0 - 255 and invert
//      // if you're using a common-cathode LED, just use "constrain(color, 0, 255);"
//      red = 255 - constrain(red, 0, 255);
//      green = 255 - constrain(green, 0, 255);
//      blue = 255 - constrain(blue, 0, 255);
//
//      // fade the red, green, and blue legs of the LED:
//      // print the three numbers in one string as hexadecimal:
//      Serial.print(red, HEX);
//      Serial.print(green, HEX);
//      Serial.println(blue, HEX);
//    }
//  }
//}
