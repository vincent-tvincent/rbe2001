
#include<Romi32U4.h>
#include<LineTrack.h>
#include<gripper.h>
#include<BlueMotor.h>
#include<IRdecoder.h>
#include<ir_codes.h>
#include<sonarApproach.h>
#include<RemoteConstants.h>
//Servo32U4 servo;
LineTrack Track(700,2,2); 
Chassis drive;
BlueMotor motor;
gripper Gripper(gripperStartEncoderCount,gripperEndEncoderCount);
IRDecoder decoder(6);
sonarApproach sonar(2,0.5);
void setup() {
  delay(2000);
  Serial.println("setup");
  drive.init();
  Gripper.init();
  Track.init();
  motor.setup();
  motor.reset();
  decoder.init();
  sonar.init();
  motor.toStartPosition(motorEffort);
  motor.reset();
}
float pickDistance = 2.2;
float stopDistance = 12.7;
float speed = 20;
float turnSpeed = 90;
float operationSpeed = 5;
float plateLength = 10;

int end = 0;
int roofToStart = 1;
int startToRoof = 2;
bool runTask = true;
void loop() {
  if(runTask){
    int next = roofToStart;
    while(next != end){
      if(next == startToRoof){
        Track.trackFor(speed,1);
        Track.switchTrack(turnSpeed);
        Track.track(speed);
        while(sonar.getDistance() > stopDistance){
        if(Track.isCross()){
          Track.stop();
          Track.trackFor(speed,carRadius);
          Track.stop();
          Track.switchTrack(turnSpeed);
          Track.track(speed);
        }
        Track.track(sonar.getApproachingSpeed(stopDistance,speed));
        }
        Track.stop();
        next = roofToStart;
      }else if(next == roofToStart){
        delay(5000);
        Serial.println("roof to start");
        Track.trackFor(speed,1);
        Track.switchTrack(-turnSpeed);
        
        Track.track(speed);
        while(true);
        while(sonar.getDistance() > stopDistance){
        if(Track.isCross()){
          Track.stop();
          Track.trackFor(speed,carRadius);
          Track.stop();
          Track.switchTrack(-turnSpeed);
          Track.track(speed);
        }
        Track.track(sonar.getApproachingSpeed(stopDistance,speed));
        }
        Track.stop();
        while(true);
        next = startToRoof;
      }
    }
  }
  //Track.switchTrack(-turnSpeed);
  // while(sonar.getDistance() > pickDistance){
  //   Track.track(sonar.getApproachingSpeed(pickDistance,speed));
  // }
  // Track.stop();
  // Gripper.closeTo(1);
  // while(sonar.getDistance() < 2.5 + pickDistance){
  //   Gripper.hold();
  //   Track.track(-operationSpeed);
  // }
  // Track.stop();
  // delay(1000);
  // Gripper.closeTo(0.4);
  // delay(1000);
  // Track.trackFor(speed,2.5);
  // Gripper.closeTo(1);
  // bool keepMove = true;
  // while(keepMove){
  //   keepMove = motor.move(carryEncoderRead);
  //   Gripper.hold();
  // }
  // delay(1000);
  // while(sonar.getDistance() <= pickDistance){
  //     Track.track(-speed);
  // }
  // Track.stop();
  // delay(1000);
  // Track.switchTrack(turnSpeed);
  // Track.trackFor(speed,5);
  // while(true);
};