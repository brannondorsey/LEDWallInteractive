//
//  FluidDynamicsApp.cpp
//  LEDWallVideoPlayer
//
//  Created by bdorse on 2/2/16.
//
//

#include "FluidDynamicsApp.h"

FluidDynamicsApp::FluidDynamicsApp() {
    
};

FluidDynamicsApp::~FluidDynamicsApp() {
    
};

void FluidDynamicsApp::setup(ofFbo *fbo_) {

    setFBO(fbo_);
    
    OpenCVEngine::getInstance().setup();
    
    ofColor bg = ofColor(ofRandom(255), ofRandom(255), ofRandom(255));
    bg.setSaturation(255);
    _colorManager.setBackground(bg);
    
    TrackedWall* wall = OpenCVEngine::getInstance().getWall(0);
    if (wall != NULL) _walls.push_back(wall);
    
    wall = OpenCVEngine::getInstance().getWall(1);
    if (wall != NULL) _walls.push_back(wall);
    
    _largeFbo.allocate(OpenCVEngine::getInstance().videoSourceWidth * 2, OpenCVEngine::getInstance().videoSourceHeight);
    _largeFbo.begin();
    ofClear(0, 0, 0, 0);
    ofSetColor(_colorManager.getBackground());
    ofRect(0, 0, _largeFbo.getWidth(), _largeFbo.getHeight());
    _largeFbo.end();
    
    _fluid.allocate(_largeFbo.getWidth(), _largeFbo.getHeight(), 0.5);
    
    _fluid.dissipation = 0.99;
    _fluid.velocityDissipation = 0.99;
    
    _fluid.setGravity(ofVec2f(0.0,0.0));
    
//    _fluid.begin();
//    ofSetColor(0,0);
//    ofSetColor(255);
//    ofCircle(width*0.5, height*0.35, 40);
//    _fluid.end();
    _fluid.setUseObstacles(false);
    
    ofFbo obstacleFbo;
    obstacleFbo.allocate(_largeFbo.getWidth(), _largeFbo.getHeight());
    obstacleFbo.begin();
    ofClear(0, 0, 0, 0);
    ofPushStyle();
    ofSetColor(255);
    ofRect(0, 0, _largeFbo.getWidth(), _largeFbo.getHeight());
    ofSetColor(0);
    int borderSize = 1;
    ofRect(borderSize, borderSize, _largeFbo.getWidth() - borderSize * 2, _largeFbo.getHeight() - borderSize * 2);
    ofPopStyle();
    obstacleFbo.end();
    
    _fluid.setObstacles(obstacleFbo);
    
    // Adding constant forces
    //    fluid.addConstantForce(ofPoint(width*0.15,height*0.5), ofPoint(2,0), ofFloatColor(0.5,0.1,0.0), 20.f);
    
//    fbo.allocate(width, height, GL_RGB, 4);
//    fbo.begin();
//    ofClear(0, 0, 0, 0);
//    fbo.end();
    
    // setup screen shader
    string shaderProgram = STRINGIFY(
                                     uniform sampler2DRect tex0; // this is how we receive the texture
                                     uniform vec4 blend;
                                     varying vec2 texCoordVarying;
                                     varying vec4 colorVarying;
                                     
                                     vec4 screen(vec4 base, vec4 blend) {
                                         return vec4(1.0) - (vec4(1.0) - blend) * (vec4(1.0) - base);
                                     }
                                     
                                     void main()
                                     {
                                         gl_FragColor = screen(texture2DRect(tex0, gl_TexCoord[0].st), blend);
                                     }
                                     );
    
    _screenShader.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderProgram);
    _screenShader.linkProgram();
    
    ofColor c(ofRandom(255), ofRandom(255), ofRandom(255));
    c.setSaturation(255);
    _colorManager.setBackground(c);
    _colorManager.setHueInterval(0.1);
};

void FluidDynamicsApp::update() {
    
    _colorManager.update();
    
    OpenCVEngine::getInstance().update();
    
    for (int i = 0; i < _walls.size(); i++) {
        if (_walls[i]->mainBlob.getVertices().size() > 0) {
            
            // Adding temporal Force
            ofPoint m = _walls[i]->mainBlob.getCentroid2D();// ofPoint(mouseX, mouseY);
            if (i == 1) m.x += _largeFbo.getWidth() / 2;

            float margin = _largeFbo.getHeight() * 0.20;
            m.y = ofMap(((i == 0) ? sin(ofGetElapsedTimef()) : cos(ofGetElapsedTimef())), -1.0, 1.0, margin, _largeFbo.getHeight() - margin);
            
            ofPoint d = (m - ((i == 0) ? _prevPoints.first : _prevPoints.second)) * 10.0;
            ofPoint c = ofPoint(_largeFbo.getWidth() * 0.5, _largeFbo.getHeight() * 0.5) - m;
            c.normalize();
            //    fluid.addTemporalForce(m, d, ofFloatColor(c.x, c.y,0.5) * sin(ofGetElapsedTimef()), 10.0f);
            ofFloatColor color(_colorManager.getForeground());
            color.setBrightness(0.2);
            _fluid.addTemporalForce(m, d, color, 10.0f);
            if (i == 0) _prevPoints.first = m;
            else _prevPoints.second = m;

        }
    }
    
    _fluid.update();
    _draw();
};

void FluidDynamicsApp::onAppSwitch() {
    
};

void FluidDynamicsApp::_draw() {
    
    ofFloatColor bgColor(_colorManager.getBackground());
    
    _largeFbo.begin();
        _screenShader.begin();
        _screenShader.setUniform4f("blend", bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        _fluid.draw();
        _screenShader.end();
    _largeFbo.end();
    
    fbo->begin();
        _largeFbo.draw(0, 0, getWidth(), getHeight());
    fbo->end();
    fbo->draw(0, 0);

};

