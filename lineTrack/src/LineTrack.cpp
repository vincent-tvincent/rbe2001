#include "LineTrack.h"

LineTrack::LineTrack(int lineADC, float kp, float ki, float kd){
    //dt = 1; 
    l0 = PIN_A0; 
    r0 = PIN_A2;
    l1 = PIN_A3;
    r1 = PIN_A4;
    l2 = PIN_A6;
    r2 = PIN_A7;
    LineADC = lineADC; 
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
    ADC_L0 = analogRead(PIN_A0);
    ADC_R0 = analogRead(PIN_A1);
    ADC_L1 = analogRead(PIN_A2);
    ADC_R1 = analogRead(PIN_A3);
    ADC_L2 = analogRead(PIN_A4);
    ADC_R2 = analogRead(PIN_A6);
}

void LineTrack::track(float speed){
    upDateADC();
    int rightSpeed = speed + getError();
    int leftSpeed = speed - getError();
    chassis.setWheelSpeeds(rightSpeed,leftSpeed);
}
bool LineTrack::isCross(){
    upDateADC();
    bool onTrack0 = ADC_L0 >= LineADC && ADC_R0 >= LineADC; 
    bool onTrack1 = ADC_L1 >= LineADC && ADC_R1 >= LineADC; 
    bool onTrack2 = ADC_L2 >= LineADC && ADC_R2 >= LineADC; 
    return onTrack0 && onTrack1 && onTrack2; 
}

float LineTrack::getError(){
    error = (ADC_L0 - ADC_R0) * W0 + (ADC_L1 - ADC_R1) * W1 + (ADC_L2 - ADC_R2) * W2;
    float Pout = error * Kp; 
    //float Iout = 0;
    //float Dout = (error - pError) / dt * Ki;  
    //pError = error;
    Serial.print(error);
    return Pout;
}