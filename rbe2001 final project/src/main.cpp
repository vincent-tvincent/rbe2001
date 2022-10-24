
#include<Romi32U4.h>
#include<LineTrack.h>
#include<gripper.h>
#include<BlueMotor.h>
#include<IRdecoder.h>
#include<ir_codes.h>
#include<sonarApproach.h>
#include<RemoteConstants.h>
//Servo32U4 servo;
LineTrack Track(700,1.5,2); 
Chassis drive;
BlueMotor motor;
gripper Gripper(gripperStartEncoderCount,gripperEndEncoderCount);
IRDecoder decoder(remotePin);
sonarApproach sonar(2,2);
Romi32U4ButtonA start;

bool setupLiftingSystem = false;
void setup() {
  start.waitForButton();
  delay(1000);
  Serial.println("setup");
  motor.setup();
  motor.reset();
  motor.toStartPosition(motorEffort);
  motor.reset();
  Gripper.init();
  Serial.println("running");
  drive.init();
  Track.init();
  decoder.init();
  sonar.init();
}
int UnloadEncoderRead = 1500;
int lift45degEncoderRead = 2700;
int lift25degEncoderRead = 3828;
int carryEncoderRead =5500;
float roofOperationDsitance = 15.5;
float platformOperationDsitance = 7;
float stopDistance = 20;
float speed = 20;
float turnSpeed = 90;
float operationSpeed = 10;
float plateLength = 10;


enum states{
  end,
  roofToStart45, 
  roofToStart25,
  startToRoof45,
  startToRoof25,
  lift45,
  lift25,
  liftPlatform,
  put45,
  put25,
  putPlatform
};
//moving to another part 
void move(bool isClockWise){
  int Tspeed = 0;
  if(isClockWise){
    Tspeed = -turnSpeed;
  }else{
    Tspeed = turnSpeed;
  }
  Gripper.hold();
  Track.switchTrack(-Tspeed);
  float approachingSpeed = sonar.getApproachingSpeed(stopDistance,speed);
  while(approachingSpeed){
    Gripper.hold();
    Track.track(approachingSpeed);
    if(Track.onCross()){
      Track.stop();
      delay(250);
      Track.trackFor(carRadius,speed);
      Track.switchTrack(Tspeed);
      Track.track(speed);
    }
    approachingSpeed = sonar.getApproachingSpeed(stopDistance,speed);
  }
  Track.stop();
}

//lift from roof
void lift(int degreeCount){
  //approaching
  motor.moveTo(degreeCount);
  Gripper.release();
  Track.trackFor(speed,1);
  float approachingSpeed = sonar.getApproachingSpeed(roofOperationDsitance,operationSpeed);
  while(approachingSpeed != 0){
      Track.track(approachingSpeed);
      approachingSpeed = sonar.getApproachingSpeed(roofOperationDsitance,operationSpeed);
  }
  Track.stop();
  //gripping
  for(float percent = 0; percent < 1; percent += 0.05){
    Gripper.closeTo(percent);
    delay(100);
  }
  // bool unclosed = !Gripper.tryClose();
  // while(unclosed){
  //   Track.trackFor(speed,-2.5);
  //   delay(200);
  //   unclosed = !Gripper.tryClose();
  // }
  approachingSpeed =sonar.getApproachingSpeed(roofOperationDsitance - 5,operationSpeed);
  bool move = true;
  while(move || approachingSpeed != 0){
    Gripper.close();
    Track.track(approachingSpeed);
    move = motor.move(degreeCount + 2000);
    approachingSpeed = sonar.getApproachingSpeed(roofOperationDsitance - 5,operationSpeed);
  }
  Track.stop();
  approachingSpeed = sonar.getApproachingSpeed(roofOperationDsitance + 4,operationSpeed);
  move = true;
  while(abs(approachingSpeed) > 0.1 || move){
    Gripper.hold();
    Track.track(approachingSpeed);
    move = motor.move(carryEncoderRead);
    approachingSpeed = sonar.getApproachingSpeed(roofOperationDsitance + 4,operationSpeed);
  }
  Gripper.close();
  Track.stop();
}

//load from platform 

