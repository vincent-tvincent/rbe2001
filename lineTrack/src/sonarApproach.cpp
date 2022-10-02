#include <sonarApproach.h>
sonarApproach:: sonarApproach(int kp, int kd){
    Kp = kp;
    Kd = kd;
    error = 0.0;
    distance = 0.0;
    pError = error;
    start = true;
}

float sonarApproach:: getDistance(){
    //delay(5);
    distance = rangefinder.getDistance() - sonarOffSetDistance;
    Serial.println(distance);
    if(distance < sonarDistanceTolorance) distance = 0;
    return distance;
}

float sonarApproach:: setStartDistance(){
    errorRange = getDistance();
    Serial.println(errorRange);
    return errorRange;
}

float sonarApproach:: getApproachingSpeed(float targetDistance, float maximumSpeed){
    Serial.println(errorRange);
    if(start){
        setStartDistance();
        delay(50);
        setStartDistance();
        if(errorRange > effectiveApproachingRange){
            return maximumSpeed;
        }else{
            start = false;
        }
    }
    distance = getDistance();
    error = distance - targetDistance;
    float speed = maximumSpeed * getFix(error,targetDistance);
    Serial.print("speed: ");
    Serial.println(speed);
    pError = error;
    return speed;
}

float sonarApproach:: getFix(float error,float target){
    if(errorRange == 0) return 0;
    Serial.println(errorRange);
    float Pout = error/errorRange * Kp;
    Serial.println(Pout);
    float Dout = (error - pError)/errorRange * Kd;
    Serial.println(error - pError);
    Serial.println(Dout);
    Serial.println(Pout + Dout);
    float fix = Pout + Dout;
    if(fix * fix < 0.1 * 0.1){
        fix = 0;
        start = true;
    }
    return fix;
}