#ifndef SONAR
#define SONAR
#include <Rangefinder.h>
#include <CommonIO.h>
class sonarApproach{
    public: 
        sonarApproach(int Kp,int Kd);
        void init();
        float getApproachingSpeed(float targetDistance,float maximumSpeed);// set the speed to appoporate value based on the target distance from the barrier and expected speed
        float getPerpotionalApproachingSpeed(float targetDsitance,float maximumSpeed);
        float getDistance();// get distance which considered the offset length due to the position sonar monted
        float setStartDistance(); // get the distance when approaching start
        bool haveDistance(float distance); // if have this distance
    private: 
        //int offesetDistance = 0; // distance from the sonar to the frot of the romibot 
        int Kp;
        int Kd;
        bool start; // a variable for mark if the sonar just start up for filter the exceptional value appear while first start up
        float errorRange; // the error value while prgram start, which is the maximum error value possible in the whole progress
        float error; // current error 
        float pError; // previous error 
        float distance; // distance collected by sonar 
        float getFix(float,float); // get the fix on speed in percentage 
};
#endif