//
//  FadeCandyController.h
// 
//  Created by Brannon Dorsey
//
//

#pragma once
#include "ofMain.h"
#include "OPC.h"

class FadeCandyController{
public:
    FadeCandyController();
    ~FadeCandyController();
    void setup(vector<string>);
    
    void update(ofPixels& pixels);
    
    bool allClientsConnected();
    bool connect();
    
    OPCClient* clientOne;
    OPCClient* clientTwo;
    OPCClient* clientThree;
    OPCClient* clientFour;
    
    vector<OPCClient*> clients;
};
