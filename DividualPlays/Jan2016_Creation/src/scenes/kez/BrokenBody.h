//
//  BrokenBody.h
//  afterWorkshop
//
//  Created by 神田竜 on 2014/07/17.
//
//

#ifndef afterWorkshop_BrokenBody_h
#define afterWorkshop_BrokenBody_h

class BrokenBox{
public:
    slidePoint mPos;

    float v;
    float a;
    bool bFixed = false;
    
    BrokenBox(){
        mPos.speed = 0.333;
        v = 1.0;
        a = 1.1;
    }
    
    void update(ofPoint pos){
        
        if(bFixed == false){
            mPos.set(pos);
            v = 1.0;
        }
        else {
            v *= a;
            mPos.yAdd(-v);
            
            if(mPos.y < 0.0)mPos.imSetY(0.0);
        }
        
        mPos.update();
    }
};

class BrokenBody : public ramBaseScene
{
    
public:
	
	string getName() const { return "BrokenBody"; }
    
	void setupControlPanel()
	{
      
        ramGetGUI().addSlider("thresh", 0.1, 30.0, &thresh);
        // ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &FMCube::onPanelChanged);
        
    }
    
	void setup()
	{
        for(int i = 0; i < NUM; i++){
            mBoxes.push_back(BrokenBox());
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
        
        for(int i = 0; i <tmpActor.getNumNode(); i++){
            
            ramNode &tmp = tmpActor.getNode(i);
            
            float jointSize = (i == ramActor::JOINT_HEAD) ? 8.0 : 5.0;
            
            float length = tmpActor.getNode(i).getVelocity().length();
            
            if(length > thresh)mBoxes[i].bFixed = false;
            else mBoxes[i].bFixed = true;
            
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
    vector<BrokenBox>mBoxes;
    bool mTog[NUM];
    float thresh = 10.0;
};

#endif
