//
//  FFTVisualizer.cpp
//  LEDWallVideoPlayer
//
//  Created by bdorse on 1/28/16.
//
//

#include "FFTVisualizerApp.h"

FFTVisualizerApp::FFTVisualizerApp():
WallAppBase()
{
    
};

FFTVisualizerApp::~FFTVisualizerApp() {
    
};

void FFTVisualizerApp::setup(ofFbo* fbo_) {
    setFBO(fbo_);
    shapeColor = ofColor(255, 0, 0);
    bg = ofColor(ofRandom(255), ofRandom(255), ofRandom(255));
    bg.setSaturation(255);
    shapeHue = shapeColor.getHueAngle();
    hueInterval = 1.5;
    verticalInterval = 0.5;
    fft.setup();
    fft.setNormalize(true);
};

void FFTVisualizerApp::update() {
    ofPushStyle();
    fft.update();
    if (shapeHue >= 360) shapeHue = 0;
    shapeColor.setHueAngle(shapeHue +=  hueInterval);
    _draw(0, 0, getWidth(), getHeight());
    ofPopStyle();
};

void FFTVisualizerApp::onAppSwitch() {

};

void FFTVisualizerApp::_draw(int x, int y, int width, int height) {
    
    //    fbo->begin();
    //    ofSetColor(bg);
    //    ofRect(0, 0, getWidth(), getHeight());
    //    ofSetColor(255);
    //    fbo->end();
    
    ofPushStyle();
    ofSetColor(bg);
    ofRect(x, y, width, height);
    ofSetColor(255);
    fbo->draw(x, y - verticalInterval);
    _drawShape(x, y, width, height);
    
    ofTexture tex;
    tex.allocate(width, height, GL_RGB, 4);
    tex.loadScreenData(x, y, width, height);
    fbo->begin();
    tex.draw(x, y, width, height);
    fbo->end();
    ofPopStyle();
};

void FFTVisualizerApp::_drawBins() {
    float numFFTBins = fft.getNumFFTbins();
    ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetLineWidth(2);
    const vector<float>& fftSpectrum = fft.getFFTSpectrum();
    for(int i=0; i < numFFTBins; i++){ //for the number of columns
        ofSetColor(255,100,200);
        ofNoFill();
        ofRect(ofGetWidth()*((float)i/numFFTBins),
               ofGetHeight()-20,
               ofGetWidth()/numFFTBins,
               -ofMap(fftSpectrum[i], 0, 1, 0, ofGetHeight() -50));
    }
    ofPopStyle();
    
}

void FFTVisualizerApp::_drawShape(int x, int y, int width, int height) {
    
    float numFFTBins = fft.getNumFFTbins();
    ofPushStyle();
    ofSetRectMode(OF_RECTMODE_CORNER);
    ofSetLineWidth(2);
    const vector<float>& fftSpectrum = fft.getFFTSpectrum();
    
    ofPath path;
    shapeColor.setSaturation(127);
    path.setFillColor(shapeColor);
    path.setFilled(true);
    path.moveTo(width, height);
    path.lineTo(x, width);
    for(int i = 0; i < numFFTBins; i++){ //for the number of columns
        ofSetColor(100,100,200);
        ofFill();
        if (i == 0) {
            path.lineTo(x, height - ofMap(fftSpectrum[i], 0, 1, 0, height));
        }
        ofPoint point(width * ((float)i/numFFTBins),
                      height - ofMap(fftSpectrum[i], 0, 1, 0, width));
        path.curveTo(point);
        if (i == numFFTBins - 1) {
            path.lineTo(width, height - ofMap(fftSpectrum[i], 0, 1, 0, height));
        }
    }
    path.draw(x, y);
    ofPopStyle();
}