#pragma once
class BlueMotor
{
public:
    BlueMotor();
    void setEffort(int effort);
    int setEffortWithoutDB(int effort);
    void motorBreak();
    void moveTo(long position);
    void stayAT(long position);
    long getPosition();
    void reset();
    void setup();
    float getFix(float,float,float,float,float);
    void motorTest();
    void motorPlot();

private:
    const int tolerance = 3;
    const int PWMOutPin = 11;
    const int IN2 = 4;
    const int IN1 = 13;
    static const int ENCA = 0;
    static const int ENCB = 1;
    long Kp = 30;
    long Ki = 6;
    int CPR = 540;
    int motorEffort = 400;
    int minMotorEffort = 130;
   
    void setEffort(int effort, bool clockwise);
    static void isrA();
    static void isrB();
};

