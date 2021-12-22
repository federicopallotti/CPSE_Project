#include <Adafruit_VL53L0X.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
int number_of_measures = 20;

void setup() {
  Serial.begin(115200);
  // initialize serial communication at 9600 bits per second  
  Serial.begin(9600); 
  // initialize UART with baud rate of 9600 with Portenta
  Serial1.begin(9600);            
  // wait until serial port opens for native USB devices
  while (! Serial1) {
    delay(1);
  }

  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin(0x29, true)) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}

void loop() {
  while (Serial1.available() >= 0) {
    int value = Serial1.read();
    // Wait for the start symbol "1"
    while(value != 1){
      value = Serial1.parseInt();
      Serial.print("\t Waiting: ");
      Serial.println(value);
    }

    VL53L0X_RangingMeasurementData_t measure;
      
    Serial.print("Reading a measurement... ");
    lox.rangingTest(&measure, false);

    float readings_sum = 0;
    int counter = 0;
    while (counter<number_of_measures){
      if (measure.RangeStatus != 4){
        readings_sum += measure.RangeMilliMeter;
        counter++;
      }
      // Delay between readings
      delay(20);
    }
    // Send the average result as response
    Serial1.println((int)readings_sum/number_of_measures);
    Serial.println((int)readings_sum/number_of_measures);
  }
  Serial.println("No serial available");
}
