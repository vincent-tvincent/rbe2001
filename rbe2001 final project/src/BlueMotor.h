#pragma once
class BlueMotor
{
public:
    BlueMotor();
    void setEffort(int effort);
    int setEffortWithoutDB(int effort);
    void motorBreak();
    void moveTo(int position);
    void moveTo(int position, bool cw);
    void stayAT(long position);
    long getPosition();
    void reset();
    void setup();
    float getFix(float count,float prevCount,float target,float Kp,float Kd);
    void motorTest();
    void motorPlot();

private:
    const int tolerance = 100;
    const int PWMOutPin = 11;
    const int IN2 = 4;
    const int IN1 = 13;
    static const int ENCA = 0;
    static const int ENCB = 1;
    long Kp = 1.5;
    long Kd = 0;
    int CPR = 540;
    int motorEffort = 400;
    int minMotorEffort = 200;
   
    void setEffort(int effort, bool clockwise);
    static void isrA();
    static void isrB();
};

