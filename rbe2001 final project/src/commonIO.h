#ifndef COMIO
#define COMIO
#include <Chassis.h>
#include <servo32u4.h>

//line track
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
const int liftMax = 5000;
const int liftStart = 0;
const int startSensorPin = PIN_A1;
const int startOffset = -2500;

//sonar 
const int sonarEcho = 2;
const int sonarTrig = 12;
const float sonarDistanceTelorance = 0.5;

//servo
const int ServoPWM = 5;
const int servoMin = 500;
const int servoMax = 2500;
const int servoEncoder = PIN_A0;
const int servoErrorTolerance = 5;
const int servoMsPerCount = 5;
const int gripperStartEncoderCount = 475;
const int gripperEndEncoderCount = 90;
const int stall = 100;
const int gripperLockZone = 110;

//remote 
const int remotePin = 2;
//romi bot constant 
const float wheelDiameter = 7 * PI;
const float sonarOffSetDistance = 0;
const float effectiveApproachingRange = 30;
const float carRadius = 8;
const int encoderTickPerRevo = 1440;

#endif 