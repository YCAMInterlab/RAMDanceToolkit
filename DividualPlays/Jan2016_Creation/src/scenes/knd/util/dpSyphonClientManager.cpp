//
//  dpSyphonClientManager.cpp
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2016/01/24.
//  Copyright (c) 2016年 YCAMInterlab. All rights reserved.
//

#include "dpSyphonClientManager.h"
#include "dpConstants.h"

dpSyphonClientManager* dpSyphonClientManager::__instance = NULL;

void dpSyphonClientManager::setup(){
    
    mServerDirectory.setup();
    
    ofAddListener(mServerDirectory.events.serverAnnounced, this, &dpSyphonClientManager::serverAnnounced);
    ofAddListener(mServerDirectory.events.serverRetired, this, &dpSyphonClientManager::serverRetired);
    
}

const ofPtr<ofxSyphonClient> & dpSyphonClientManager::getClient(int clientNum){
    
    if(mClients.empty()){
        
        assert(false);
    
    }
    
    if(clientNum >= mClients.size()){
       // cout << "dpSyphonClientManager : client num is bigger than size, return first cilent" << endl;
        map<string, ofPtr<ofxSyphonClient> >::iterator it = mClients.begin();
        
        return it->second;
    }
    
    map<string, ofPtr<ofxSyphonClient> >::iterator it = mClients.begin();
    
    for(int i = 0; i < clientNum; i++){
        it++;
    }
    
    return it->second;
    
}

void dpSyphonClientManager::serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg)
{
   /* for( auto& dir : arg.servers ){
        
        auto itr = mClients.find(dir.serverName);
        
        if(itr != mClients.end());
        else{
            ofPtr<ofxSyphonClient> client = ofPtr<ofxSyphonClient>(new ofxSyphonClient);
            client->setup();
            client->set(dir);
            mClients[dir.serverName] = client;
        }
    }*/
}


void dpSyphonClientManager::serverRetired(ofxSyphonServerDirectoryEventArgs &arg)
{
    mClients.clear();
   // cout << "retire" << endl;
    /*for( auto& dir : arg.servers ){
    
        auto itr = mClients.find(dir.serverName);
            cout << dir.serverName << endl;
        if(itr != mClients.end()){
            mClients.erase(dir.serverName);
        }
    }*/
}

void dpSyphonClientManager::update(){
    
    vector<ofxSyphonServerDescription> desc = mServerDirectory.getServerList();
    
    for(auto &v:desc){
        
        auto itr = mClients.find(v.serverName);
        
        if(itr != mClients.end());
        else{
            ofPtr<ofxSyphonClient> client = ofPtr<ofxSyphonClient>(new ofxSyphonClient);
            client->setup();
            client->set(v.serverName,v.appName);
            mClients[v.serverName] = client;
        }
    }
}

void dpSyphonClientManager::draw(ofPoint pos, ofPoint size, int clientNum){
    draw(pos.x, pos.y, size.x, size.y,clientNum);
}

void dpSyphonClientManager::draw(float x,float y, float width, float height, int clientNum){

    if(empty())return;

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    ofDisableDepthTest();
    getClient(clientNum)->draw(x,y,width,height);
    glPopAttrib();
}

void dpSyphonClientManager::draw(float x, float y, float width, float height){
    draw(x,y,width,height,0);
}

void dpSyphonClientManager::drawWithSideCrop(float x, float y){
    
    float scale = (float)SINGLE_SCREEN_HEIGHT / (float)EXTERNAL_VIDEO_HEIGHT;
    float width = EXTERNAL_VIDEO_WIDTH * scale;
    
    draw((SINGLE_SCREEN_WIDTH - width) * 0.5 + x, y, width, SINGLE_SCREEN_HEIGHT);
    
};

void dpSyphonClientManager::drawWithSideCrop(){
    
    drawWithSideCrop(0,0);
    
};

bool dpSyphonClientManager::empty(){
    if(mClients.empty()){
        
        return true;
        
    }
    
    return false;
}

