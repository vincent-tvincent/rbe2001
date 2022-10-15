#include <sonarApproach.h>
Rangefinder rangefinder(sonarEcho,sonarTrig);
sonarApproach:: sonarApproach(int kp, int kd){
    Kp = kp;
    Kd = kd;
    error = 0.0;
    distance = 0.0;
    pError = error;
    start = true;
}

void sonarApproach::init(){
    rangefinder.init();
}
float sonarApproach:: getDistance(){
    //delay(5);
    distance = rangefinder.getDistance() - sonarOffSetDistance;
    Serial.println(distance);
    if(distance < sonarDistanceTelorance) distance = 0;
    return distance;
}

float sonarApproach:: setStartDistance(){
    Serial.println("start distance updated");
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
            Serial.println("no target");
            return maximumSpeed;
        }else{
            Serial.println("get target");
            start = false;
        }
    }
    distance = getDistance();
    error = distance - targetDistance;
    //Serial.print("error: ");
    Serial.println(error);
    float speed = maximumSpeed * getFix(error,targetDistance);
    Serial.print("speed: ");
    //Serial.println(speed);
    pError = error;
    return speed;
}

float sonarApproach:: getFix(float error,float target){
    if(errorRange - target < 0.1) return 0;
    //Serial.print("error range");
    //Serial.println(errorRange);
    float Pout = (float)error/(float)errorRange * Kp;
    //Serial.print("P out: ");
    //Serial.println(Pout);
    float Dout = (float)(error - pError)/(float)errorRange * Kd;
    //Serial.println(" dError, D out: ");
    //Serial.println(error - pError);
    //Serial.println(Dout);
    //Serial.print("output");
    //Serial.println(Pout + Dout);
    float fix = Pout + Dout;
    if(fix * fix < 0.1 * 0.1){
        fix = 0;
        start = true;
    }
    return fix;
}

float sonarApproach::getPerpotionalApproachingSpeed(float targetDistance, float maximumspeed){
    error = getDistance() - targetDistance;
    float Pout = error / targetDistance * Kp;
    float Dout = (error - pError) / targetDistance * Kd;
    pError = error;
    return maximumspeed * (Pout + Dout);
}

bool sonarApproach::haveDistance(float distance){
    return abs(distance - getDistance()) < sonarDistanceTelorance + 0.1;
}