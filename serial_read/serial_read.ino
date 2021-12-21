
//Receiver:
void setup() {
  delay(1500);
  
  pinMode(LED_BUILTIN, OUTPUT);      // set LED pin as output
  digitalWrite(LED_BUILTIN, LOW);    // switch off LED pin

  Serial.println("Set pin");

  Serial.begin(9600);               // initialize serial communication at 9600 bits per second:
  Serial1.begin(9600);            // initialize UART with baud rate of 9600

  Serial.println("Setup end");
}

void loop() {
  Serial.println("Start");
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
    
    Serial.print("Result: ");
    Serial.println(value);
    
  }
  delay(500);
  Serial.println("Next Step");
}
