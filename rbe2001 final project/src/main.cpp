
#include<Romi32U4.h>
#include<LineTrack.h>
#include<gripper.h>
#include<BlueMotor.h>
#include<IRdecoder.h>
#include<ir_codes.h>
#include<sonarApproach.h>
#include<RemoteConstants.h>
//Servo32U4 servo;
LineTrack Track(700,1.5,1.5); 
Chassis drive;
BlueMotor motor;
gripper Gripper(gripperStartEncoderCount,gripperEndEncoderCount);
IRDecoder decoder(6);
sonarApproach sonar(4,1);
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
int UnloadEncoderRead = 500;
int lift45degEncoderRead = 2600;
int lift25degEncoderRead = 3200;
int carryEncoderRead = 5500;
float roofOperationDsitance = 11;
float platformOperationDsitance = 5;
float stopDistance = 12.7;
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
      delay(1000);
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
  float approachingSpeed = sonar.getApproachingSpeed(roofOperationDsitance,speed);
  while(approachingSpeed != 0){
      Track.track(approachingSpeed);
      approachingSpeed = sonar.getApproachingSpeed(roofOperationDsitance,speed);
  }
  Track.stop();
  //gripping
  for(float percent = 0; percent < 1; percent += 0.05){
    Gripper.closeTo(percent);
    delay(100);
  }
  Gripper.close();
  bool keepLift = true;
  motor.moveTo(degreeCount + 1000);
  while(keepLift){
    Track.track(-operationSpeed);
    Gripper.hold();
    keepLift = motor.move(carryEncoderRead);
  }
  Track.stop();
}

//load from platform 

void load(){
  //approaching
  motor.toStartPosition();
  Gripper.closeTo(0.5);
  float approachingSpeed = sonar.getApproachingSpeed(roofOperationDsitance,speed);
  while(approachingSpeed != 0){
      Track.track(approachingSpeed);
      approachingSpeed = sonar.getApproachingSpeed(roofOperationDsitance,speed);
  }
  //gripping
  //1. grib
  for(float percent = 0; percent < 1; percent += 0.05){
    Gripper.closeTo(percent);
    delay(50);
  }
  Gripper.close();
  //2. pull
  float pullDistance = roofOperationDsitance + 1;
  approachingSpeed = sonar.getApproachingSpeed(pullDistance, speed);
  while(approachingSpeed != 0){
      Gripper.hold();
      Track.track(approachingSpeed);
      approachingSpeed = sonar.getApproachingSpeed(pullDistance, speed);
  }
  //3. grip again
  //3a. release 
  for(float percent = 1; percent > 0.5; percent -= 0.05){
    Gripper.closeTo(percent);
    delay(100);
  }
  //3b. push forward and regrip 
  while(approachingSpeed != 0){
      Track.track(approachingSpeed);
      approachingSpeed = sonar.getApproachingSpeed(roofOperationDsitance,operationSpeed);
  }
  for(float percent = 0; percent < 1; percent += 0.05){
    Gripper.closeTo(percent);
    delay(50);
  }
  Gripper.close();
  bool move = true;
  while(move){
    Gripper.hold();
    move = motor.move(carryEncoderRead);
    Track.track(sonar.getApproachingSpeed(stopDistance,operationSpeed));
  }
  Track.stop();
}

//put on roof
void put(int degreeCount){
  motor.moveTo(carryEncoderRead);
  Track.trackFor(speed,1);
  float approachingSpeed = sonar.getApproachingSpeed(roofOperationDsitance,speed);
  while(approachingSpeed != 0){
      Gripper.hold();
      Track.track(approachingSpeed);
      approachingSpeed = sonar.getApproachingSpeed(roofOperationDsitance,speed);
  }
  for(float percent = 1; percent > 0; percent -= 0.05){
    Gripper.closeTo(percent);
    delay(100);
  }
  Track.trackFor(-operationSpeed,2);
  Track.stop();
}

// unload to platform 
void unload(){
  motor.moveTo(UnloadEncoderRead);
  float approachingSpeed = sonar.getApproachingSpeed(platformOperationDsitance,speed); 
  while(approachingSpeed != 0){
      Gripper.hold();
      Track.track(approachingSpeed);
      approachingSpeed = sonar.getApproachingSpeed(platformOperationDsitance,speed);
  }
  motor.toStartPosition();
  for(float percent = 0; percent < 0.55; percent += 0.05){
    Gripper.closeTo(percent);
    delay(100);
  }
  Track.trackFor(-speed,5);
  Gripper.close();
  Track.stop();
}

// shift to another side 
void shift(){
}






bool runTask = false;
void loop(){
    Track.track(speed);
    if(Track.onCross()){
      Track.stop();
      delay(1000);
      Track.trackFor(carRadius,speed);
      delay(1000);
      Track.switchTrack(turnSpeed);
      Track.track(speed);
    }

    
    if(runTask){
    int next = roofToStart45;
    while(next != end){
      if(next == startToRoof45 || next == roofToStart25){
        move(true);
        next = end;
      }else if(next == startToRoof25 || next == roofToStart45){
        move(false);
        next = end;
      }else if(next == lift45){
        lift(lift45degEncoderRead);
        next = end;
      }else if(next == lift25){
        lift(lift25degEncoderRead);
        next = end;
      }else if(next == liftPlatform){
        load();
        next = end;
      }else if(next == put45){
        put(lift45degEncoderRead);
        next = end;
      }else if(next == put25){
        put(lift25degEncoderRead);
        next = end;
      }else if(next == putPlatform){
        unload();
        next = end;
      }else{
        next = end;
      }     
    }
  }
   // delay(2000);
  // float sampleDistance = 8;
  // Serial.println("start: \n");
  // Serial.print("distance");
  // Serial.println(",speed, ");
  // float approachingSpeed = sonar.getApproachingSpeed(sampleDistance,20);    
  // float Distance = sonar.getDistance();
  // while(approachingSpeed != 0){
  //   approachingSpeed = sonar.getApproachingSpeed(sampleDistance,20);
  //   Distance =sonar.getDistance();
  //   Serial.print(Distance);
  //   Serial.print(",");
  //   Serial.print(approachingSpeed);
  //   Serial.println(",");
  //   delay(250);
  // }
  //Track.switchTrack(-turnSpeed);
  // while(sonar.getDistance() > roofOperationDsitance){
  //   Track.track(sonar.getApproachingSpeed(roofOperationDsitance,speed));
  // }
  // Track.stop();
  // Gripper.closeTo(1);
  // while(sonar.getDistance() < 2.5 + roofOperationDsitance){
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
  // while(sonar.getDistance() <= roofOperationDsitance){
  //     Track.track(-speed);
  // }
  // Track.stop();
  // delay(1000);
  // Track.switchTrack(turnSpeed);
  // Track.trackFor(speed,5);
  // while(true);
};