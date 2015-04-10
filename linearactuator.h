#ifndef LINEAR_ACTUATOR_H
#define LINEAR_ACTUATOR_H

#include <Arduino.h>

enum L_Movements {
  L_STOP = 0,
  L_FORWARD = 1,
  L_BACKWARD = 2
};

struct LinearActuator {
  int pinA;
  int pinB;
  L_Movements dir;
  bool reverse;
};

void l_attach(LinearActuator & l, int pinA, int pinB, bool reverse = false) {
  l.pinA = pinA;
  l.pinB = pinB;
  
  l.reverse = reverse;
  
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
}

void l_move(LinearActuator & l) {
  if  (l.dir == L_FORWARD) {
    digitalWrite(l.pinA, HIGH);
    digitalWrite(l.pinB, LOW);
  } else if (l.dir == L_BACKWARD) {
    digitalWrite(l.pinA, LOW);
    digitalWrite(l.pinB, HIGH);
  } else {
    digitalWrite(l.pinA, LOW);
    digitalWrite(l.pinB, LOW);
  }
}

void l_dir(LinearActuator & l, L_Movements dir) {
  l.dir = dir;
  
  if (l.reverse) {
    if (l.dir == L_FORWARD) l.dir = L_BACKWARD;
    else if (l.dir == L_BACKWARD) l.dir = L_FORWARD;
  }
}

#endif
