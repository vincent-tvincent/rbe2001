
#include<Romi32U4.h>
#include<LineTrack.h>
#include<gripper.h>
#include<BlueMotor.h>
#include<IRdecoder.h>
#include<ir_codes.h>
#include<sonarApproach.h>
#include<RemoteConstants.h>
//Servo32U4 servo;
LineTrack Track(700,2,1.5); 
Chassis drive;
BlueMotor motor;
gripper Gripper(gripperStartEncoderCount,gripperEndEncoderCount);
IRDecoder decoder(6);
sonarApproach sonar(2,0.5);
Romi32U4ButtonA start;
void setup() {
  start.waitForButton();
  delay(1000);
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
  motor.moveTo(4000);
}
float pickDistance = 8.7;
float stopDistance = 12.7;
float speed = 20;
float turnSpeed = 90;
float operationSpeed = 10;
float plateLength = 10;

int end = 0;
int roofToStart = 1;
int startToRoof = 2;
int lift45 = 3;
bool runTask = false;
void loop(){
  motor.moveTo(2000);

  if(runTask){
    int next = lift45;
    while(next != end){
      if(next == startToRoof){
        Serial.println("startToRoof");
        Track.trackFor(speed,1);
        Track.switchTrack(turnSpeed);
        Track.trackFor(speed,1);
        bool move = true;
        bool sonarActivate = false;
        while(move){
          if(Track.isCross()){
            Track.stop();
            Track.trackFor(speed,carRadius);
            Track.stop();
            Track.switchTrack(turnSpeed);
            Track.trackFor(speed,1);
            sonarActivate = true;
          }
          if(sonarActivate){
            move = !sonar.haveDistance(stopDistance);
            Track.track(sonar.getApproachingSpeed(stopDistance,speed));
          }else{
            Track.track(speed);
          }
          Serial.print("move: ");
          Serial.println(move);
        }
        Track.stop();
        next = lift45;
      }else if(next == roofToStart){
        Serial.println("roof to start");
        int startDriveCount = chassis.getLeftEncoderCount() - chassis.getRightEncoderCount() / 2;
        Track.trackFor(speed,1);
        Track.switchTrack(-turnSpeed);
        int endDriveCount = chassis.getLeftEncoderCount() - chassis.getRightEncoderCount() / 2;
        Track.trackFor(speed,1);
        bool sonarActivate = false;
        bool move = true;
        while(move){
          if(Track.isCross()){
            Track.stop();
            Track.trackFor(speed,carRadius);
            Track.stop();
            Track.switchTrack(-turnSpeed);
            Track.trackFor(speed,1);
            sonarActivate = true;
          }
          if(sonarActivate){
            move = !sonar.haveDistance(stopDistance);
            Track.track(sonar.getApproachingSpeed(stopDistance,speed));
          }else{
            Track.track(speed);
          }
          Serial.print("move: ");
          Serial.println(move);
        }
        Track.stop();
        next = lift45;
      }if(next == lift45){
        motor.moveTo(lift45degEncoderRead);
        Gripper.release();
        //int prevDriveEncoderCount = (chassis.getLeftEncoderCount() + chassis.getRightEncoderCount())/2;
        Track.trackFor(10,2);
        while(!sonar.haveDistance(pickDistance)){
          Track.track(sonar.getApproachingSpeed(pickDistance,speed));
          //prevDriveEncoderCount = (chassis.getLeftEncoderCount() + chassis.getRightEncoderCount())/2;
        }
        Track.stop();
        for(float percent = 0; percent < 1; percent += 0.05){
          Gripper.closeTo(percent);
          delay(50);
        }
        Gripper.close();
        motor.moveTo(lift45degEncoderRead + 2500);
        bool keepLift = true;
        while(keepLift){
          Track.track(-operationSpeed);
          Gripper.hold();
          keepLift = motor.move(carryEncoderRead);
        }
        Track.stop();
        while(true);
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