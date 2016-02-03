#pragma once



#include "ofMain.h"
#include "Scheduler.h"

#include "FadeCandyController.h"
#include "WallAppBase.h"


#include "OpenCVMinimalParticle.h"
#include "VideoPlayerApp.h"
#include "OpenCVBurstApp.h"
#include "OpenCVParticleApp.h"
#include "FFTVisualizerApp.h"
#include "ContourSillhouetteApp.h"

#if 0
#include "RowDrawApp.h"
#include "PongApp.h"
#endif

#include "StartupController.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int key);

    void clearWall();

    FadeCandyController* fadeCandyController;

    bool debug;

    vector<WallAppBase*> wallApps;
    int currentWallAppIndex;

    WallAppBase* currentApp;
    ofFbo fbo;

    bool doDrawDebug;

    StartupController startupController;
    Scheduler scheduler;
    bool hasStartedFadeCandy;
    bool disableFadeCandies;

    bool doSwapCameras;

};
