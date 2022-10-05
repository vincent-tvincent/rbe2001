#include <sonarApproach.h>
sonarApproach:: sonarApproach(int kp, int kd){
    Kp = kp;
    Kd = kd;
    error = 0.0;
    distance = 0.0;
    pError = error;
    start = true;
}

float sonarApproach:: getDistance(){ // the get distance function of this class, which is for remove the offset distance from the sonar's position to the romi bot's front 
    //delay(5);
    distance = rangefinder.getDistance() - sonarOffSetDistance;
    Serial.println(distance);
    if(distance < sonarDistanceTolorance) distance = 0;
    return distance;
}

float sonarApproach:: setStartDistance(){ // once an approaching process start, record the start distance for generage fixing value 
    errorRange = getDistance();
    Serial.println(errorRange);
    return errorRange;
}

float sonarApproach:: getApproachingSpeed(float targetDistance, float maximumSpeed){
    Serial.println(errorRange);
    if(start){ // read distance twice for avoid the first exceptional value 
        setStartDistance();
        delay(50);
        setStartDistance();
        if(errorRange > effectiveApproachingRange){ // if the distance from the surface is too high, identifing as target lost, do not do approaching operation 
            return maximumSpeed;
        }else{
            start = false; // if there is object in effective distance, record the start distance and start approaching 
        }
    }
    distance = getDistance();
    error = distance - targetDistance; // update error 
    float speed = maximumSpeed * getFix(error,targetDistance); // apply fix to the speed value 
    pError = error; // update previous error 
    return speed;
}

float sonarApproach:: getFix(float error,float target){ // function for generate the fix value 
    if(errorRange == 0) return 0;
    float Pout = error/errorRange * Kp;
    float Dout = (error - pError)/errorRange * Kd;
    float fix = Pout + Dout;
    if(fix * fix < 0.1 * 0.1){
        fix = 0;
        start = true;
    }
    return fix;
}