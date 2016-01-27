//
//  BurstBody_ext.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2016/01/28.
//
//

#ifndef RAMDanceToolkit_BurstBody_ext_h
#define RAMDanceToolkit_BurstBody_ext_h

#include "ramMotionExtractor.h"
#include "KezSlidePoint.h"
#include "BurstBody.h"

class BurstBody_ext : public ramBaseScene
{
    
public:
    
    string getName() const { return "BurstBody_ext"; }
    
    void setupControlPanel()
    {
        
        ramGetGUI().addSlider("thresh", 0.1, 30.0, &mThresh);
        ramGetGUI().addToggle("threshDir",&mThreshDir);
        ramGetGUI().addToggle("velMode",&mVelMode);
   
        ofxUICanvasPlus* gui = ramGetGUI().getCurrentUIContext();
        gui->addWidgetDown(new ofxUILabel("BodyColor", OFX_UI_FONT_MEDIUM));
        gui->addSlider("R", 0, 1, &bodyColor.r, 70, 16);
        gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
        gui->addSlider("G", 0, 1, &bodyColor.g, 70, 16);
        gui->addSlider("B", 0, 1, &bodyColor.b, 70, 16);
        gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
        gui->autoSizeToFitWidgets();
        
        mex.setupControlPanel(this);
    }
    
    void setup()
    {
    }
    
    void updatWithOscMessage(ofxOscMessage &m){
        
    }
    
    void update()
    {
        ramActorManager &am = ramGlobalShortcut::getActorManager();
        
        int numNodes = 0;
        for (int i = 0;i < am.getNumNodeArray();i++)
            numNodes += am.getNodeArray(i).getNumNode();
        
        if (mBoxes.size() != numNodes)
        {
            mBoxes.clear();
            for (int i = 0;i < numNodes;i++)
                mBoxes.push_back(BurstBox());
        }
        
        mex.update();
        
        for(auto &v:mBoxes){
            v.setVelMode(mVelMode);
        }
    }
    
    void draw()
    {
        
        ramBeginCamera();
        
        ramActorManager &am = ramGlobalShortcut::getActorManager();
        int cnt = 0;
        for (int i = 0;i < am.getNumNodeArray();i++)
        {
            ramActor act = am.getNodeArray(i);
            bool bActEnable = false;
            for (int q = 0;q < mex.getNumPort();q++)
                if (mex.getActorNameAt(q) == act.getName()) bActEnable = true;
            
            if (!bActEnable) continue;
            
            for (int j = 0;j < act.getNumNode();j++)
            {
                ramNode & tmpNode = act.getNode(j);
                
                float jointSize = (j == ramActor::JOINT_HEAD) ? 8.0 : 5.0;
                float length = tmpNode.getVelocity().length();
                int idx = cnt;
                
                if(mThreshDir)mBoxes[idx].bFixed = (length > mThresh);
                else mBoxes[idx].bFixed = (length < mThresh);
                
                mBoxes[idx].update(tmpNode.getGlobalPosition(),act.getNode(i).getVelocity());
                
                tmpNode.setGlobalPosition(mBoxes[idx].mPos);
                
                ofSetColor(bodyColor);
                ramBox(tmpNode, jointSize);
                ramLine(tmpNode);
                
                cnt++;
            }
        }
        
        mex.draw();
        ramEndCamera();
    }
    
    void drawActor(const ramActor& actor)
    {
        
    }
    
    
    
    void drawRigid(const ramRigidBody &rigid)
    {
        
    }
    
    void onActorSetup(const ramActor &actor)
    {
        
    }
    
    void onActorExit(const ramActor &actor)
    {
        
    }
    
    void onRigidSetup(const ramRigidBody &rigid)
    {
        
    }
    
    void onRigidExit(const ramRigidBody &rigid)
    {
        
    }
    
    
    void onPanelChanged(ofxUIEventArgs& e)
    {
       
    }
    
private:

    float mThresh = 2.0;
    
    bool mThreshDir = true;
    bool mVelMode = false;
    
    ofFloatColor bodyColor;
    vector<BurstBox> mBoxes;
    ramMotionExtractor mex;
    
};


#endif
