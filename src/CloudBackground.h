#pragma once

#include "ofMain.h"

class CloudBackground
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
        camera.setGlobalPosition( ofVec3f(0.00326601, 4.02035, 0.311073) );
        camera.lookAt( ofVec3f(0,0,0) );
        camera.setNearClip( 0.0001f );
        camera.setFarClip( 1024 );
        fbo.allocate(width, height);
        fbo.begin();
            ofClear(0, 0, 0, 0);
        fbo.end();
        
    }
    
    void update()
    {
        frequency+=0.00051;
    }
    
    void draw()
    {
        ofPushStyle();
            fbo.begin();
                ofClear(ofColor::white);
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
                
                camera.end();
            fbo.end();
        ofPopStyle();

        fbo.draw(0, 0);
    }
    
    
    
    
    
    
    
};
