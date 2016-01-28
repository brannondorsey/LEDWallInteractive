
#pragma once
#include "WallAppBase.h"
#include "OpenCVEngine.h"

#include "ParticleLayer.h"
#include "ParticleAppCloudBackground.h"

struct WallParticleGroup2
{
    vector<ofxParticle*> particles;
    TrackedWall* wall;
    int id;
    WallParticleGroup2()
    {
        id = -1;
        wall = NULL;
        
    }
};
struct TextureSequence
{
    void loadImages(string folderName)
    {
        ofDirectory dir(ofToDataPath(folderName, true));
        if (dir.exists())
        {
            dir.listDir();
            dir.sort();
            vector<ofFile> files = dir.getFiles();
            for(int i=0; i<files.size(); i++)
            {
                ofLogVerbose() << files[i].getFileName();
                ofTexture texture;
                bool didLoad = ofLoadImage(texture, files[i].getAbsolutePath());
                //ofLogVerbose() << files[i].getFileName() << " LOAD STATUS " << didLoad;
                textures.push_back(texture);
            }
            currentFrame = 0;
        }
        
    };
    vector<ofTexture> textures;
    int    currentFrame;
    //particle-scale_00000.png
};

struct ColorPair
{
    ofColor color1;
    ofColor color2;
};

class OpenCVParticleApp : public WallAppBase
{
public:
    
    void setup(ofFbo* fbo_);
    void update();
    TextureSequence textureSequence;
   
    bool doUpdateBackground;
        
    
    ParticleAppCloudBackground cloudBackground;
    
    ParticleLayer* particleLayer;
    int particleSize;
    void updateParticles();
    
    ofFbo backgroundFBO;

    ofTexture particleTexture;
    void drawBackground();
    
    int blobCounter;
    
    WallParticleGroup2 leftWall;
    WallParticleGroup2 rightWall;
    vector<WallParticleGroup2*> wallGroups;
    
    void onAppSwitch();
    
    int singleWallWidth;
    int wholeWallWidth;
    vector<ColorPair> colorPairs;
    int currentColorPairIndex;
};
