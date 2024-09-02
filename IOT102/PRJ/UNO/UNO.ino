#include <SoftwareSerial.h>  // Include SoftwareSerial Library

SoftwareSerial s(A0, A1);  // Initialize SoftwareSerial on pins A0 (RX) and A1 (TX)
char floatStr[50];         // Character array to store the converted float

void setup() {
  // Initialize hardware serial communication at 9600 baud rate
  Serial.begin(9600);
  // Initialize software serial communication at 115200 baud rate
  s.begin(115200);
}

char waterLevelStr[50];
char weightLevelStr[50];
char message[100];

void loop() {
  dtostrf((float)25.555, 10, 9, waterLevelStr);    // (float value, min width, precision, buffer)
  dtostrf((float)33.3333, 10, 9, weightLevelStr);  // (float value, min width, precision, buffer)
  // Send data to ESP
  message[0] = '\0';
  strcat(message, waterLevelStr);
  strcat(message, "|");
  strcat(message, weightLevelStr);
  s.write(message);
  // Delay for 1 second
  delay(10000);
}
