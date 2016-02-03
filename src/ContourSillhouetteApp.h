//
//  ContourSillhouette.h
//  ContourSilhouette
//
//  Created by bdorse on 1/19/16.
//
//

#ifndef __ContourSilhouette__ContourSillhouette__
#define __ContourSilhouette__ContourSillhouette__

#include "ofMain.h"
#include "WallAppBase.h"
#include "ContourSillhouette.h"
#include "TrackedWall.h"
#include "ColorManager.h"
#include "OpenCVEngine.h"

class ContourSillhouetteApp : public WallAppBase {
public:
    
    ~ContourSillhouetteApp();
    void setup(ofFbo* fbo_);
    void update();
    void onAppSwitch();
    void setColor(const ofColor& color);
    
protected:
    
    void _draw(); // called internally by update()
    ofPath _polysToPath(const std::vector<ofPolyline>& polyline);
    unsigned long _lastTimerMillis;
    ColorManager _colorManager;
    
    std::vector<TrackedWall*> _walls;
    std::vector<std::vector<ContourSillhouette*> > _sillhouettes;

    float _alphaDecrement;
    float _scaleIncrement;
    float _hueInterval;
    int _timerMillis;

};

#endif /* defined(__ContourSilhouette__ContourSillhouette__) */
