#include <Wire.h>

static const uint8_t D0 = 16;
static const uint8_t D1 = 5;
static const uint8_t D2 = 4;
static const uint8_t D3 = 0;
static const uint8_t D4 = 2;
static const uint8_t D5 = 14;
static const uint8_t D6 = 12;
static const uint8_t D7 = 13;
static const uint8_t D8 = 15;
static const uint8_t D9 = 3;
static const uint8_t D10 = 1;

#define RX_PIN 4  // Chân RX của EspSoftwareSerial
#define TX_PIN 5  // Chân TX của EspSoftwareSerial

/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL677_NJByb"
#define BLYNK_TEMPLATE_NAME "Pet feeding"
#define BLYNK_AUTH_TOKEN "duExzsTvP--xXyW2F__AQGj_8GALG3iN"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "FPTU_Student";
char pass[] = "12345678";

BlynkTimer timer;

String waterLevelStr;
String weightLevelStr;
String message;

// SoftwareSerial s(D9, D10);  // Initialize SoftwareSerial on pins D9 (RX) and D10 (TX)

void myTimerEvent() {
  message = "";
  Wire.requestFrom(8, 20); /* request & read data of size 13 from slave */
  while (Wire.available()) {
    char c = Wire.read();
    message += c;
  }

  Serial.println(message);
  int delimiterIndex = message.indexOf('|');
  if (delimiterIndex != -1) {
    waterLevelStr = message.substring(0, delimiterIndex);
    weightLevelStr = message.substring(delimiterIndex + 1);
  } else {
    waterLevelStr = "";
    weightLevelStr = "";
  }
  // delay(500);
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V0, waterLevelStr);
  Blynk.virtualWrite(V1, weightLevelStr);
}

void setup() {
  // Debug console
  Serial.begin(115200);
  // s.begin(115200);
  Wire.begin(D1, D2); /* join i2c bus with SDA=D1 and SCL=D2 of NodeMCU */

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Setup a function to be called every second
  timer.setInterval(5000L, myTimerEvent);
}

void loop() {
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
  // message = "";
  // Wire.requestFrom(8, 20); /* request & read data of size 13 from slave */
  // while (Wire.available()) {
  //   char c = Wire.read();
  //   message += c;
  // }

  // Serial.println(message);
  // int delimiterIndex = message.indexOf('|');
  // if (delimiterIndex != -1) {
  //   waterLevelStr = message.substring(0, delimiterIndex);
  //   weightLevelStr = message.substring(delimiterIndex + 1);
  // } else {
  //   waterLevelStr = "";
  //   weightLevelStr = "";
  // }
  // // delay(500);
}