//
//  dpVideoGrabber.cpp
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2016/01/24.
//  Copyright (c) 2016年 YCAMInterlab. All rights reserved.
//

#include "dpVideoGrabberManager.h"

dpVideoGrabberManager* dpVideoGrabberManager::__instance = NULL;

void dpVideoGrabberManager::setupDevice(int deviceID){
    
    auto itr = mGrabbers.find(deviceID);
    
    if(itr != mGrabbers.end()){
        
        cout << "dpVideoGrabberManager : this deviceID has already registered" << endl;
        
        return;
    
    }
    
    ofPtr<ofVideoGrabber> grabber = ofPtr<ofVideoGrabber>(new ofVideoGrabber);
    
    mGrabbers[deviceID] = grabber;
    mGrabbers[deviceID]->setDeviceID(deviceID);
    mGrabbers[deviceID]->initGrabber(EXTERNAL_VIDEO_WIDTH, EXTERNAL_VIDEO_HEIGHT);
    
}

void dpVideoGrabberManager::update(){
    for(auto &v:mGrabbers){
        v.second->update();
    }
}

const ofPtr<ofVideoGrabber> & dpVideoGrabberManager::getGrabber(int cameraNum){
    
    if(mGrabbers.empty())assert(false);
    
    if(cameraNum > mGrabbers.size()){
        
        cout << "dpVideoGrabberManager : camera number is bigger than size, return first camera" << endl;
        
        return mGrabbers.begin()->second;
    }
    
    map<int,ofPtr<ofVideoGrabber> >::iterator it = mGrabbers.begin();
    
    for(int i = 0; i < cameraNum; i++){
        it++;
    }
    
    return it->second;
    
}

const ofTexture & dpVideoGrabberManager::getTexture(int cameraNum){
    return getGrabber(cameraNum)->getTextureReference();
}

void dpVideoGrabberManager::draw(ofPoint pos, ofPoint size){
    draw(pos.x,pos.y,size.x,size.y,0);
};

void dpVideoGrabberManager::draw(float x, float y,float width, float height){
    draw(x,y,width,height,0);
};

void dpVideoGrabberManager::draw(ofPoint pos, ofPoint size, int cameraNum){
    draw(pos.x,pos.y,size.x,size.y,cameraNum);
};

void dpVideoGrabberManager::draw(float x, float y,float width, float height, int cameraNum){
    getGrabber(cameraNum)->draw(x,y,width,height);
};

dpVideoGrabberManager::~dpVideoGrabberManager(){
    for(auto &v:mGrabbers){
        v.second->close();
    }
}