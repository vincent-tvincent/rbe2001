#ifndef COMIO
#define COMIO
#include <Chassis.h>
const int encoderTickPerSecond = 1440;
const int l0 = PIN_A3; 
const int r0 = PIN_A2;
const int l1 = -1;
const int r1 = -1;
const int l2 = PIN_A6;
const int r2 = PIN_A4;
const int sonarEcho = 11;
const int sonarTrig = 12;
const int lineTrackerADCTolorance = 52;
const float wheelPeremeter = 7 * PI;
const float sonarOffSetDistance = 9.7;
const float effectiveApproachingRange = 30;
const float carRadius = 7.62;
const float sonarDistanceTolorance = 0.2;
#endif 