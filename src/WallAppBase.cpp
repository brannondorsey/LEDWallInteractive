#include "WallAppBase.h"


WallAppBase::WallAppBase()
{
    fbo = NULL;
    name = "WallAppBase";
}

void WallAppBase::setFBO(ofFbo* fbo_)
{
    fbo = fbo_;

}

float WallAppBase::getWidth()
{
    return fbo->getWidth();
}

float WallAppBase::getHeight()
{
    return fbo->getHeight();
}

void WallAppBase::draw()
{
    draw(0, 0, getWidth(), getHeight());
}

void WallAppBase::draw(int x, int y, int width, int height)
{
    fbo->draw(x, y, width, height);
}



ofPixels& WallAppBase::getPixelsRef()
{
    fbo->readToPixels(pixels);
    return pixels;
}

ofColor WallAppBase::getRandomColor()
{
    return ofColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255));
}

void WallAppBase::drawGradientMesh(
                               const ofColor& start,
                               const ofColor& end,
                               ofGradientMode mode,
                               float w,
                               float h)
{
    ofVboMesh gradientMesh;
    gradientMesh.clear();
    gradientMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
    gradientMesh.setUsage(GL_STREAM_DRAW);
    switch (mode)
    {
            
        case OF_GRADIENT_CIRCULAR :
        {
            // this could be optimized by building a single mesh once, then copying
            // it and just adding the colors whenever the function is called.
            ofVec2f center(w / 2, h / 2);
            gradientMesh.addVertex(center);
            gradientMesh.addColor(start);
            int n = 32; // circular gradient resolution
            float angleBisector = TWO_PI / (n * 2);
            float smallRadius = ofDist(0, 0, w / 2, h / 2);
            float bigRadius = smallRadius / cos(angleBisector);
            for(int i = 0; i <= n; i++)
            {
                float theta = i * TWO_PI / n;
                gradientMesh.addVertex(center + ofVec2f(sin(theta), cos(theta)) * bigRadius);
                gradientMesh.addColor(end);
            }
            break;
        }
        case OF_GRADIENT_LINEAR:
        {
            gradientMesh.addVertex(ofVec2f(0, 0));
            gradientMesh.addVertex(ofVec2f(w, 0));
            gradientMesh.addVertex(ofVec2f(w, h));
            gradientMesh.addVertex(ofVec2f(0, h));
            gradientMesh.addColor(start);
            gradientMesh.addColor(start);
            gradientMesh.addColor(end);
            gradientMesh.addColor(end);
            break;
        }
        case OF_GRADIENT_BAR:
        {
            gradientMesh.addVertex(ofVec2f(w / 2, h / 2));
            gradientMesh.addVertex(ofVec2f(0, h / 2));
            gradientMesh.addVertex(ofVec2f(0, 0));
            gradientMesh.addVertex(ofVec2f(w, 0));
            gradientMesh.addVertex(ofVec2f(w, h / 2));
            gradientMesh.addVertex(ofVec2f(w, h));
            gradientMesh.addVertex(ofVec2f(0, h));
            gradientMesh.addVertex(ofVec2f(0, h / 2));
            gradientMesh.addColor(start);
            gradientMesh.addColor(start);
            gradientMesh.addColor(end);
            gradientMesh.addColor(end);
            gradientMesh.addColor(start);
            gradientMesh.addColor(end);
            gradientMesh.addColor(end);
            gradientMesh.addColor(start);
            break;
        }
    }
    GLboolean depthMaskEnabled;
    glGetBooleanv(GL_DEPTH_WRITEMASK,&depthMaskEnabled);
    glDepthMask(GL_FALSE);
    gradientMesh.draw();
    if(depthMaskEnabled)
    {
        glDepthMask(GL_TRUE);
    }
   

}


void WallAppBase::drawLetters()
{
    map<char, int> leds;
    leds['E'] = 16;
    leds['D'] = 16;
    leds['C'] = 28;
    leds['B'] = 32;
    leds['A'] = 40;
    
    vector<string> rows;
    string sequence = "DEEDCBAABCDE";
    
    
    rows.push_back("EEDCBAABCDEE");
    rows.push_back("EDCBAABCDEED");
    rows.push_back("DCBAABCDEEDC");
    rows.push_back("CBAABCDEEDCB");
    rows.push_back("BAABCDEEDCBA");
    rows.push_back("AABCDEEDCBAA");
    rows.push_back("ABCDEEDCBAAB");
    rows.push_back("BCDEEDCBAABC");
    rows.push_back("CDEEDCBAABCD");
    rows.push_back("DEEDCBAABCDE");
    
    rows.push_back("EEDCBAABCDEE");
    rows.push_back("EDCBAABCDEED");
    rows.push_back("DCBAABCDEEDC");
    rows.push_back("CBAABCDEEDCB");
    rows.push_back("BAABCDEEDCBA");
    rows.push_back("AABCDEEDCBAA");
    rows.push_back("ABCDEEDCBAAB");
    rows.push_back("BCDEEDCBAABC");
    rows.push_back("CDEEDCBAABCD");
    rows.push_back("DEEDCBAABCDE");
    
    rows.push_back("EEDCBAABCDEE");
    rows.push_back("EDCBAABCDEED");
    rows.push_back("DCBAABCDEEDC");
    rows.push_back("CBAABCDEEDCB");
    
    
    
    ofPushMatrix();
    //ofTranslate(20, 20);
    for(int i=0; i<rows.size(); i++)
    {
        string row = rows[i];
        for(int j=0; j<row.size(); j++)
        {
            char letter = row[j];
            int numPixels = leds[letter];
            ofColor color = ofColor::black;
            switch (letter)
            {
                case 'A':
                {
                    color = ofColor::red;
                    break;
                }
                case 'B':
                {
                    //color = ofColor::blue;
                    break;
                }
                case 'C':
                {
                    //color = ofColor::green;
                    break;
                }
                case 'D':
                {
                    //color = ofColor::orange;
                    break;
                }
                case 'E':
                {
                    //color = ofColor::yellow;
                    break;
                }
                default:
                    break;
            }
            
            ofSetColor(color);
            int size =10;
            ofPushMatrix();
            ofTranslate(i*size, j*size);
            if(ofGetFrameNum()%10 == 0)
            {
                //drawGradient(ofColor::black, color, OF_GRADIENT_BAR, numPixels, size);
                
            }else
            {
                
            }
            ofRect(0, 0, size, size);
            //
            ofPopMatrix();
            
            
            
            for(int k=0; k<numPixels; k++)
            {
                //
                //ofSetColor(ofColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255)));
                
                //ofDrawBitmapString(ofToString(letter), i*20, j*20);
            }
        }
        
        
    }
    ofPopMatrix();
}

