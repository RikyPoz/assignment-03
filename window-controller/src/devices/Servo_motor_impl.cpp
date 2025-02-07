#include "Servo_motor_impl.h"
#include <Arduino.h>

const float COEFF = (2250.0 - 750.0) / 180; // 750 -> 0, 2250 -> 180

ServoMotorImpl::ServoMotorImpl(int pin)
{
  this->pin = pin;
}

void ServoMotorImpl::on()
{
  motor.attach(pin);
}

void ServoMotorImpl::setPosition(int angle)
{
  motor.write(750 + angle * COEFF);
}

void ServoMotorImpl::off()
{
  motor.detach();
}

int ServoMotorImpl::getPosition()
{
  return (motor.read() - 750) / COEFF;
}
