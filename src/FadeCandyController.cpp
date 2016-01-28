//
//  FadeCandyController.cpp
//
//  Created by Brannon Dorsey
//
//

#include "FadeCandyController.h"



FadeCandyController::FadeCandyController()
{
    clientOne = NULL;
    clientTwo = NULL;
    clientThree = NULL;
    clientFour = NULL;
    
}


FadeCandyController::~FadeCandyController()
{
    
    for(size_t i=0; i<clients.size(); i++)
    {
        delete clients[i];
    }
    clients.clear();
    clientOne = NULL;
    clientTwo = NULL;
    clientThree = NULL;
    clientFour = NULL;
}

void FadeCandyController::setup(vector<string> hosts)
{
    
    for(int i=0; i<hosts.size(); i++)
    {
        OPCClient* client = new OPCClient();
        client->setup(hosts[i], 7890, i);
        clients.push_back(client);
    }
    
    
    for(int i=0; i<clients.size(); i++)
    {
        switch (clients[i]->id)
        {
            case 0:
            {
                clientOne =clients[i];
                break;
            }
            case 1:
            {
                clientTwo =clients[i];
                break;
            }
            case 2:
            {
                clientThree =clients[i];
                break;
            }
            case 3:
            {
                clientFour =clients[i];
                break;
            }
                
            default:
            {
                break;
            }
        }
    }
    
}

bool FadeCandyController::connect()
{
    
    for(size_t i=0; i<clients.size(); i++)
    {
        if (!clients[i]->isConnected())
        {
            clients[i]->connect();
        }
    }
    return allClientsConnected();
}





ofColor getRandomColor()
{
    return ofColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255));
}



void FadeCandyController::update(ofPixels& pixels){

    int totalStripsX = 48;
    int totalStripsY = 24;
    int xStep = pixels.getWidth()/totalStripsX;
    int yStep = pixels.getHeight()/totalStripsY;
    
    int indexOne = 4;
    int indexTwo = 4;
    int indexThree = 4;
    int indexFour = 4;
    
    for (int y = 0; y < pixels.getHeight(); y += yStep)
    {
        if(clientOne || clientTwo)
        {
            for (int x = 0; x < pixels.getWidth() * 0.5; x += xStep)
            {
                ofColor color = pixels.getColor(x, y);
                float xLimit =pixels.getWidth() * 0.25;
                
                if (x >= xLimit)
                {
                    if(clientTwo)
                    {
                       clientTwo->processPacket(indexTwo, color);
                    }
                } else
                {
                    if(clientOne)
                    {
                        clientOne->processPacket(indexOne, color);
                    }
                }
            }
        }
        if(clientThree || clientFour)
        {
            for (int x = pixels.getWidth() * 0.5; x < pixels.getWidth(); x += xStep)
            {
                
                ofColor color = pixels.getColor(x, y);
                if (x >= pixels.getWidth() * 0.75)
                {
                    if (clientThree)
                    {
                        clientThree->processPacket(indexThree, color);
                    }
                    
                } else
                {
                    
                    if(clientFour)
                    {
                        clientFour->processPacket(indexFour, color);
                    }
                    
                }
            }
        }
        
    }
    
    for(size_t i=0; i<clients.size(); i++)
    {
        clients[i]->write();
    }
    
}

bool FadeCandyController::allClientsConnected()
{
    bool result = true;
    for(size_t i=0; i<clients.size(); i++)
    {
        if(!clients[i]->isConnected())
        {
            //ofLogVerbose(__func__) << "client ID: " << i << " FAILED";
            result = false;
            break;
        }
    }
    return result;
}
