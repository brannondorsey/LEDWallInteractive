//
//  RPiSceneChanger.h
//  LEDWallVideoPlayer
//
//  Created by bdorse on 2/6/16.
//
//

#ifndef __LEDWallVideoPlayer__RPiSceneChanger__
#define __LEDWallVideoPlayer__RPiSceneChanger__

#include "ofxOsc.h"

class RPiSceneChanger {
public:
    
    ofEvent<void> sceneChangeEvent;
    void setup() {
        int port = 12345;
        _receiver.setup(port);
        ofLogVerbose("RPiSceneChanger::setup") << "Listening on port " << port;
    }
    
    void update() {
        while(_receiver.hasWaitingMessages()){
            // get the next message
            ofxOscMessage m;
            _receiver.getNextMessage(&m);
            if (m.getAddress() == "/moment") {
                ofNotifyEvent(sceneChangeEvent);
            }
        }
    }

protected:
    ofxOscReceiver _receiver;
};

#endif /* defined(__LEDWallVideoPlayer__RPiSceneChanger__) */
