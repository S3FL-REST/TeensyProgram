#ifndef LINEAR_ACTUATOR_H
#define LINEAR_ACTUATOR_H

#include <Servo.h>

enum L_Movements {
  L_STOP = 0,
  L_FORWARD = 1,
  L_BACKWARD = 2
};

struct LinearActuator {
  Servo actuator;
  L_Movements dir;
};

void l_attach(LinearActuator & l, int pin) {
  l.actuator.attach(pin);
}

void l_move(LinearActuator & l) {
  if  (l.dir == L_FORWARD)
    l.actuator.write(180);
  else if (l.dir == L_BACKWARD)
    l.actuator.write(0);
  else
    l.actuator.write(90);
}

void l_dir(LinearActuator & l, L_Movements dir) {
  l.dir = dir;
}

#endif
