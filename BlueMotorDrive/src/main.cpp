#include <Arduino.h>
#include <Romi32U4.h>
#include "BlueMotor.h"
BlueMotor motor;
Romi32U4ButtonB buttonB;

void setup()
{
  Serial.begin(9600);
  //motor.setup();
  //motor.reset();
  //delay(3000);
  // Serial.print("Time (ms)");
  // Serial.print("   ");
  // Serial.print("Position");
  // Serial.print("    ");
  // Serial.println("speedInRPM");
  // delay(3000);
}

long target = 100;
void loop()
{
  // timeToPrint = millis() + sampleTime;
  // oldPosition = motor.getPosition();
  // while (buttonB.isPressed())
  // {
  //   // The button is currently pressed.
  //   motor.setEffort(motorEffort);
  //   if ((now = millis()) > timeToPrint)
  //   {
  //     timeToPrint = now + sampleTime;
  //     newPosition = motor.getPosition();
  //     speedInRPM = (newPosition - oldPosition) * 600 / 270;
  //     Serial.print(now);
  //     Serial.print("          ");
  //     Serial.print(newPosition);
  //     Serial.print("          ");
  //     Serial.println(speedInRPM);
  //     oldPosition = newPosition;
      
  //   }
    
  // }
  motor.moveTo(540);
}