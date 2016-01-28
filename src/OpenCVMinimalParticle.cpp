//
//  OpenCVMinimalParticle.cpp
//  LEDWallVideoPlayer
//
//  Created by Jason Van Cleave on 9/20/15.
//
//

#include "OpenCVMinimalParticle.h"

void OpenCVMinimalParticle::setup(ofFbo* fbo_)
{
    setFBO(fbo_);
    doUpdateBackground = true;
    
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
    
    ofLoadImage(particleTexture, "dot.png");


}

void OpenCVMinimalParticle::updateParticles()
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
                    //
                }else
                {
                    particleTarget = mainBlob.getCentroid2D();
                }

                if (wallGroups[i]->id == 1)
                {
                     particleTarget.x+=(singleWallWidth*wallGroups[i]->id);
                }else
                {
                   particleTarget.x-=40;
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





void OpenCVMinimalParticle::update()
{
   
    OpenCVEngine::getInstance().update();

    updateParticles();
    
    
    backgroundFBO.begin();
        ofClear(0, 0, 0, 0);
        drawBackground();
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
        particleTexture.draw(0, -100, 120, 1000);
        ofPopMatrix();
        ofDisableBlendMode();
        
        ofPopMatrix();
    }
    
    ofPopStyle();
    OpenCVEngine::getInstance().contentLayerFBO.end();
    
    fbo->begin();
    ofPushStyle();
    
    //backgroundFBO.draw(0, 0);
    OpenCVEngine::getInstance().contentLayerFBO.draw(0,
                                                     0,
                                                     480,
                                                     120);
    ofPopStyle();
    
    fbo->end();
    
    

}
void OpenCVMinimalParticle::onAppSwitch()
{

}

void OpenCVMinimalParticle::drawBackground()
{
#if 0
    drawGradientMesh(colorPairs[currentColorPairIndex].color1,
                     colorPairs[currentColorPairIndex].color2,
                     OF_GRADIENT_BAR,
                     ofGetWidth(),
                     ofGetHeight());
#endif
}


