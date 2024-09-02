#include <Keypad.h>
#include <math.h>

const int bit_1 = 13;
const int bit_2 = 12;
const int bit_3 = 11;
const int bit_4 = 10;

const int SCREEN_SiZE = 4;
int SCREEN[] = { bit_1, bit_2, bit_3, bit_4 };

int ERROR[] = { HIGH, HIGH, HIGH, HIGH };
int INIT[] = { HIGH, LOW, LOW, LOW };
int OFF[] = { LOW, LOW, LOW, LOW };

int firstOperand = -1;
int secondOperand = -1;
int res = 0;
bool calc = false;
char operation = '\0';

const int numBits = 4;
int binaryArray[numBits];

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 9, 8, 7, 6 };
byte colPins[COLS] = { 5, 4, 3, 2 };

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < SCREEN_SiZE; i++) {
    pinMode(SCREEN[i], OUTPUT);
  }
}


void write_screen(int output[]) {
  for (int i = 0; i < SCREEN_SiZE; i++) {
    digitalWrite(SCREEN[i], output[i]);
  }
}

void intToBinaryArray(int n) {
  for (int i = numBits - 1; i >= 0; i--) {
    binaryArray[numBits - 1 - i] = (n >> i) & 1;
  }
}


void initialize() {
  if (res == 0) {
    write_screen(INIT);
    delay(100);
    write_screen(OFF);
    delay(100);
  } else if (res > 15) {
    write_screen(ERROR);
    delay(100);
    write_screen(OFF);
    delay(100);
  } else {
    intToBinaryArray(res);
    write_screen(binaryArray);
  }
}

void reset_calc() {
  res = 0;
  firstOperand = -1;
  secondOperand = -1;
}

void performCalculation() {
  switch (operation) {
    case 'A':
      res = firstOperand + secondOperand;
      break;
    case 'B':
      res = firstOperand - secondOperand;
      break;
    case 'C':
      res = firstOperand * secondOperand;
      break;
  }

  if (res >= 0 and res < 15) {
    Serial.println(res, BIN);
  }
}

void loop() {
  char pressedKey = customKeypad.getKey();

  if (pressedKey) {
    Serial.println(pressedKey);
    if (pressedKey >= '0' && pressedKey <= '9') {
      if (firstOperand == -1) {
        firstOperand = pressedKey - '0';
      } else {
        secondOperand = pressedKey - '0';
      }
    } else if (pressedKey == 'A' || pressedKey == 'B' || pressedKey == 'C') {
      operation = pressedKey;
    } else if (pressedKey == '*') {
      reset_calc();
    } else if (pressedKey == '#') {
      if (firstOperand != -1 && secondOperand != -1 && operation != '\0') {
        performCalculation();
      }
    }
  }

  initialize();
}
