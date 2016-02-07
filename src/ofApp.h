#pragma once



#include "ofMain.h"
#include "Scheduler.h"

#include "FadeCandyController.h"
#include "WallAppBase.h"
#include "ColorSchemes.h"

#include "OpenCVMinimalParticle.h"
#include "VideoPlayerApp.h"
#include "OpenCVBurstApp.h"
#include "OpenCVParticleApp.h"
#include "FFTVisualizerApp.h"
#include "ContourSillhouetteApp.h"
#include "FluidDynamicsApp.h"

#if 0
#include "RowDrawApp.h"
#include "PongApp.h"
#endif

#include "StartupController.h"
#include "RPiSceneChanger.h"

class ofApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int key);

    void clearWall();
    void onRPiSceneChangeButtonPress(void);

    FadeCandyController* fadeCandyController;

    bool debug;

    vector<WallAppBase*> wallApps;
    int currentWallAppIndex;

    WallAppBase* currentApp;
    ofFbo fbo;

    bool doDrawDebug;

    StartupController startupController;
    Scheduler scheduler;
    ColorSchemes colorSchemes;
    bool hasStartedFadeCandy;
    bool disableFadeCandies;

    bool doSwapCameras;
    RPiSceneChanger sceneChanger;

};
