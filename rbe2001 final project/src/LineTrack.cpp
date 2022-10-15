#include "LineTrack.h"
Chassis chassis;
LineTrack::LineTrack(int lineADC, float kp, float kd){
    direction = 1;
    LineADC = lineADC; 
    error = 0;
    pError = 0;
    ADC_L0 = 0;
    ADC_R0 = 0; 
    ADC_L1 = 0;
    ADC_R1 = 0;
    ADC_L2 = 0;
    ADC_R2 = 0;
    Kp = kp;
    Kd = kd;
    W0 = 1;
    W1 = 0; 
    W2 = 0;  
}

LineTrack::~LineTrack(){}

void LineTrack::init(){
    chassis.init();
}

void LineTrack:: upDateADC(){
    ADC_L0 = analogRead(l0);
    ADC_R0 = analogRead(r0);
    ADC_L1 = analogRead(l1);
    ADC_R1 = analogRead(r1);
    ADC_L2 = analogRead(l2);
    ADC_R2 = analogRead(r2);
}

void LineTrack::track(float speed){
    upDateADC();
    if(speed < 0){
        chassis.setWheelSpeeds(speed,speed);
    }else{
        //fix the speeds by the percentange of it self
        float rightSpeed = speed * (1 + getFix());
        float leftSpeed = speed * (1 - getFix());
        chassis.setWheelSpeeds(rightSpeed,leftSpeed);
    }
}

void LineTrack::trackSetStart(){
    start = (chassis.getLeftEncoderCount() + chassis.getRightEncoderCount())/2;
}

void LineTrack::trackFor(float speed,float distance){
    int start = (chassis.getLeftEncoderCount() + chassis.getRightEncoderCount())/2;
    int end = distance / wheelDiameter * encoderTickPerRevo;
    while((chassis.getLeftEncoderCount() + chassis.getRightEncoderCount())/2 - start < end){
        track(speed);
    }
    chassis.setWheelSpeeds(0,0);
}

void LineTrack::stop(){
    chassis.setWheelSpeeds(0,0);
}
void LineTrack::switchTrack(float turnSpeed){
   upDateADC();

    while(onTrack(ADC_L0,ADC_R0)) {
        Serial.println("go out");
        chassis.setTwist(0,turnSpeed);
        upDateADC();
    }
    delay(100);
    while(!onTrack(ADC_L0,ADC_R0)){
        Serial.println("go on");
        chassis.setTwist(0,turnSpeed);
        upDateADC();
    }
    chassis.setMotorEfforts(0,0);
}

bool LineTrack::isCross(){
    upDateADC();
    return onTrack(ADC_L0,ADC_R0) && onTrack(ADC_L2,ADC_R2);
}

bool LineTrack::onTrack(){
    upDateADC();
    return onTrack(ADC_L0,ADC_R0);
}

bool LineTrack::onTrack(int lADC, int rADC){
    Serial.println(lADC >= LineADC && rADC >= LineADC);
    return lADC >= LineADC && rADC >= LineADC;
}

float LineTrack::getFix(){
    error = (ADC_R0 - ADC_L0) * direction;// get error 
    if(error * error < lineTrackerADCTolorance * lineTrackerADCTolorance){error = 0;}
    float Pout = (error/1023) * Kp; // applying P control 
    float Dout = ((error - pError)/1023) * Kd;  // applying D control
    pError = error;
    return Pout + Dout; // generate output 
}

void LineTrack::travel(float speed,int turnSpeed, int* map){
    int nextAction = 0;
    while(map[nextAction] != Stop){
        track(speed);
        if(isCross()){
            stop();
            trackFor(speed,carRadius);
            stop();
            delay(100);
            // if(map[nextAction] == toRight){
            //     switchTrack(-turnSpeed);
            //     track(speed);
            // }else if(map[nextAction] == toLeft){
            //     switchTrack(turnSpeed);
            //     track(speed);
            // }
            // nextAction++;
            while(true);
        }
    }
    stop();
}
