#pragma once
#include<commonIO.h>
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
    float getFix(float count,float target,float Kp,float Kd);
    void motorTest();
    void motorPlot();
    void toStartPosition(bool sensor, int effort);
    void toStartPosition();

private:
    long Kp = 2.5;
    long Kd = 0.7;
    int error;
    int pError;
    void setEffort(int effort, bool clockwise);
    static void isrA();
    static void isrB();
};

