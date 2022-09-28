#include <Arduino.h>
#include <BlueMotor.h>
#include <Romi32U4.h>

long oldValue = 0;
long newValue;
long count = 0;
unsigned time = 0;


BlueMotor::BlueMotor(){
    setup();
    reset();
}

void BlueMotor::setup()
{
    pinMode(PWMOutPin, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(ENCA, INPUT);
    pinMode(ENCB, INPUT);
    TCCR1A = 0xA8; //0b10101000; //gcl: added OCR1C for adding a third PWM on pin 11
    TCCR1B = 0x11; //0b00010001;
    ICR1 = 400;
    OCR1C = 0;

    attachInterrupt(digitalPinToInterrupt(ENCA), isrA, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCB),isrB,CHANGE);
    reset();
}

long BlueMotor::getPosition()
{
    long tempCount = 0;
    noInterrupts();
    tempCount = count;
    interrupts();
    return tempCount;
}

void BlueMotor::reset()
{
    noInterrupts();
    count = 0;
    interrupts();
}


void BlueMotor::isrA()
{
    if(digitalRead(ENCA) && digitalRead(ENCB)){
        count--;
    }else{
        count++;
    }
}

void BlueMotor::isrB()
{
    if(digitalRead(ENCA) && digitalRead(ENCB)){
        count++;
    }else{
        count--;
    }
}

void BlueMotor::setEffort(int effort)
{
    if (effort < 0)
    {
        setEffort(-effort, true);
    }
    else
    {
        setEffort(effort, false);
    }
}

void BlueMotor::setEffort(int effort, bool clockwise)
{
    if (clockwise)
    {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
    }
    else
    {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
    }
    OCR1C = constrain(effort, 0, 400);
}

void BlueMotor::motorBreak()
{
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
}

void BlueMotor::stayAT(long target)
{
    float count = getPosition();
    setEffort(motorEffort * getFix(count,target,Kp));
}

void BlueMotor::moveTo(long target)  
{                                 
    float count = getPosition();
    while(abs(target - count) > tolerance){
        setEffort(motorEffort * getFix(count,target,2));
        //Serial.println(motorEffort * getFix(count,target,1));
        count = getPosition();
        //delay(100);
    }
    motorBreak();
}

float BlueMotor::getFix(float count,float target,float Kp)
{
    return (target - count) / CPR * Kp;
}