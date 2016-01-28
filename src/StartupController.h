#pragma once

class StartupController :public ofThread
{
public:
    bool isReady;
    stringstream statusLog;
    StartupController()
    {
        isReady = false;
        statusLog << "STATUS LOG: " << endl;
    }
    
    void setup(bool skipCheck=false)
    {
        if(!skipCheck)
        {
            startThread(true);
        }else
        {
            isReady = true;
        }
        
    }
    
    bool checkForFadeCandyServer(string ipStart, vector<string>& failedIPs, vector<string>& successfulIPs)
    {
        bool areAllAvailable = false;
        
        for (int i = 0; i<4; i++)
        {
            string ip = ipStart+ofToString(i);
            
            string response = "";
            char   commandResponse[128];
            FILE   *pipe;
            string command = "ssh -i ~/.ssh/id_rsa pi@"+ip+" 'echo raspberry | sudo ps | grep fcserver'";
            statusLog << "COMMAND: " << command << endl;
            pipe = popen( command.c_str(), "r+" );
            
            if( pipe )
            {
                while(fgets(commandResponse, 128, pipe))
                {
                    ofLogVerbose() << "commandResponse: " << commandResponse;
                    response+=ofToString(commandResponse);
                    statusLog << response << endl;
                }
            }else
            {
                ofLogVerbose() << "NO PIPE";
            }
            bool didFail = true;
            if(!response.empty())
            {
                if(ofIsStringInString(response, "fcserver"))
                {
                    didFail = false;
                }
            }
            if(didFail)
            {
                failedIPs.push_back(ip);
            }else
            {
                successfulIPs.push_back(ip);
            }
        }
        areAllAvailable = failedIPs.empty();
        return areAllAvailable;
    }
    
    bool pingRPis(string ipStart, vector<string>& failedIPs, vector<string>& successfulIPs)
    {
        bool areAllAvailable = false;
        
        for (int i = 0; i<4; i++)
        {
            string ip = ipStart+ofToString(i);
            
            string response = "";
            char   commandResponse[128];
            FILE   *pipe;
            string command = "ping -c 2 "+ ip;
            statusLog << "COMMAND: " << command << endl;
            
            pipe = popen( command.c_str(), "r+" );
            
            if( pipe )
            {
                while(fgets(commandResponse, 128, pipe))
                {
                    ofLogVerbose() << "commandResponse: " << commandResponse;
                    response+=ofToString(commandResponse);
                    statusLog << response << endl;
                    
                }
            }else
            {
                ofLogVerbose() << "NO PIPE";
            }
            bool didFail = ofIsStringInString(response, "100.0% packet loss");
            if(didFail)
            {
                failedIPs.push_back(ip);
            }else
            {
                successfulIPs.push_back(ip);
            }
        }
        areAllAvailable = failedIPs.empty();
        return areAllAvailable;
    }
    
    void rebootRpis(vector<string>& ips)
    {
        for (int i = 0; i<ips.size(); i++)
        {
            string ip = ips[i];
            
            string response = "";
            char   commandResponse[128];
            FILE   *pipe;
            string command = "ssh -i ~/.ssh/id_rsa pi@"+ip+" 'echo raspberry | sudo reboot'";
            statusLog << "COMMAND: " << command << endl;
            pipe = popen( command.c_str(), "r+" );
            
            if( pipe )
            {
                while(fgets(commandResponse, 128, pipe))
                {
                    ofLogVerbose() << "commandResponse: " << commandResponse;
                    response+=ofToString(commandResponse);
                    statusLog << response << endl;
                    
                }
            }else
            {
                ofLogVerbose() << "NO PIPE";
            }
            
        }
    }
    
    void killAllFCservers(vector<string>& ips)
    {
        for (int i = 0; i<ips.size(); i++)
        {
            string ip = ips[i];
            
            string response = "";
            char   commandResponse[128];
            FILE   *pipe;
            string command = "ssh -i ~/.ssh/id_rsa pi@"+ip+" 'echo raspberry | sudo killall fcserver'";
            statusLog << "COMMAND: " << command << endl;
            pipe = popen( command.c_str(), "r+" );
            
            if( pipe )
            {
                while(fgets(commandResponse, 128, pipe))
                {
                    ofLogVerbose() << "commandResponse: " << commandResponse;
                    response+=ofToString(commandResponse);
                    statusLog << response << endl;
                    
                }
            }else
            {
                ofLogVerbose() << "NO PIPE";
            }
            bool didKill = false;
            if(!response.empty())
            {
                if(ofIsStringInString(response, "no process found"))
                {
                    didKill = true;
                    ips.erase(ips.begin(), ips.begin()+i);
                }
            }
        }
    }
    
    void threadedFunction()
    {
        while (isThreadRunning())
        {
            string ipStart = "192.168.2.10";
        
            vector<string> pis;
            for (int i = 0; i<4; i++)
            {
                string ip = ipStart+ofToString(i);
                pis.push_back(ip);
            }
            rebootRpis(pis);
            sleep(1000*120);
            
            vector<string> failedFadeCandyIPs;
            vector<string> successfulFadeCandyIPs;
            checkForFadeCandyServer(ipStart, failedFadeCandyIPs, successfulFadeCandyIPs);
            for(int i=0; i<failedFadeCandyIPs.size(); i++)
            {
                ofLogVerbose() << "failedFadeCandyIP: " << failedFadeCandyIPs[i];
            }
            for(int i=0; i<successfulFadeCandyIPs.size(); i++)
            {
                ofLogVerbose() << "successfulFadeCandyIP : " << successfulFadeCandyIPs[i];
            }
            
            isReady = failedFadeCandyIPs.empty();
            statusLog << "STOPPING THREAD: " << endl;
            
            stopThread();
        }
    }
};
