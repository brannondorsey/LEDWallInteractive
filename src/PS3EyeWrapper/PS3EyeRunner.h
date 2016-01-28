#pragma once
#include "ofMain.h"

class PS3EyeRunner : public ofThread
{
public:
    static PS3EyeRunner& getInstance()
    {
        static PS3EyeRunner    instance;
        return instance;
    }
    void start()
    {
        startThread(false);   // blocking, verbose
    }
    
    void stop()
    {
        stopThread();
    }
private:
    PS3EyeRunner()
    {
    }
   
    
    //--------------------------
    void threadedFunction()
    {
        while( isThreadRunning() != 0 )
        {
            bool res = ps3eye::PS3EYECam::updateDevices();
            if(!res)
            {
                break;
            }
        }
    }
    
    ~PS3EyeRunner() {};
    PS3EyeRunner(PS3EyeRunner const&);
    void operator=(PS3EyeRunner const&);
};
