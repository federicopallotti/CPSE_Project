#include <Adafruit_VL53L0X.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

int RANGE = 20;

void setup() {
  Serial.begin(115200);

  Serial.begin(9600);               // initialize serial communication at 9600 bits per second:
  Serial1.begin(9600);            // initialize UART with baud rate of 9600
  delay(1500);

  Serial.println("test");
  // wait until serial port opens for native USB devices
  while (! Serial1) {
    delay(1);
  }
  Serial.println("test 2");

  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin(0x29, true)) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
  
}


void loop() {
  while (Serial1.available() >= 0) {
    int value = Serial1.read();
    // Wait for the start symbol "$"
    while(value != 1){
      value = Serial1.parseInt();
      Serial.print("\t Waiting: ");
      Serial.println(value);
    }

    VL53L0X_RangingMeasurementData_t measure;
      
    Serial.print("Reading a measurement... ");
    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

    // 
    float tepm_sum = 0;
    int counter = 0;
    while (counter<RANGE){
      if (measure.RangeStatus != 4){
        tepm_sum += measure.RangeMilliMeter;
        counter++;
      }
      delay(20);
    }
    Serial1.println((int)tepm_sum/RANGE);
    Serial.println((int)tepm_sum/RANGE);
    
  }
  Serial.println("No serial available");

}
