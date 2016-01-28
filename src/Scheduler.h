#pragma once
#include "ofMain.h"
#include "OpenCVEngine.h"
#include "FadeCandyController.h"


class Scheduler
{
public:
    bool doSwitchApp;
    bool doShutDown;
    int startHour;
    int nextHour;
    
    int currentMinute;
    int nextMinute;
    int lastHourSwitchedApp;
    int lastMinuteSwitchedApp;

    Scheduler()
    {
        doSwitchApp = false;
        doShutDown = false;
    }
    
    void onUpdate(ofEventArgs& eventArgs)
    {
        int currentHour = ofGetHours();
        if(currentHour == 19)
        {
            doShutDown = true;
            ofRemoveListener(ofEvents().update, this, &Scheduler::onUpdate);
        }
        if(currentHour == nextHour)
        {
            nextHour = currentHour+1;
            if(currentHour+1 == 23)
            {
                nextHour = 0;
            }else
            {
                nextHour = currentHour+1;
            }
        }
        
        int currentMinute = ofGetMinutes();
        if(currentMinute == nextMinute)
        {
            if(currentMinute+1 == 60)
            {
                nextMinute = 0;
            }else
            {
                nextMinute = currentMinute+1;
            }
        }
        if(currentMinute == 1)
        {
            
            if(lastHourSwitchedApp != currentHour)
            {
                lastHourSwitchedApp = currentHour;
                doSwitchApp = true;
            }
        }
    }
    
    void setup()
    {
        
        startHour = ofGetHours();
        nextHour = startHour+1;
        currentMinute = ofGetMinutes();
        nextMinute = currentMinute+1;
        ofAddListener(ofEvents().update, this, &Scheduler::onUpdate);
        
    }
    
   

};
