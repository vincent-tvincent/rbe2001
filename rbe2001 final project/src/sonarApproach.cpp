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
    if(distance < sonarDistanceTelorance) distance = 0;
    return distance;
}

float sonarApproach:: setStartDistance(){
    //Serial.println("start distance updated");
    errorRange = getDistance();
    //Serial.println("error range: ");
    //Serial.println(errorRange);
    return errorRange;
}

float sonarApproach:: getApproachingSpeed(float targetDistance, float maximumSpeed){
    //Serial.print("error range: ");
    //Serial.println(errorRange);
    if(start){
        setStartDistance();
        delay(50);
        setStartDistance();
        if(errorRange > effectiveApproachingRange){
            //Serial.println("no target");
            return maximumSpeed;
        }else{
            //Serial.println("get target");
            start = false;
        }
    }
    distance = getDistance();
    error = distance - targetDistance;
    //Serial.print("error: ");
    //Serial.println(error);
    float speed = maximumSpeed * getFix(error,targetDistance);
    // Serial.print("approaching speed: ");
    // Serial.println(speed);
    //Serial.print("speed: ");
    //Serial.println(speed);
    pError = error;
    return speed;
}

float sonarApproach:: getFix(float error,float target){
    if(errorRange == 0) return 0;
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
    if(fix * fix < 0.05 * 0.05){
        fix = 0;
        start = true;
    }
    return fix;
}

float sonarApproach::getPerpotionalApproachingSpeed(float targetDistance, float maximumspeed){
    error = getDistance() - targetDistance;
    float Pout = error * Kp / targetDistance;
    float Dout = (error - pError) / targetDistance * Kd;
    pError = error;
    return maximumspeed * (Pout + Dout);
}

bool sonarApproach::checkDistance(float distance){
    // Serial.print("\ncheck distance: ");
    // Serial.println(abs(distance - getDistance()));
    // Serial.print("check result: ");
    // Serial.println(abs(distance - getDistance()) < sonarDistanceTelorance);
    return abs(distance - getDistance()) < sonarDistanceTelorance;
}