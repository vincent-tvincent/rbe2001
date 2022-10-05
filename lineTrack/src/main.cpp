#include <Romi32U4.h>
#include <LineTrack.h>
#include <sonarApproach.h>
Chassis chassis;
LineTrack Track(700,1,2);
Rangefinder rangefinder(sonarEcho,sonarTrig);
sonarApproach sonar(2.5,1.5);
Servo32U4 servo;
int countToMs(int count){
  return count * servoMsPerCount;

}
void gripperRelease(){
  servo.writeMicroseconds(countToMs(gripperStartEncoderCount));
}

void gripperClose(){
  servo.writeMicroseconds(countToMs(gripperEndEncoderCount));
  gripperRelease();
  if(analogRead(ServoEncoder) < gripperEndEncoderCount){
      gripperRelease();
  }
}
void setup() {
  Serial.begin(9600);
  servo.setMinMaxMicroseconds(servoMin,servoMax);
  chassis.init();
  rangefinder.init();
  delay(50);
}
bool onCross = false;
bool start = true;
float speed = 20;
float turnSpeed = 180;
float targetDistanceFromWall = 12.7;
float errorRange = 0.0;
bool doTask1 = false;
void loop() {
  if(doTask1){
    Track.switchTrack(turnSpeed); // because there is no interct at the start point, switch track will make the romi bot turn back
    // start approaching the target wall 
    float distanceFromWall = sonar.getDistance();
    while(distanceFromWall > targetDistanceFromWall){ // once detect the cross, switch to the perpendicular track
      if(Track.isCross()){
        Track.trackFor(speed, carRadius); // move the romi bot to the center of cross 
        Track.switchTrack(turnSpeed); // switch to the target track face left 
      }
      //if not on the corss, keep tracking till approaching the fist object face to the robot with certain distance 
      Track.track(sonar.getApproachingSpeed(targetDistanceFromWall,speed));
      distanceFromWall = sonar.getDistance();
    }
    while(true);
    //Track.track(sonar.getApproachingSpeed(targetDistanceFromWall,speed));
  }
  gripperRelease();
  gripperClose();
}