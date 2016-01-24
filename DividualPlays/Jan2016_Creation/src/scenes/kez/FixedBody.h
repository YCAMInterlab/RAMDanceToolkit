//
//  FixedBody.h
//  afterWorkshop
//
//  Created by 神田竜 on 2014/07/17.
//
//

#ifndef afterWorkshop_FixedBody_h
#define afterWorkshop_FixedBody_h

#include "slidePoint.h"

class FixedBox{
public:
    slidePoint mPos;
    ofPoint mTarget;
    bool bFixed = false;
    
    FixedBox(){
        mPos.speed = 0.333;
    }
    
    void update(ofPoint pos){
        mTarget.set(pos);
        
        if(bFixed == false)mPos.set(mTarget);
        
        mPos.update();
    }
};

class FixedBody : public ramBaseScene
{
    
public:
	
	string getName() const { return "FixedBody"; }
    
	void setupControlPanel()
	{
        for(int i = 0; i < NUM; i++){
           // mTog.push_back(1);
            ramGetGUI().addToggle(ramActor::getJointName(i), &mTog[i]);
        }
        
       // ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &FMCube::onPanelChanged);
        
    }
    
	void setup()
	{
        for(int i = 0; i < NUM; i++){
            mBoxes.push_back(FixedBox());
        }
      
    }
    
    void updatWithOscMessage(ofxOscMessage &m){
        
    }
    
	void update()
	{
       
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
        
        for(int i = 0; i < actor.getNumNode(); i++){
            
            ramNode &tmp = tmpActor.getNode(i);
            
            float jointSize = (i == ramActor::JOINT_HEAD) ? 8.0 : 5.0;
            
            mBoxes[i].bFixed = mTog[i];
            mBoxes[i].update(tmp.getGlobalPosition());
            
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
    vector<FixedBox>mBoxes;
    bool mTog[NUM];
};


#endif
