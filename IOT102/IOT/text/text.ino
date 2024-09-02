#include "font.h"

// 2-dimensional array of row pin numbers:
int R[] = {13, 8, A2, 10, 2, A3, A0, A5};  
// 2-dimensional array of column pin numbers:
int C[] = {9, A1, 5, 12, A4, 11, 7, 6};    

char word_to_display[] = "Thuy <3";

void setup()  
{  
   // iterate over the pins:
  for(int i = 0;i<8;i++)  
  // initialize the output pins:
  {  
    pinMode(R[i],OUTPUT);  
    pinMode(C[i],OUTPUT);  
  }  
  Serial.begin(9600); 
  randomSeed(analogRead(0));
}  
  
void loop()  
{  
  print();
}  

void print() {
  int arrayLength = sizeof(word_to_display) / sizeof(word_to_display[0]);
  int current = 0;
  unsigned char *bitmap = font8x8_basic[0];
  unsigned char *current_word = font8x8_basic[word_to_display[current]];
  unsigned short int col = 0;
  while (true){
    Display(bitmap);
    for (int r = 0; r < 8; r++) {
      unsigned char set = (current_word[r] & (1 << (col))) >> (col);
      // Serial.print(set, BIN);
      bitmap[r] = (bitmap[r] << 1) | set;
    }
    if (col == 7) {
      col = 0;
      current +=1;
    } else {
      col += 1;
    }
    // Serial.print("\n");
    if (current > arrayLength) {
      current = 0;
    }
    current_word = font8x8_basic[word_to_display[current]];
  }
}

void Display(char *bitmap)    
{
  for(int i = 0; i < 15; i++) {
    for(int c = 0; c<8;c++)  
    {  
      digitalWrite(C[c],LOW);//use thr column 
      //loop
      for(int r = 0;r<8;r++)  
      {  
        int set = ((bitmap[r] >> (7 - c)) & 0x01) ? 1 : 0;
        digitalWrite(R[r],set);  
      }  
      delay(1);  
      Clear(); 
    }
  }  
}
  
void Clear()                          //清空显示  
{  
  for(int i = 0;i<8;i++)  
  {  
    digitalWrite(R[i],LOW);  
    digitalWrite(C[i],HIGH);  
  }  
}  
