#include <Romi32U4.h>
#include <LineTrack.h>
Chassis chassis;
LineTrack* Track;
void setup() {
  Serial.begin(9600);
  chassis.init();
  Track = new LineTrack(750,1,1);
}
bool onCross = false;
bool start = true;
int speed = 35;
int turnSpeed = 180;
void loop() {
    if(start){Track->turnBack(); start = false;}
    while(!onCross){
      Track->track(speed);
      onCross = Track->isCross();
    }
    chassis.setWheelSpeeds(0,0);
    chassis.driveFor(carRadius,speed,true);
    chassis.turnFor(speed,turnSpeed,true);
    bool notOnTrack = !Track->onTrack();
    while(notOnTrack){
      chassis.setTwist(0,turnSpeed);
      notOnTrack = !Track->onTrack();
    }
    Track->trackFor(speed,18);
    while(true);
}