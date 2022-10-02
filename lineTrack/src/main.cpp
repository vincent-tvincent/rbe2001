#include <Romi32U4.h>
#include <LineTrack.h>
#include <sonarApproach.h>
Chassis chassis;
LineTrack Track(700,1,2);
Rangefinder rangefinder(sonarEcho,sonarTrig);
sonarApproach sonar(2.5,1.5);
void setup() {
  Serial.begin(9600);
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
void loop() {
  Track.switchTrack(turnSpeed); // turing back;
  float distanceFromWall = sonar.getDistance();
  while(distanceFromWall > targetDistanceFromWall){
    if(Track.isCross()){
      Track.trackFor(speed, carRadius);
      Track.switchTrack(turnSpeed);
    }
    Track.track(sonar.getApproachingSpeed(targetDistanceFromWall,speed));
    distanceFromWall = sonar.getDistance();
  }
  while(true);
  //Track.track(sonar.getApproachingSpeed(targetDistanceFromWall,speed));
}