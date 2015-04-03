#include <Servo.h>
#include "linearactuator.h"

#define PIN 7

//Input Parameters
int armRate = 0;
LinearActuator l_scoop;
LinearActuator l_bin;

void setup() {
  Serial.begin(9600);
  
  l_attach(l_scoop, 7);
}

void loop() {
  ReadSerial();
  
  Serial.print("Rate: ");
  Serial.print(armRate);
  
  Serial.print(", Scoop: ");
  Serial.print(l_scoop.dir);
  
  l_move(l_scoop);
  
  Serial.print(", Bin: ");
  Serial.println(l_bin.dir);
}

//Serial Communication

const int INPUT_LENGTH = 15;
char serialInput[INPUT_LENGTH];

const int NUMBER_LENGTH = 9;
char numberInput[NUMBER_LENGTH];

/*
Commands:
a:<rate> --- Scoop Arm Rate
s:<0|1|2> --- Scoop Linear Actuators
b:<0|1|2> --- Collection Bin Linear Actuators
*/

void ClearSerial() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') break;
  }
}

void ReadSerial() {
  if (Serial.available() == 0) return;
  
  char *ptr = serialInput;
  *ptr = Serial.read();
  
  //Reads all serial values into a char array
  while (*ptr != '\r' && *ptr != '\n') {
    ++ptr;
    
    while (!Serial.available());
    *ptr = Serial.read();
  }
  
  ptr = serialInput;
  char command = *ptr;
  
  if (command == 'a') {
    ++ptr; // Ignore first char
    ++ptr; // Ignore first colon
    
    //Creates a pointer to work with numberInput Array
    char *numPtr = numberInput;
    
    //Puts all valid characters from input into numberInput
    while (*ptr != '\n' && *ptr != '\r' && numPtr - numberInput < NUMBER_LENGTH) {
      *numPtr = *ptr;
      ++ptr; ++numPtr;
    }
    
    //Defense against broken values
    if (numPtr - numberInput >= NUMBER_LENGTH) ClearSerial();
    
    //Sets the current (next element past numbers) to null character
    *numPtr = '\0';
    
    //Turns characters into an int for the arm rate
    armRate = atoi(numberInput);
  } else if (command == 'b' || command == 's') {
    ++ptr;
    ++ptr;
    
    char *numPtr = numberInput;
    *numPtr = *ptr;
    *(++numPtr) = '\0';
    
    L_Movements dir = static_cast<L_Movements>(atoi(numberInput));
    
    if (command == 'b')
      l_dir(l_scoop, dir);
    else if (command == 's')
      l_dir(l_bin, dir);
  }
  
  if (Serial.available()) ReadSerial();
}
