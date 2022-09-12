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
}