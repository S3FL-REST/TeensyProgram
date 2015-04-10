#include <Servo.h>
#include "linearactuator.h"

#define BIN_1A 1
#define BIN_1B 2
#define BIN_2A 3
#define BIN_2B 4

#define EXC_1A 5
#define EXC_1B 6
#define EXC_2A 7
#define EXC_2B 8

#define SCOOP_A 9
#define SCOOP_B 10

//Input Parameters
int armRate = 0;
LinearActuator l_exc_1;
LinearActuator l_exc_2;
LinearActuator l_bin_1;
LinearActuator l_bin_2;

void setup() {
  Serial.begin(9600);
  
  l_attach(l_bin_1, BIN_1A, BIN_1B, false);
  l_attach(l_bin_2, BIN_2A, BIN_2B, true);
  
  l_attach(l_exc_1, EXC_1A, EXC_1B, false);
  l_attach(l_exc_2, EXC_2A, EXC_2B, true);
}

void loop() {
  ReadSerial();
  
  Serial.print("Rate: ");
  Serial.print(armRate);
  
  Serial.print(", Scoop: ");
  Serial.print(l_exc_1.dir);
  
  l_move(l_exc_1);
  l_move(l_exc_2);
  
  Serial.print(", Bin: ");
  Serial.println(l_bin_1.dir);
  
  l_move(l_bin_1);
  l_move(l_bin_2);
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
  while (Serial.available()) Serial.read();
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
    
    if (command == 'b') {
      l_dir(l_exc_1, dir);
      l_dir(l_exc_2, dir);
    } else if (command == 's') {
      l_dir(l_bin_1, dir);
      l_dir(l_bin_2, dir);
    }
  }
  
  if (Serial.available()) ReadSerial();
  ClearSerial();
}
