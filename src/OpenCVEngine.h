#pragma once
#include "ofMain.h"
#include "TrackedWall.h"

#include <arpa/inet.h>
#include <net/if.h>
#include <ifaddrs.h>

class LocalAddressGrabber
{
public:
    
    static vector<string>& availableList(void){
        struct ifaddrs *myaddrs;
        struct ifaddrs *ifa;
        struct sockaddr_in *s4;
        int status;
        
        char buf[64];
        
        vector<string> interfaces;
        
        status = getifaddrs(&myaddrs);
        if (status != 0)
        {
            perror("getifaddrs");
            
        }
        
        for (ifa = myaddrs; ifa != NULL; ifa = ifa->ifa_next)
        {
            if (ifa->ifa_addr == NULL) continue;
            if ((ifa->ifa_flags & IFF_UP) == 0) continue;
            
            if (ifa->ifa_addr->sa_family == AF_INET)
            {
                s4 = (struct sockaddr_in *)(ifa->ifa_addr);
                if (inet_ntop(ifa->ifa_addr->sa_family, (void *)&(s4->sin_addr), buf, sizeof(buf)) == NULL)
                {
                    printf("%s: inet_ntop failed!\n", ifa->ifa_name);
                }
                else
                {
                    ofLogNotice() << ifa -> ifa_name;
                    interfaces.push_back(ifa -> ifa_name);
                    
                }
            }
        }
        
        freeifaddrs(myaddrs);
        
    }
    
    static string getIpAddress(const string& interface)
    {
        string output = "NOT FOUND";
        struct ifaddrs *myaddrs;
        struct ifaddrs *ifa;
        struct sockaddr_in *s4;
        int status;
        
        char buf[64];
        
        status = getifaddrs(&myaddrs);
        if (status != 0)
        {
            perror("getifaddrs");
            
        }
        
        for (ifa = myaddrs; ifa != NULL; ifa = ifa->ifa_next)
        {
            if (ifa->ifa_addr == NULL) continue;
            if ((ifa->ifa_flags & IFF_UP) == 0) continue;
            
            if (ifa->ifa_addr->sa_family == AF_INET)
            {
                s4 = (struct sockaddr_in *)(ifa->ifa_addr);
                if (inet_ntop(ifa->ifa_addr->sa_family, (void *)&(s4->sin_addr), buf, sizeof(buf)) == NULL)
                {
                    printf("%s: inet_ntop failed!\n", ifa->ifa_name);
                }
                else
                {
                    if(ofToString(ifa->ifa_name) == interface)
                    {
                        freeifaddrs(myaddrs);
                        return ofToString(buf);
                    }
                }
            }
        }
        
        ofLogWarning() << output;	
        freeifaddrs(myaddrs);
        return output;
    }
};

class OpenCVEngine
{
public:
    static OpenCVEngine& getInstance()
    {
        static OpenCVEngine    instance;
        return instance;
    }
    
    vector<TrackedWall*> trackedWalls;
    ofFbo contentLayerFBO;
    int canvasWidth;
    int canvasHeight;
    int videoSourceWidth;
    int videoSourceHeight;
    
    void clearTrackedWallFBOs()
    {
        for(int i=0; i<trackedWalls.size(); i++)
        {
            if(trackedWalls[i])
            {
                if(trackedWalls[i]->fbo.isAllocated())
                {
                    trackedWalls[i]->fbo.begin();
                    ofClear(0, 0, 0, 0);
                    trackedWalls[i]->fbo.end();
                }
            }
        }
    }

    TrackedWall* getWall(int id)
    {
        TrackedWall* wall = NULL;
        for (int i = 0; i<trackedWalls.size(); i++)
        {
            if(trackedWalls[i]->id == id)
            {
                wall = trackedWalls[i];
                break;
            }
        }
        return wall;

    }
    void setup(bool useMovie = false)
    {
        
        LocalAddressGrabber :: availableList();
        string localIP = LocalAddressGrabber :: getIpAddress("en2");
        if(localIP != "192.168.2.200")
        {
            useMovie = true;
        }
        
//        useMovie = false;
        if(trackedWalls.empty())
        {
            string moviePath = "";
            if(useMovie)
            {
                moviePath = "Wall_Pablo_straight.mov";
            }
            TrackedWall* wall1 = new TrackedWall(0);
            wall1->setup(moviePath);
            trackedWalls.push_back(wall1);
            
            TrackedWall* wall2 = new TrackedWall(1);
            wall2->setup(moviePath);
            
            if(useMovie)
            {
               wall2->videoSource->videoPlayer.setLoopState(OF_LOOP_PALINDROME);
            }
#if 0
            if(wall2->videoSource->camera.eye->deviceAddress == wall1->videoSource->camera.eye->deviceAddress)
            {

            }else
            {
                ofLogVerbose() << "Can switch";
            }
            
#endif
            //wall2->videoSource.doRandom = true;
            //wall2->doUpdateBackground = false;
            //wall2->videoSource.videoPlayer.setLoopState(OF_LOOP_PALINDROME);
            trackedWalls.push_back(wall2);
            
            contentLayerFBO.allocate(canvasWidth, canvasHeight);
            contentLayerFBO.begin();
                ofClear(0, 0, 0, 0);
            contentLayerFBO.end();
        }else
        {
            ofLogVerbose(__func__) << "ALREADY SETUP";
        }
    }
    
    void swapCameras()
    {
        if(trackedWalls.size() == 2)
        {
            VideoSource* tempVideoSource = trackedWalls[0]->videoSource;
            VideoSource* tempVideoSource2 = trackedWalls[1]->videoSource;
            trackedWalls[0]->videoSource = tempVideoSource;
            trackedWalls[1]->videoSource = tempVideoSource2;
        }

    }
    
    void update()
    {
        for (int i=0; i<trackedWalls.size(); i++)
        {
            trackedWalls[i]->update();
        }
    }
    
    
private:
    OpenCVEngine()
    {
        videoSourceWidth = 640;
        videoSourceHeight = 480;
        
        canvasWidth = videoSourceWidth*2;
        canvasHeight = videoSourceHeight;
    }
    
    
    ~OpenCVEngine() {};
    OpenCVEngine(OpenCVEngine const&);
    void operator=(OpenCVEngine const&);
};


