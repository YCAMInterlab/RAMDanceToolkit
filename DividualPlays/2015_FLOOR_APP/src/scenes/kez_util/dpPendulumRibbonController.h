//
//  RibbonController.h
//  HakoVisTri
//
//  Created by kezzardrix2 on 2015/01/06.
//
//

#ifndef HakoVisTri_RibbonController_h
#define HakoVisTri_RibbonController_h

#include "dpServoPendulumRibbon.h"

class dpServoPendulumRibbonController{
public:
    void setup(){
        
        mRibbons.clear();
        
        for(int i = 0; i < MAX_RIBBONS; i++){
            mRibbons.push_back(dpServoPendulumRibbon());
            mRibbons.back().setup();
        }
    }
    
    void changeRibbon(){
        
        mRibbons[mCurrentRibbon].finishedAddPt();
        
        mCurrentRibbon++;
        mCurrentRibbon %= mRibbons.size();
        
    }
    
    void update(){
        for(auto &v:mRibbons){
            v.update();
        }
    }
    
    void addPoint(ofPoint pt){
        
        if((mPrePt - pt).length() > mThresh){
        
            mRibbons[mCurrentRibbon].setHead(pt);
            mIsRibbonChanged = false;
            mPrePt = pt;
        }else{
            
            if(mIsRibbonChanged == false){
                mIsRibbonChanged = true;
                changeRibbon();
            }
            
        }
    
    }
    
    void draw(){
        for(auto &v:mRibbons){
            v.draw();
        }
    }
    
    
private:
    static const int MAX_RIBBONS = 6;
    vector<dpServoPendulumRibbon>mRibbons;
    
    float mThresh = 1.0;
    int mCurrentRibbon = 0;
    
    bool mIsRibbonChanged = true;
    
    ofPoint mPrePt;
};

#endif
