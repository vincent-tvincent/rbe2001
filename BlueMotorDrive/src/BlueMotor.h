#pragma once
class BlueMotor
{
public:
    BlueMotor();
    void setEffort(int effort);
    void motorBreak();
    void moveTo(long position);
    void stayAT(long position);
    long getPosition();
    void reset();
    void setup();

private:
    long Kp = 2;
    long timeToPrint = 0;
    long now = 0;
    long newPosition = 0;
    long oldPosition = 0;
    long sampleTime = 100;
    int speedInRPM = 0;
    int CPR = 270;
    int motorEffort = 400;
    const int tolerance = 3;
    const int PWMOutPin = 11;
    const int IN2 = 4;
    const int IN1 = 13;
    static const int ENCA = 0;
    static const int ENCB = 1;
    void setEffort(int effort, bool clockwise);
    float getFix(float,float,float);
    static void isrA();
    static void isrB();
    
};

