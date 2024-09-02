#include <Arduino.h>
#include "HX711.h"
#include <Servo.h>
#include <Wire.h>

// Set up variable for food handle
// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = A0;
const int LOADCELL_SCK_PIN = A1;

HX711 scale;
// Servo
Servo foodDoor;  // create servo object to control a servo
const int foodDoorPin = 7;
const int weightLowThreshold = 100;   // 100g
const int weightHighThreshold = 300;  // 300g


// Set up variable for water handle
const int waterSensorPin = A2;      // Chân cảm biến nước
const int relayPin = 8;             // Chân relay
const int waterLowThreshold = 280;  // Ngưỡng nước
const int waterHighThreshold = 500;

char waterLevelStr[50];
char weightLevelStr[50];
char message[100];

void setup() {
  Wire.begin(8);                /* join i2c bus with address 8 */
  Wire.onRequest(requestEvent); /* register request event */
  // esp.begin(115200);  // Init

  // Set up water sensor and relay for water pump
  pinMode(waterSensorPin, INPUT);  // Thiết lập chân cảm biến nước là đầu vào
  pinMode(relayPin, OUTPUT);       // Thiết lập chân relay là đầu ra
  digitalWrite(relayPin, LOW);     // Tắt relay ban đầu
  Serial.begin(9600);              // Khởi tạo Serial để in ra giá trị

  // Set up servo
  foodDoor.attach(foodDoorPin);

  // Init scale
  Serial.println("Initializing the scale");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());  // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));  // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));  // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
                                          // by the SCALE parameter (not set yet)

  scale.set_scale(-533.6609523810);
  //scale.set_scale(-471.497);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();  // reset the scale to 0

  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
  Serial.println(scale.read());  // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));  // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));  // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight, divided
                                          // by the SCALE parameter set with set_scale
}

void openFoodDoor() {
  foodDoor.write(100);  // Rotate to 0 degree
}

void closeFoodDoor() {
  foodDoor.write(0);  // Rotate to 100 degree
}

void loop() {
  int waterLevel = analogRead(waterSensorPin);  // Đọc giá trị cảm biến nước
  float foodWeight = scale.get_units();         // Read the weight value
  Serial.print("Water Level: ");
  Serial.println(waterLevel);  // In giá trị cảm biến nước ra Serial Monitor
  Serial.print("Food weight (g): ");
  Serial.println(foodWeight);  // In giá trị cảm biến nước ra Serial Monitor

  // Pump water
  if (waterLevel < waterLowThreshold) {
    digitalWrite(relayPin, LOW);  // Bật relay để bơm nước
  } else if (waterLevel > waterHighThreshold) {
    digitalWrite(relayPin, HIGH);  // Tắt relay
  }

  // Feed the food. Rotate servo
  if (foodWeight < weightLowThreshold) {
    openFoodDoor();
  } else if (foodWeight > weightHighThreshold) {
    closeFoodDoor();
  }

  dtostrf((float)waterLevel, 8, 7, waterLevelStr);   // (float value, min width, precision, buffer)
  dtostrf((float)foodWeight, 10, 9, weightLevelStr);  // (float value, min width, precision, buffer)
  // Send data to ESP
  message[0] = '\0';
  strcat(message, waterLevelStr);
  strcat(message, "|");
  strcat(message, weightLevelStr);
  // esp.print(message);
  Wire.write(message); /*send string on request */
  

  delay(1000);  // Chờ 1 giây trước khi đọc lại
}

// function that executes whenever data is requested from master
void requestEvent() {
  Wire.write(message);  /*send string on request */ 
}