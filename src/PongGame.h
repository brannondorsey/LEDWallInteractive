#pragma once
#include "ofMain.h"

class PongBall : public ofRectangle
{
public:
    int speedX;
    int speedY;
    
    int heightLimit;
    int widthLimit;
    ofColor color;
    PongBall()
    {
        x = 0;
        y = 0;
        width = height = 50;
        speedX = 30;
        speedY = 30;
        color = ofColor::white;
    }
    void setup(int x_, int y_)
    {
        x = x_;
        y = y_;
    }
    
    void update()
    {
        x += speedX;
        y += speedY;
        if(y - 5 < 0)
        {
            y = 5;
            speedY = -speedY;
        }
        else
        {
            if(y + 5 > heightLimit)
            {
                y = heightLimit-5;
                speedY = -speedY;
            }
        }
        
        if(x < 0 || x > widthLimit)
        {
            //ofLogVerbose() << "x: " << x << " y: " << y << " getTopRight: " <<getTopRight();
            //ofLogVerbose() << "widthLimit: " << widthLimit;
            speedY = 30;
            speedX = 30;
            x = widthLimit/2;
            y = heightLimit/2;
        }
    }
    string getPos()
    {
        stringstream ss;
        ss << "x: " << x << endl;
        ss << "y: " << y << endl;
        ss << "speedX: " << speedX << endl;
        ss << "speedY: " << speedY << endl;
        ss << "widthLimit: " << widthLimit << endl;
        return ss.str();
    }
    void draw()
    {
        ofPushStyle();
        ofSetColor(color);
        
        ofRect(x, y, width, height);
        ofPopStyle();
        //ofDrawBitmapString(getPos(), getTopRight());
    }
    
};
class Paddle : public ofRectangle
{
public:
    int speedY;
    int heightLimit;
    Paddle()
    {
        speedY = 0;
        heightLimit = 0;
    }
    void setup(int x_, int y_, int width_, int height_)
    {
        x = x_;
        y = y_;
        width = width_;
        height = height_;
    }
    void update(ofRectangle& rectangle)
    {
        
        x = rectangle.getCenter().x;
        y = rectangle.y;
        
    }
    
    string getPos()
    {
        stringstream ss;
        ss << "x: " << x << endl;
        ss << "y: " << y << endl;
        ss << "width: " << width << endl;
        ss << "height: " << height << endl;
        ss << "heightLimit: " << heightLimit << endl;
        return ss.str();
    }
    void draw()
    {
        ofPushStyle();
        ofSetColor(255, 255, 255, 220);
        ofRect(x, y, width, height);
        ofPopStyle();
        //ofDrawBitmapString(getPos(), getTopRight());
        
    }
};

class PongGame
{
public:
    
    PongBall ball;
    Paddle paddle1;
    Paddle paddle2;
    
    int width;
    int height;

    PongGame()
    {
        width = 0;
        height = 0;
    }
    void setup(int width_, int height_)
    {
        
        width = width_;
        height = height_;
        
        paddle1.setup(100, 10, 50, 200);
        paddle2.setup(width-100, 10, 50, 200);
        
        paddle1.heightLimit = height;
        paddle2.heightLimit = height;
        
        ball.widthLimit = width;
        ball.heightLimit = height;
        
        ball.setup(200, 200);
        
        
    }
    
    void update(ofRectangle paddle1Rect, ofRectangle paddle2Rect)
    {
        
        ball.update();
        paddle1.update(paddle1Rect);
        paddle2.update(paddle2Rect);
        if(ball.intersects(paddle1))
        {
            ball.speedX = 30;
            ball.speedY += (paddle1.speedY / 2);
            ball.x += ball.speedX;
            ball.color = ofColor::green;
        }
        
        if(ball.intersects(paddle2))
        {
            ball.speedX = -30;
            ball.speedY += (paddle2.speedY / 2);
            ball.x += ball.speedX;
            ball.color = ofColor::red;
        }
        
    }
    
    void draw()
    {
        
        ball.draw();
        paddle1.draw();
        paddle2.draw();
    }
};

