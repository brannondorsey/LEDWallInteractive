//
//  ContourSillhouette.cpp
//  ContourSilhouette
//
//  Created by bdorse on 1/19/16.
//
//

#include "ContourSillhouetteApp.h"
ContourSillhouetteApp::~ContourSillhouetteApp() {
    for (int i = 0; i < _walls.size(); i++) {
        delete _walls[i];
        _walls.erase(_walls.begin() + i);
    }
}

void ContourSillhouetteApp::setup(ofFbo* fbo_) {
    
    setFBO(fbo_);
    OpenCVEngine::getInstance().setup();
    
    ofColor bg = ofColor::turquoise;
    bg.setSaturation(255);
    _colorManager.setBackground(bg);
//    TrackedWall* wall = new TrackedWall(1);
//    wall->setup("Wall_Pablo_Straight.mov");
//    _walls.push_back(wall);
//    
//    wall = new TrackedWall(2);
//    wall->setup("Wall_Pablo_Straight.mov");
//    ofVideoPlayer& player = wall->videoSource->videoPlayer;
//    player.setFrame(int(ofRandom(player.getTotalNumFrames())));
//    _walls.push_back(wall);
    TrackedWall* wall = OpenCVEngine::getInstance().getWall(0);
    if (wall != NULL) _walls.push_back(wall);
    
    wall = OpenCVEngine::getInstance().getWall(1);
    if (wall != NULL) _walls.push_back(wall);
    
    _sillhouettes.resize(2);
    
    _largeFbo.allocate(OpenCVEngine::getInstance().videoSourceWidth * 2, OpenCVEngine::getInstance().videoSourceHeight);
    _largeFbo.begin();
        ofClear(0, 0, 0, 0);
        ofSetColor(_colorManager.getBackground());
        ofRect(0, 0, _largeFbo.getWidth(), _largeFbo.getHeight());
    _largeFbo.end();

    
    fbo->allocate(getWidth(), getHeight(), GL_RGB, 4);
    fbo->begin();
        ofClear(0, 0, 0, 0);
        ofSetColor(_colorManager.getBackground());
        ofRect(0, 0, getWidth(), getHeight());
        ofSetColor(255, 255, 255);
    fbo->end();
    
    _lastTimerMillis = 0;
    _alphaDecrement = 1.01;
    _scaleIncrement = 0.03;
    _hueInterval = 5;
    _timerMillis = 250;
}

void ContourSillhouetteApp::onAppSwitch() {
    
}

void ContourSillhouetteApp::update() {

    OpenCVEngine::getInstance().update();
    _colorManager.setHueInterval(_hueInterval);
    
    if (ofGetElapsedTimeMillis() - _lastTimerMillis > _timerMillis) {
        for (int i = 0; i < _walls.size(); i++) {
            
            if (_walls[i]->mainBlob.getVertices().size() > 0) {
                std::vector<ofPolyline> polylines;
                polylines.push_back(_walls[i]->mainBlob);
                ofPath path = _polysToPath(polylines);
                //                int sum = 0;
                //                std::vector<ofPolyline> poly = path.getOutline();
                //                for (int j = 0; j < poly.size(); j++) {
                //                    sum += poly[j].getVertices().size();
                //                }
                //                cout << sum << endl;
                path.simplify(3);
                //                sum = 0;
                //                poly.clear();
                //                poly = path.getOutline();
                //                for (int j = 0; j < poly.size(); j++) {
                //                    sum += poly[j].getVertices().size();
                //                }
                //                cout << sum << endl << endl;
                
//                path.scale(_scaleSum, _scaleSum);
//                ofPoint offset = _origCentroid - (_origCentroid * _scaleSum);
//                path.draw(offset.x, offset.y);

                ofPoint centroid = _walls[i]->mainBlob.getCentroid2D();
//                float scale = 0.40;
//                path.translate(ofPoint(centroid.x, 0));
//                path.scale(scale, scale);
//                ofPoint offset(centroid - (centroid * scale));
                
//                centroid.x = ofMap(centroid.x, 0, _walls[i]->videoSource->getWidth(), 0, fbo->getWidth());

//                centroid.y = 0;
                
                ContourSillhouette* s = new ContourSillhouette(path, centroid);
                _colorManager.update();
                s->setColor(_colorManager.getForeground());
                _sillhouettes[i].push_back(s);
            }
        }
        
        _lastTimerMillis = ofGetElapsedTimeMillis();
    }
    
    for (int i = 0; i < _sillhouettes.size(); i++) {
        for (int j = 0; j < _sillhouettes[i].size(); j++) {
            _sillhouettes[i][j]->alphaDecrement = _alphaDecrement;
            _sillhouettes[i][j]->scaleIncrement = _scaleIncrement;
            _sillhouettes[i][j]->update();
            if (_sillhouettes[i][j]->isDead()) {
                delete _sillhouettes[i][j];
                _sillhouettes[i].erase(_sillhouettes[i].begin() + j);
            }
        }
    }
    
    for (int i = 0; i < _walls.size(); i++) {
        _walls[i]->update();
    }
    
    _draw();
}

void ContourSillhouetteApp::_draw() {
    //    std::vector<ofPolyline> polylines;
    //    polylines.push_back(wall->mainBlob);
    //    ofPath path = polysToPath(polylines);
    
    //    path.setStrokeWidth(20);
    //    path.setColor(ofColor(255, 0, 0));
    //    path.setFilled(false);
    //    path.draw();
    _largeFbo.begin();
        ofSetColor(_colorManager.getBackground());
        ofRect(0, 0, _largeFbo.getWidth(), _largeFbo.getHeight());
        ofSetColor(255, 255, 255);
        for (int i = 0; i < _sillhouettes.size(); i++) {
            
            if (i == 1) {
                ofPushMatrix();
                ofTranslate(_largeFbo.getWidth()/2, 0);
            }
            
            for (int j = 0; j < _sillhouettes[i].size(); j++) {
                _sillhouettes[i][j]->draw();
            }
            
            if (i == 1) {
                ofPopMatrix();
            }
        }
    _largeFbo.end();
    
    fbo->begin();
        ofSetColor(_colorManager.getBackground());
        ofRect(0, 0, getWidth(), getHeight());
        ofSetColor(255);
        _largeFbo.draw(0, 0, getWidth(), getHeight());
    fbo->end();
    fbo->draw(0, 0);
    
    //
    //    ofPoint origCentroid = wall->mainBlob.getCentroid2D();
    //    ofPoint newCentroid = origCentroid * 0.7;
    //    ofPoint offset = origCentroid - newCentroid;
    //
    //    path.scale(0.7, 0.7);
    //    path.setFillColor(ofColor(0, 0, 255));
    //
    //    path.draw(offset.x, offset.y);
    
    //    wall->mainBlob.simplify();
    //    ofSetLineWidth(100);
    //    wall->mainBlob.draw();
}

ofPath ContourSillhouetteApp::_polysToPath(const vector<ofPolyline>& polylines) {
    ofPath path;
    for(int outline = 0; outline < polylines.size(); ++outline) {
        for (int i = 0; i < polylines[outline].getVertices().size(); i++){
            if ( i == 0 )
                path.moveTo(polylines[outline].getVertices()[i].x,polylines[outline].getVertices()[i].y);
            else
                path.lineTo(polylines[outline].getVertices()[i].x,polylines[outline].getVertices()[i].y);
        }
        path.close();
    }
    return path;
}

