#include <Romi32U4.h>
#include <LineTrack.h>
Chassis chassis;
LineTrack* Track;
void setup() {
  Serial.begin(9600);
  chassis.init();
  Track = new LineTrack(500,1.5,0,2);
}
bool atCross = false;
void loop() {
  atCross = Track->isCross();
  if(atCross){
    chassis.setWheelSpeeds(0,0);
    chassis.driveFor(7.62,10);
    
  }else{
    Track->track(10);
  }
}