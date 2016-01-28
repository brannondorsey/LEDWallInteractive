//
//  OpenCVParticleApp.cpp
//  LEDWallVideoPlayer
//
//  Created by Jason Van Cleave on 9/20/15.
//
//

#include "OpenCVParticleApp.h"

void OpenCVParticleApp::setup(ofFbo* fbo_)
{
    setFBO(fbo_);
    doUpdateBackground = true;
    cloudBackground.setup(ofGetWidth(), ofGetHeight());
    
    OpenCVEngine::getInstance().setup();

    singleWallWidth = OpenCVEngine::getInstance().videoSourceWidth;
    wholeWallWidth = OpenCVEngine::getInstance().canvasWidth;
    int wallHeight = OpenCVEngine::getInstance().canvasHeight;
    
    particleLayer = NULL;
    
    particleLayer = new ParticleLayer();
    
    backgroundFBO.allocate(wholeWallWidth, wallHeight);
    backgroundFBO.begin();
    ofClear(0, 0, 0, 0);
    backgroundFBO.end();
    particleSize = 400;
    particleLayer->setup(particleSize);
    
    leftWall.id = 0;

    leftWall.wall = OpenCVEngine::getInstance().getWall(0);
    
    rightWall.id = 1;
    rightWall.wall = OpenCVEngine::getInstance().getWall(1);
    
    for (int x =0; x<backgroundFBO.getWidth(); x+=particleSize)
    {
        for (int y =0; y<backgroundFBO.getHeight(); y+=particleSize)
        {
            ofVec3f particleOrigin(x, y, 0);
            ofxParticle* particle = particleLayer->addParticle(particleOrigin);
            if (particleOrigin.x<=singleWallWidth)
            {
                leftWall.particles.push_back(particle);
            }else
            {
                if (particleOrigin.x>=singleWallWidth)
                {
                    rightWall.particles.push_back(particle);
                }else{
                
                    ofLogVerbose() << "particleOrigin: " << particleOrigin << " NOT REGISTERING";
                }
            }
            
        }
    }
    wallGroups.push_back(&leftWall);
    wallGroups.push_back(&rightWall);

    
    blobCounter = 0;

    textureSequence.loadImages("particle-scale");
    
    ColorPair c1; //1.jpg
    c1.color1 = ofColor(255, 22, 93); //red
    c1.color2 = ofColor(171, 255, 99); //green
    
    ColorPair c2; //2.jpg
    c2.color1 = ofColor::red;
    c2.color2 = ofColor::blue;
    
    ColorPair c3; //3.jpg
    c3.color1 = ofColor(229, 255, 86); //lime
    c3.color2 = ofColor(0, 255, 234); //teal
    
    ColorPair c4; //4.jpg
    c4.color1 = ofColor(255, 137, 87); //orange
    c4.color2 = ofColor(223, 87, 255); //purple
    
    colorPairs.push_back(c1);
    colorPairs.push_back(c2);
    colorPairs.push_back(c3);
    colorPairs.push_back(c4);

    currentColorPairIndex = ofRandom(colorPairs.size());


}

void OpenCVParticleApp::updateParticles()
{
    float attractionStrength = 10.0f;
    for (int i=0; i<wallGroups.size(); i++)
    {
        ofPolyline& mainBlob = wallGroups[i]->wall->mainBlob;
        vector<ofxParticle*>& currentParticles = wallGroups[i]->particles;
        if (abs(mainBlob.getArea()))
        {
            for (int j=0; j<currentParticles.size(); j++)
            {
                ofVec3f particleTarget;
                
                if(j%2 ==0)
                {
                    particleTarget = mainBlob.getBoundingBox().getPosition();
                }else
                {
                    particleTarget = mainBlob.getCentroid2D();
                }
                particleTarget = mainBlob.getBoundingBox().getPosition();
                if (wallGroups[i]->id == 1)
                {
                     particleTarget.x+=(singleWallWidth*wallGroups[i]->id);
                }
                particleTarget.y = 0;
                
                currentParticles[j]->moveTo(particleTarget);
            }
        }else
        {
            for (int j=0; j<currentParticles.size(); j++)
            {
                currentParticles[j]->moveTowards(currentParticles[j]->origin, 0.25f * (float)j);
            }
        }
    }
    particleLayer->update();
}





void OpenCVParticleApp::update()
{
    cloudBackground.update();
   
    OpenCVEngine::getInstance().update();

    updateParticles();
    
    backgroundFBO.begin();
        ofClear(0, 0, 0, 0);
        drawBackground();
        cloudBackground.draw();
    backgroundFBO.end();
    
    //Draw at full res
    OpenCVEngine::getInstance().contentLayerFBO.begin();
    ofClear(0, 0, 0, 0);
    
    ofPushStyle();
    
    for (int i=0; i<particleLayer->particles.size(); i++)
    {
        ofPushMatrix();
        ofTranslate(particleLayer->particles[i]->x,
                    particleLayer->particles[i]->y);
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        textureSequence.textures[textureSequence.currentFrame].draw(0, 0, particleSize, particleSize*2);

        if (textureSequence.currentFrame+1<textureSequence.textures.size())
        {
            textureSequence.currentFrame++;
        }else
        {
            textureSequence.currentFrame = 0;
        }
        ofPopMatrix();
        ofDisableBlendMode();
        
        ofPopMatrix();
    }
    
    ofPopStyle();
    OpenCVEngine::getInstance().contentLayerFBO.end();
    
    fbo->begin();
    ofPushStyle();
    
    backgroundFBO.draw(0, 0);
    OpenCVEngine::getInstance().contentLayerFBO.draw(0,
                                                     0,
                                                     480,
                                                     120);
    ofPopStyle();
    
    fbo->end();
    
    

}
void OpenCVParticleApp::onAppSwitch()
{
    currentColorPairIndex = ofRandom(colorPairs.size());
}

void OpenCVParticleApp::drawBackground()
{

    drawGradientMesh(colorPairs[currentColorPairIndex].color1,
                     colorPairs[currentColorPairIndex].color2,
                     OF_GRADIENT_BAR,
                     ofGetWidth(),
                     ofGetHeight());
}


