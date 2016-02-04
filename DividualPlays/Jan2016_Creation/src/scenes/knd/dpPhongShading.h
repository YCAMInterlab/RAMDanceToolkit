//
//  dpPhongShading.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2016/02/04.
//
//

#ifndef RAMDanceToolkit_dpPhongShading_h
#define RAMDanceToolkit_dpPhongShading_h

#include "ramMain.h"

class dpPhongShading{
public:
    
    static dpPhongShading& instance()
    {
        if (__instance == NULL){
            __instance = new dpPhongShading;
            __instance->setup();
        }
        return *__instance;
    }
    
    void begin();
    void begin(ofPoint lightPos,float spec);
    void end();
    
private:
    
    ofShader mShader;
    
    static dpPhongShading *__instance;
    
    dpPhongShading() {};
    dpPhongShading(const dpPhongShading&){}
    dpPhongShading& operator=(const dpPhongShading&){return *__instance;}
    ~dpPhongShading();
    
    void setup();
    
};

#endif
