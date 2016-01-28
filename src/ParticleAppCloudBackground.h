#pragma once

#include "ofMain.h"

class ParticleAppCloudBackground
{
public:
    
    float frequency;
    ofColor cloudColor;
    
    ofEasyCam camera;
    ofShader shader;
    ofImage noiseImage;
    
    ofFbo fbo;
    int width;
    int height;
    int minFrequency;
    int maxFrequency;
    // --------------------------------
    void setup(int width_, int height_)
    {
        minFrequency = 3.0f;
        maxFrequency = 7.0f;
        width  = width_;
        height = height_;
        ofSetLogLevel(OF_LOG_VERBOSE);
        
        frequency = minFrequency;
        cloudColor = ofColor::black;
        
        
        shader.load("RayMarchingCloudsVolumeofCamera");
        
        ofDisableArbTex();
            noiseImage.loadImage("NoiseForClouds.png");
            noiseImage.getTextureReference().setTextureWrap( GL_REPEAT, GL_REPEAT );
        ofEnableArbTex();
        
        camera.setAutoDistance( false );
        camera.setGlobalPosition( ofVec3f(-0.550536, 3.9944, 0.0407352) );
        camera.lookAt( ofVec3f(0,0,0) );
        camera.setNearClip( 0.0001f );
        camera.setFarClip( 1024 );
        fbo.allocate(width, height);
        fbo.begin();
            ofClear(0, 0, 0, 0);
        fbo.end();
        
    }
    
    
    // --------------------------------
    void update()
    {
        frequency+=0.00051;

        //frequency+=0.01;
        //ofLogVerbose() << "frequency: " << frequency;
        //frequency = ofRandom(2.5, 5);
    }
    
    // --------------------------------
    void draw()
    {
        ofPushStyle();
       
        fbo.begin();
        ofClear(0, 0, 0, 0);
        camera.begin();
        
        ofEnableAlphaBlending();
        
        shader.begin();
        
        shader.setUniform1f("aspect", (float)width / height );
        shader.setUniform1f("fovYScale", tan( ofDegToRad(camera.getFov())/2 ) );
        
        shader.setUniform1f("time", ofGetElapsedTimef() );
        shader.setUniform2f("resolution", width*2, height*2 );
        shader.setUniformTexture("noiseTexture", noiseImage, 0 );
        shader.setUniform1f("frequency", frequency );
        
        ofFloatColor cloudBaseColor = cloudColor;
        shader.setUniform4fv("cloudBaseColor", cloudBaseColor.v );
        
        ofRect( -1, -1, 2, 2 );
        
        shader.end();
        
        //ofDisableDepthTest();
        //ofDrawBox( 0.5f );
        
        camera.end();
        fbo.end();
        
        //ofSetColor( ofColor::black );
        
        
        ofPopStyle();

        fbo.draw(0, 0);
    }
    
    
    
    
    
    
    
};
