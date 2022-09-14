#include "LineTrack.h"

LineTrack::LineTrack(int lineADC, float kp, float ki,float kd){
    l0 = PIN_A0; 
    r0 = PIN_A2;
    l1 = PIN_A3;
    r1 = PIN_A4;
    l2 = PIN_A6;
    r2 = PIN_A8;
    LineADC = lineADC; 
    error = 0;
    pError = 0;
    iError = 0;
    ADC_L0 = 0;
    ADC_R0 = 0; 
    ADC_L1 = 0;
    ADC_R1 = 0;
    ADC_L2 = 0;
    ADC_R2 = 0;
    Kp = kp;
    Ki = ki;
    Kd = kd;
    W0 = 1;
    W1 = 0; 
    W2 = 0;  
}

LineTrack::~LineTrack(){}

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
    int rightSpeed = speed * (1 + getFix());
    int leftSpeed = speed * (1 - getFix());
    chassis.setWheelSpeeds(rightSpeed,leftSpeed);
}
bool LineTrack::isCross(){
     return onTrack(ADC_L0,ADC_R0) && onTrack(ADC_L2,ADC_R2);
}
bool LineTrack::onTrack(){return onTrack(ADC_L0,ADC_R0);}
bool LineTrack::onTrack(int lADC, int rADC){
    upDateADC();
    return lADC >= LineADC && rADC >= LineADC;
}

float LineTrack::getFix(){
    error = ADC_R0 - ADC_L0;
    if(error * error < 52 * 52){error = 0;}
    float Pout = (error/1023) * Kp; 
    float Iout = (iError/1023) * Ki;
    float Dout = ((error - pError)/1023) * Kd;  
    pError = error;
    iError += error;
    return Pout + Iout + Dout;
}