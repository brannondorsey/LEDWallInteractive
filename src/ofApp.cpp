#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    doSwapCameras = false;
    ofSetEscapeQuitsApp(false);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetLogLevel("ofThread", OF_LOG_SILENT);
    doDrawDebug = false;
    
    //options for testing locally
    //see also OpenCVEngine::setup to force movie
    disableFadeCandies = true;
    bool skipRPiReboot = true;
    
    startupController.setup(skipRPiReboot);
    
    ofBackground(ofColor::black);

   
    fbo.allocate(480, 120);
    fbo.begin();
        ofClear(0, 0, 0, 0);
    fbo.end();
    
    OpenCVBurstApp* openCVBurstApp = new OpenCVBurstApp();
    openCVBurstApp->name = "OpenCVBurstApp";
    openCVBurstApp->setup(&fbo);
    wallApps.push_back(openCVBurstApp);
    
    OpenCVMinimalParticle* openCVMinimalParticle = new OpenCVMinimalParticle();
    openCVMinimalParticle->setup(&fbo);
    openCVMinimalParticle->name = "OpenCVMinimalParticle";
    wallApps.push_back(openCVMinimalParticle);
    
    OpenCVParticleApp* openCVParticleApp = new OpenCVParticleApp();
    openCVParticleApp->name = "OpenCVParticleApp";
    openCVParticleApp->setup(&fbo);
    wallApps.push_back(openCVParticleApp);
    
    VideoPlayerApp* videoPlayerApp = new VideoPlayerApp();
    videoPlayerApp->name = "VideoPlayerApp";
    videoPlayerApp->setup(&fbo);
    wallApps.push_back(videoPlayerApp);
    
#if 0
    PongApp* pongApp = new PongApp();
    pongApp->name = "PongApp";
    pongApp->setup(&fbo);
    wallApps.push_back(pongApp);
#endif

    currentWallAppIndex = 0;
    currentApp = wallApps[currentWallAppIndex];
    float aspect = currentApp->getWidth() / currentApp->getHeight();
    ofSetWindowShape(ofGetScreenWidth() / 2, (ofGetScreenWidth() / 2) / aspect);
    
    hasStartedFadeCandy = false;


}
//--------------------------------------------------------------
void ofApp::update(){
   
    if(!startupController.isReady) return;
    if(!hasStartedFadeCandy && !disableFadeCandies)
    {
        vector<string> hosts;
        hosts.push_back("192.168.2.100");
        hosts.push_back("192.168.2.101");
        hosts.push_back("192.168.2.102");
        hosts.push_back("192.168.2.103");
        fadeCandyController = new FadeCandyController();
        
        fadeCandyController->setup(hosts);
        
        bool didConnect = fadeCandyController->connect();
        ofLogVerbose(__func__) << "didConnect: " << didConnect;

        scheduler.setup();
        hasStartedFadeCandy = true;
    }
    
    if(scheduler.doShutDown)
    {
        ofExit(0);
        return;
    }
    if(scheduler.doSwitchApp)
    {
        if (currentWallAppIndex+1 < wallApps.size())
        {
            currentWallAppIndex++;
            
        }else
        {
            currentWallAppIndex = 0;
        }
        OpenCVEngine::getInstance().clearTrackedWallFBOs();
        fbo.begin();
            ofClear(0, 0, 0, 0);
        fbo.end();
        if (fadeCandyController)
        {
            if (fadeCandyController->allClientsConnected())
            {
                ofPixels clearPixels;
                clearPixels.allocate(480, 120, OF_PIXELS_RGBA);
                clearPixels.set(0);
                fadeCandyController->update(clearPixels);
            }
        }
        ofSleepMillis(1000);
        currentApp = wallApps[currentWallAppIndex];
        currentApp->onAppSwitch();
        
        
        scheduler.doSwitchApp = false;
        
    }

    if (!currentApp)
    {
        return;
    }

    currentApp->update();
    if (fadeCandyController)
    {
        if (fadeCandyController->allClientsConnected())
        {
            fadeCandyController->update(currentApp->getPixelsRef());
        }
        
    }
   
    ofSetWindowTitle(currentApp->name);
}


//--------------------------------------------------------------
void ofApp::draw()
{
    if(!startupController.isReady)
    {
        ofDrawBitmapString(startupController.statusLog.str(), 20, 20);
        return;
    }
    
    if(doDrawDebug)
    {
        ofPushMatrix();
        for (int i=0; i<OpenCVEngine::getInstance().trackedWalls.size(); i++)
        {
            ofTranslate(i*OpenCVEngine::getInstance().videoSourceWidth, 0);
            OpenCVEngine::getInstance().trackedWalls[i]->videoSource->draw(0, 0);
            ofPushMatrix();
            ofTranslate(0, 480);
            OpenCVEngine::getInstance().trackedWalls[i]->background.drawDebug(.5);
            ofCircle(OpenCVEngine::getInstance().trackedWalls[i]->mainBlob.getCentroid2D(), 20);
            OpenCVEngine::getInstance().trackedWalls[i]->contourFinder.draw();
            ofPopMatrix();
            
        }
        ofPopMatrix();
        fbo.draw(0, 0, fbo.getWidth(), fbo.getHeight());
    }else{
        fbo.draw(0, 0, ofGetWidth(), ofGetHeight()/2);
    }

    ofDrawBitmapStringHighlight(ofToString( (int) ofGetFrameRate()) + " FPS", 5, 15);
    if (doSwapCameras)
    {
        OpenCVEngine::getInstance().swapCameras();
        doSwapCameras = false;
    }
}

void ofApp::clearWall()
{
    ofPixels pixels;
    pixels.allocate(480, 120, OF_PIXELS_RGBA);
    pixels.set(0);
    if(fadeCandyController)
    {
        fadeCandyController->update(pixels);

    }
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key)
    {
        case ' ':
        {
            debug = !debug;
            break;
        }
        case 's':
        {
            scheduler.doSwitchApp = true;
            break;
        }
        case 'd':
        {
            doDrawDebug = !doDrawDebug;
            break;
        }
        case 'v':
        {
            doSwapCameras = true;
            break;
        }
        default:
            break;
    }
}

void ofApp::exit()
{
    ofLogVerbose() << "EXITING";
    clearWall();
}

