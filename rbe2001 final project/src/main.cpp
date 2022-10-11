
#include<Romi32U4.h>
#include<LineTrack.h>
#include<gripper.h>
#include<BlueMotor.h>
//Servo32U4 servo;
LineTrack Track(1,1,700); 
BlueMotor motor;
gripper Gripper(gripperStartEncoderCount,gripperEndEncoderCount);
void setup() {
  Gripper.init();
  Track.init();
  motor.setup();
  motor.reset();
}
void loop() {
  // put your main code here, to run repeatedly:
  
  //servo.writeMicroseconds(600);

  // Gripper.release();
  // delay(500);
  // Gripper.close();
  // delay(5000);
  
  // motor.moveTo(-5200);
  // delay(1000);
  // Gripper.release();
  // while (true);
  
  //motor.moveTo(lift25degEncoderRead);


  // //Gripper.release();
  // Serial.println("1 done");
  
  // delay(500);
  // //Gripper.close();
  // Serial.println("2 done");
  // delay(3000);
  // motor.moveTo(-5000);
  // Serial.println("3 done");
  // delay(1500);
  // motor.moveTo(lift45degEncoderRead+100);
  // Serial.println("4 done");
  // delay(1500);
  // //motor.moveTo(lift45degEncoderRead + 2000);
  // //delay(1500);
  // Gripper.release();
  // Serial.println("5 done");
  //while(true);
  // int Count = motor.getPosition();
  // int prevCount = 0;
  // do{
  //   prevCount = Count;
  //   motor.setEffort(-400);
  //   Count = motor.getPosition();
  // }while(Count != prevCount);
  // motor.reset();
  motor.toStartPosition();
  delay(1000);
  Gripper.release();
  delay(3000);
  Gripper.close();
  delay(1000);
  motor.moveTo(carryEncoderRead);
  while(true){
    Gripper.hold();
    Track.track(10);
  }
};