//
//  dpSyphonClientManager.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2016/01/24.
//  Copyright (c) 2016年 YCAMInterlab. All rights reserved.
//

#ifndef RAMDanceToolkit_dpSyphonClientManager_h
#define RAMDanceToolkit_dpSyphonClientManager_h

#include "ofxSyphon.h"

class dpSyphonClientManager{
public:
    
    static dpSyphonClientManager& instance()
    {
        if (__instance == NULL)
            __instance = new dpSyphonClientManager;
        return *__instance;
    }
    
    void setup();
    void update();
    
    void draw(ofPoint pos, ofPoint size, int clientNum);
    void draw(float x, float y, float width, float height, int clientNum);
    void draw(float x, float y, float width, float height);

    void drawWithSideCrop();
    void drawWithSideCrop(float x, float y);
    
private:
    
    map<string, ofPtr<ofxSyphonClient> > mClients;
    ofxSyphonServerDirectory mServerDirectory;
    
    static dpSyphonClientManager *__instance;
    
    dpSyphonClientManager() {};
    dpSyphonClientManager(const dpSyphonClientManager&){}
    dpSyphonClientManager& operator=(const dpSyphonClientManager&){return *__instance;}
    ~dpSyphonClientManager();
  
    bool empty();
    const ofPtr<ofxSyphonClient> & getClient(int clientNum);
    void serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg);
    void serverRetired(ofxSyphonServerDirectoryEventArgs &arg);
};

#endif
