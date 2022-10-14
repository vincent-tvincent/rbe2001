#ifndef LINE_TRACK
#define LINE_TRACK
#include<commonIO.h>
#include <Chassis.h>
class LineTrack{
    public: 
        LineTrack(int,float Kp,float Kd);
        virtual ~LineTrack(); 
        void init();
        void track(float speed); // line track
        bool track(float speed, float distance);
        void stop();
        void trackSetStart();
        bool isCross(); // detect if go to the cross 
        bool onTrack(); // if the most inside pair of sensor on the line 
        void trackFor(float speed,float distance); // line track for distance 
        void turnBack(float turnSpeed); // turning back 
        void switchTrack(float turnSpeed); // when go to cross, give it option to swith to the intersecting track on left or right
        //void trackTill(float); // keep tracking line until the distance from barrier at front become the input value
        void travel(float speed,int turnSpeed, int* map); // move on map
        int toRight = 1;
        int skip = 0;
        int toLeft = -1;
        int Stop = 2;
    private: 
        int LineADC;
        int ADC_L0;
        int ADC_R0;
        int ADC_L1; 
        int ADC_R1;
        int ADC_L2;
        int ADC_R2;
        int start;
        float direction; // a peremeter repreasent if the romi bot is drving in positive direction or negative direction
        float error; //current error
        float pError;//previous error
        float W0; 
        float W1; 
        float W2;
        float Kp; 
        float Kd; 
        bool onTrack(int,int); // check if the given pair of sensors both on the line
        void upDateADC();
        float getFix(); // get the error fixing value in percentage 
};
#endif