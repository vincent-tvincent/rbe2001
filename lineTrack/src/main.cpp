#include <Romi32U4.h>
#include <LineTrack.h>
Chassis chassis;
LineTrack* Track;
void setup() {
  Serial.begin(9600);
  chassis.init();
  Track = new LineTrack(750,0.5,0,0.3);
}
bool onCross = false;
void loop() {
    onCross = Track->isCross();
    if(onCross){
      chassis.setWheelSpeeds(0,0);
      chassis.driveFor(7.62,10,true);
      Serial.print(!Track->onTrack());
      do{
        chassis.setTwist(0,30);
      }while(!Track->onTrack());

      chassis.setTwist(0,0);
    }else{
      Track->track(10);
    }
    // delay(500);
    // chassis.driveFor(7.62,10,true);
    // delay(1000);
    // chassis.turnFor(40,90,true);
    // delay(1000);
    // while(!Track->onTrack()){chassis.setTwist(0,30);}
    // chassis.setTwist(0,0);
    // while(true){}
}