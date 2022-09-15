#ifndef LINE_TRACK
#define LINE_TRACK
#include<commonIO.h>
//#include "Chassis.h"
class LineTrack{
    public: 
        LineTrack(int,float,float);
        virtual ~LineTrack(); 

        void track(float); // normal line track
        float bpTrack(float,float); // line track with backproperation 
        bool isCross();
        bool onTrack();
        void trackFor(float,float);
        void turnBack();
    private: 
        int l0; 
        int r0; 
        int l1; 
        int r1;
        int l2;
        int r2;
        int LineADC;
        int ADC_L0;
        int ADC_R0;
        int ADC_L1; 
        int ADC_R1;
        int ADC_L2;
        int ADC_R2;
        float error; 
        float pError;//previous error
        float W0; 
        float W1; 
        float W2;
        float Kp; 
        float Kd; 
        bool onTrack(int,int);
        void upDateADC();
        float getFix();
};
#endif