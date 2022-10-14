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
    bool move(int position);
    void stayAT(long position);
    long getPosition();
    void reset();
    void setup();
    float getFix(float count,float target,float Kp,float Kd);
    void motorTest();
    void motorPlot();
    void toStartPosition(int effort);
    void toStartPosition();

private:
    long Kp = 5;
    long Kd = 0.5;
    int error;
    int pError;
    int sensorPin = startSensorPin;
    void setEffort(int effort, bool clockwise);
    static void isrA();
    static void isrB();
};

