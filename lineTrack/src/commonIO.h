#ifndef COMIO
#define COMIO
#include <Chassis.h>
#include <servo32u4.h>
const int encoderTickPerSecond = 1440;
const int l0 = PIN_A3; 
const int r0 = PIN_A2;
const int l1 = -1;
const int r1 = -1;
const int l2 = PIN_A6;
const int r2 = PIN_A4;
const int sonarEcho = 2;
const int sonarTrig = 12;
const int ServoPWM = 5;
const int servoMin = 500;
const int servoMax = 2500;
const int ServoEncoder = PIN_A0;
const int servoMsPerCount = 3;
const int lineTrackerADCTolorance = 52;
const int gripperStartEncoderCount = 80;
const int gripperEndEncoderCount = 300;
const float wheelPeremeter = 7 * PI;
const float sonarOffSetDistance = 0;
const float effectiveApproachingRange = 30;
const float carRadius = 7.62;
const float sonarDistanceTolorance = 0.2;
#endif 