void load(){
  start.waitForButton();
  //approaching
  motor.toStartPosition(motorEffort);
  Gripper.closeTo(0.4);
  float approachingSpeed = sonar.getApproachingSpeed(platformOperationDsitance,operationSpeed);
  while(approachingSpeed != 0){
      Track.track(approachingSpeed);
      approachingSpeed = sonar.getApproachingSpeed(platformOperationDsitance,operationSpeed);
  }
  Track.stop();
  //gripping
  //1. grib
  Gripper.release();

  for(float percent = 0; percent < 1; percent += 0.05){
    Gripper.closeTo(percent);
    delay(50);
  }
  Track.trackFor(operationSpeed,platformOperationDsitance - 1.5);
  Gripper.close();
  bool move = true;
  while(move){
    Gripper.hold();
    move = motor.move(carryEncoderRead);
    //Track.track(sonar.getApproachingSpeed(stopDistance,operationSpeed));
  }
  Track.stop();
}

//put on roof
void put(int degreeCount){
  motor.moveTo(carryEncoderRead);
  //approaching 
  bool move = true;
  while(move){
    Gripper.hold();
    move = motor.move(carryEncoderRead);
  }
  float approachingSpeed = sonar.getApproachingSpeed(roofOperationDsitance - 5,operationSpeed);
  while(abs(approachingSpeed) != 0){
      Gripper.hold();
      Track.track(approachingSpeed);
      approachingSpeed = sonar.getApproachingSpeed(roofOperationDsitance - 5,operationSpeed);
  }
  Track.stop();
  //placing 
  for(float percent = 1; percent > 0.75; percent -= 0.025){
    Gripper.closeTo(percent);
    delay(100);
  }
  // chassis.setWheelSpeeds(speed,-speed);
  // delay(100);
  // chassis.setWheelSpeeds(-speed,speed);
  // delay(200);
  // chassis.setWheelSpeeds(speed,-speed);
  // delay(100);
  // chassis.setWheelSpeeds(0,0);
  // move = true;
  // while(move){
  //   Gripper.hold();
  //   move = motor.move(degreeCount);
  // }
  Track.trackFor(operationSpeed,-5);
  for(float percent = 0.75; percent > 0.5; percent -= 0.025){
    Gripper.closeTo(percent);
    delay(100);
  }
  Track.trackFor(operationSpeed,-4);
  for(float percent = 0.5; percent > 0; percent -= 0.025){
    Gripper.closeTo(percent);
    delay(100);
  }
  Gripper.release();
  Track.stop();
}

// unload to platform 
void unload(){
  motor.moveTo(UnloadEncoderRead);
  float approachingSpeed = sonar.getApproachingSpeed(platformOperationDsitance - 1,operationSpeed); 
  while(approachingSpeed > 0.1){
      Gripper.hold();
      Track.track(approachingSpeed);
      approachingSpeed = sonar.getApproachingSpeed(platformOperationDsitance - 1,operationSpeed);
  }
  Track.stop();
  motor.toStartPosition(motorEffort);
  for(float percent = 1; percent > 0.25; percent -= 0.025){
    Gripper.closeTo(percent);
    delay(250);
  }
  Track.trackFor(operationSpeed,4);
  Track.trackFor(operationSpeed, -platformOperationDsitance/2);
  for(float percent = 0.25; percent > 0; percent -= 0.025){
    Gripper.closeTo(percent);
    delay(100);
  }
  Track.trackFor(operationSpeed,-stopDistance);
  Gripper.close();
  Track.stop();
}

// shift to another side 
void shift(){
}




bool runOnce =  true;
bool runTask = true;
bool cond1 = true;
void loop(){
    //Serial.println(sonar.getDistance());
    if(runTask){
    int next = lift25;
    while(next != end){
      if(next == startToRoof45 || next == roofToStart25){
        move(true);
        next = putPlatform;
      }else if(next == startToRoof25 || next == roofToStart45){
        move(false);
        next = put25;
      }else if(next == lift45){
        lift(lift45degEncoderRead);
        next = end;
      }else if(next == lift25){
        lift(lift25degEncoderRead);
        next = roofToStart25;
      }else if(next == liftPlatform){
        load();
        next = startToRoof25;
      }else if(next == put45){
        put(lift45degEncoderRead);
        next = end;
      }else if(next == put25){
        put(lift25degEncoderRead);
        next = end;
      }else if(next == putPlatform){
        unload();
        next = liftPlatform;
      }else{
        next = end;
      }     
    }
  }
  while(runOnce);
};