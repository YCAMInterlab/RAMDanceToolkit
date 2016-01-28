//
//  BurstBody.h
//  afterWorkshop
//
//  Created by 神田竜 on 2014/07/17.
//
//

#ifndef afterWorkshop_BurstBody_h
#define afterWorkshop_BurstBody_h

#include "KezSlidePoint.h"

class BurstBox{
private:
    bool velSetMode = false;;
public:
    KezSlidePoint mPos;
    
    ofPoint mVel;
    float a;
    bool bFixed = false;
    
    BurstBox(){
        mPos.speed = 0.333;
        a = 1.02;
    }
    
    void update(ofPoint pos,ofPoint vel){
        
        if(velSetMode)mVel = vel;
        
        if(bFixed == false){
            mPos.set(pos);
            if(velSetMode == false)mVel = vel;
        }
        else {
            mVel *= a;
            mPos += mVel;
        }
        
        mPos.update();
    }
    
    void setVelMode(bool mode){
        velSetMode = mode;
    }
};

class BurstBody : public ramBaseScene
{
    
public:
	
	string getName() const { return "BurstBody"; }
    
	void setupControlPanel()
	{
        
        ramGetGUI().addSlider("thresh", 0.1, 30.0, &thresh);
        ramGetGUI().addToggle("threshMode",&threshMode);
        ramGetGUI().addToggle("velMode",&velMode);
        // ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &FMCube::onPanelChanged);
        
    }
    
	void setup()
	{
        for(int i = 0; i < NUM; i++){
            mBoxes.push_back(BurstBox());
        }
        
    }
    
    void updatWithOscMessage(ofxOscMessage &m){
        
    }
    
	void update()
	{
        for(auto &v:mBoxes){
            v.setVelMode(velMode);
        }
	}
    
	void draw()
	{
		
		ramBeginCamera();
        
        
		ramEndCamera();
	}
    
	void drawActor(const ramActor& actor)
	{
        //const ramActor &centered = mCentered.update(actor);
        //const ramNode &leftHand = centered.getNode(ramActor::JOINT_LEFT_HAND);//actor.getNode(ramActor::JOINT_LEFT_HAND);
        
        ramActor tmpActor(actor);
        
        for(int i = 0; i <tmpActor.getNumNode(); i++){
            
            ramNode &tmp = tmpActor.getNode(i);
            
            float jointSize = (i == ramActor::JOINT_HEAD) ? 8.0 : 5.0;
            
            float length = tmpActor.getNode(i).getVelocity().length();
            
            if(threshMode){
            
                if(length < thresh)mBoxes[i].bFixed = false;
                else mBoxes[i].bFixed = true;
            
            }else{
                
                if(length > thresh)mBoxes[i].bFixed = false;
                else mBoxes[i].bFixed = true;
                
            }
            mBoxes[i].update(tmp.getGlobalPosition(),actor.getNode(i).getVelocity());
            
            tmp.setGlobalPosition(mBoxes[i].mPos);
            
            
            ramBox(tmp, jointSize);
            ramLine(tmp);
            
        }
        
        
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
		string name = e.widget->getName();
		
		if (name == "Random Pos")
		{
			//randomizeCubePos();
		}
	}
	
private:
    
    static const int NUM = 23;
    vector<BurstBox>mBoxes;
    bool mTog[NUM];
    float thresh = 2.0;
    
    bool threshMode = true;
    bool velMode = false;
};


#endif
