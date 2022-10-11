#ifndef COMIO
#define COMIO
#include <Chassis.h>
#include <servo32u4.h>

//line track
const int encoderTickPerSecond = 1440;
const int l0 = PIN_A3; 
const int r0 = PIN_A2;
const int l1 = -1;
const int r1 = -1;
const int l2 = PIN_A6;
const int r2 = PIN_A4;
const int lineTrackerADCTolorance = 52;

//motor 
const int tolerance = 100;
const int PWMOutPin = 11;
const int IN2 = 3;
const int IN1 = 13;
static const int ENCA = 0;
static const int ENCB = 1;
const int CPR = 540;
const int motorEffort = 400;
const int minMotorEffort = 200;
const int lift45degEncoderRead = 4300;
const int lift25degEncoderRead = 4862;
const int carryEncoderRead = 5000;
const int liftMax = 9000;
const int liftStart = 2000;

//sonar 
const int startSensor = PIN_A1;
const int sonarEcho = 11;
const int sonarTrig = 12;

//servo
const int ServoPWM = 5;
const int servoMin = 500;
const int servoMax = 2500;
const int servoEncoder = PIN_A0;
const int servoErrorTolerance = 5;
const int servoMsPerCount = 5;
const int gripperStartEncoderCount = 90;
const int gripperEndEncoderCount = 225;
const int stall = 10;

//romi bot constant 
const float wheelPeremeter = 7 * PI;
const float sonarOffSetDistance = 9.7;
const float effectiveApproachingRange = 30;
const float carRadius = 7.62;
const float sonarDistanceTolorance = 0.2;

#endif 