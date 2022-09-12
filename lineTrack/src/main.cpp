#include <Romi32U4.h>
#include <LineTrack.h>
//#include <Chassis.h>

Chassis chassis;
LineTrack* Track;
void setup() {
  Serial.begin(9600);
  chassis.init();
  Track = new LineTrack(880,0.01,0,0);
}


void loop() {
  Serial.print(analogRead(PIN_A0));
  Serial.print(" ");
  Serial.println(analogRead(PIN_A2));
  Serial.println(analogRead(PIN_A0) - analogRead(PIN_A2));
  delay(500);
}