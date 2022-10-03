
#include<Romi32U4.h>
#include<LineTrack.h>
#include<gripper.h>
// Servo32U4 servo;
LineTrack Track(2,2,700); 
gripper Gripper(gripperStartEncoderCount,gripperEndEncoderCount);
void setup() {
  Gripper.init();
}
void loop() {
  // put your main code here, to run repeatedly:
  Gripper.release();
  delay(3000);
  Gripper.close();
  while(true);
}