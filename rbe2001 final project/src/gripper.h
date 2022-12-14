#ifndef GRIPPER
#define GRIPPER
#include<commonIO.h>
#include<servo32u4.h>
class gripper{
    public: 
    gripper(int min, int max);
    void init();
    void release();
    void close();
    bool tryClose();
    void closeTo(float percent);
    void hold();
    private: 
    int countToMs(int count);
    int start;
    int end;
};
#endif 