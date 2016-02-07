/*
 *
 * Copyright (c) 2014, James Hurlbut
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in
 * the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of James Hurlbut nor the names of its
 * contributors may be used to endorse or promote products
 * derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include "ofMain.h"
#include "OPC.h"

#define MAGIC_NUMBER 34564
//#define MAGIC_NUMBER 32768
//48*12*60

void initPacket(vector<uint8_t>& packetData)
{
    int numBytes = MAGIC_NUMBER;
    int packetLen = 4 + numBytes;
    if (packetData.size() != packetLen)
    {
        packetData.resize(packetLen, 0);
        packetData[0] = 0;
        packetData[1] = 0;
        packetData[2] = (uint8_t)(numBytes >> 8);
        packetData[3] = (uint8_t)(numBytes & 0xFF);
    }
}

OPCClient::OPCClient()
{
    id = -1;
    host = "localhost";
    port =  7890;
    bSocketConnected = false;
    
}
void OPCClient::setup(const string &host_, int port_, int id_)
{
    initPacket(packet);
    host = host_;
    port = port_;
    id = id_;
    address = Poco::Net::SocketAddress(host, port);
    
}
OPCClient::~OPCClient( )
{
      dispose();
}

void OPCClient::update()
{

}
void OPCClient::dispose()
{
    if(bSocketConnected)
    {
        ofLog(OF_LOG_NOTICE, "ofxFadecandy: Disposing Socket Connection");
        try
        {
            socket.shutdown();
        }
        catch(Poco::Exception e)
        {
            ofLog(OF_LOG_ERROR, "ofxFadecandy: Poco net exception: ");
        }
        
        try
        {
            socket.close();
        }
        catch (Poco::Exception e)
        {
            cout << "caught close exception" << endl;
        };
            
        bSocketConnected = false;
    }
}

bool OPCClient::connect()
{
    if(!bSocketConnected)
    {
        try
        {
            socket = Poco::Net::StreamSocket(address);
            socket.setNoDelay(true);
            socket.setKeepAlive(true);
            socket.setBlocking(true);
            bSocketConnected = true;
            ofLogVerbose(__func__) << "ofxFadecandy: Socket Connection Established";
        }
        catch(Poco::Exception e)
        {
            bSocketConnected = false;
            ofLogError(__func__) << "ofxFadecandy: Socket Connection NOT Established";
        }
    }
    return bSocketConnected;
    
}

void OPCClient::processPacket(int& index, ofColor& color)
{
    for (int i = 0; i < 40; i++)
    {
        packet[index]     = color.r;
        packet[index + 1] = color.g;
        packet[index + 2] = color.b;
        
        index += 3;
    }
    
}



float OPCClient::largestTime = 0;

void OPCClient::write()
{
    float startTime = ofGetElapsedTimeMillis();
    
    if(connect())
    {
        try
        {
            int result = socket.sendBytes(&packet[0], (int) packet.size());
        }
        catch (Poco::Exception e)
        {
            ofLog(OF_LOG_ERROR, "ofxFadecandy: Error Sending Pixels");
        }

    }
    float endTime = ofGetElapsedTimeMillis();
    float timeElapsed = (endTime - startTime);
    if(timeElapsed > OPCClient::largestTime)
    {
        largestTime = timeElapsed;
        ofLogVerbose(__func__) << "TOOK " << (endTime - startTime) << "MS LARGEST IS " << OPCClient::largestTime;
    }
//    ofSleepMillis(25);
}

bool OPCClient::isConnected()
{
    return bSocketConnected;
}
