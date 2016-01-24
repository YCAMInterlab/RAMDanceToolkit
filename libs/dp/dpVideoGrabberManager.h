//
//  dpVideoGrabberManager.h
//  2015_MAIN_APP
//
//  Created by kezzardrix2 on 2016/01/24.
//
//

#ifndef _015_MAIN_APP_dpVideoGrabberManager_h
#define _015_MAIN_APP_dpVideoGrabberManager_h

#include "dpConstants.h"

class dpVideoGrabberManager{
public:
    
    static dpVideoGrabberManager& instance()
    {
        if (__instance == NULL)
            __instance = new dpVideoGrabberManager;
        return *__instance;
    }
    
    void setupDevice(int deviceID);
    void update();
    
    const ofTexture &getTexture(int cameraNum);
    
    void draw(ofPoint pos, ofPoint size);
    void draw(float x, float y,float width, float height);
    void draw(ofPoint pos, ofPoint size, int cameraNum);
    void draw(float x, float y,float width, float height, int cameraNum);
    
private:
    
    map<int, ofPtr<ofVideoGrabber> >mGrabbers;
    
    static dpVideoGrabberManager *__instance;
    
    dpVideoGrabberManager() {};
    dpVideoGrabberManager(const dpVideoGrabberManager&){}
    dpVideoGrabberManager& operator=(const dpVideoGrabberManager&){return *__instance;}
    ~dpVideoGrabberManager();

    const ofPtr<ofVideoGrabber> &getGrabber(int cameraNum);
};

#endif
