#ifndef __SERVO_MOTOR_IMPL__
#define __SERVO_MOTOR_IMPL__

#include "Servo_motor.h"
#include <Arduino.h>
#include "ServoTimer2.h"

class ServoMotorImpl: public ServoMotor {

public:
  ServoMotorImpl(int pin);

  void on();
  void setPosition(int angle);
  void off();
  int getPosition();
    
private:
  int pin;
  ServoTimer2 motor;
};

#endif
