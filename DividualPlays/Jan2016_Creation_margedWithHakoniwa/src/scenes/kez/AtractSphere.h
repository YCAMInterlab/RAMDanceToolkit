//
//  AtractSphere.h
//  afterWorkshop
//
//  Created by 神田竜 on 2014/07/17.
//
//

#ifndef afterWorkshop_AtractSphere_h
#define afterWorkshop_AtractSphere_h

class AtractSphere{
public:
    ofPoint mPos;
    float mSpeed = 0.2;
    float mThresh = 10.0;
    float mForce = 0.2;
    float mEscapeForce = 0.2;
    ofPoint v;
    
    float mDeaden = 0.998;
    float mEscapeDeaden = 0.998;
    
    void update(ofPoint target){
        if((mPos - target).length() > mThresh){
           // mPos += (target - mPos) * mSpeed;
            atract(target);
        }else{
            //mPos += (mPos - target) * mSpeed;
            escape(target);
        }
    }
    
    void atract(ofPoint center){
        ofVec3f cVec;
        cVec = (center-mPos);
        cVec.normalize();
        cVec *= mForce;
        v += cVec;
        v *= mDeaden;
        mPos += v;
    }
    
    void escape(ofPoint center){
        ofVec3f cVec;
        cVec = (mPos - center);
        cVec.normalize();
        cVec *= mEscapeForce;
        v += cVec;
        v *= mEscapeDeaden;
        mPos += v;
    }
};

class AtractSphereScene : public ramBaseScene
{
    
public:
	
	string getName() const { return "AtractSphereScene"; }
    
	void setupControlPanel()
	{
        
        ramGetGUI().addSlider("thresh", 1.0, 300.0, &thresh);
        ramGetGUI().addSlider("deaden", 0.8, 1.0, &deaden);
        ramGetGUI().addSlider("force", 0.0, 1.0, &force);
        ramGetGUI().addSlider("escapeDeaden", 0.8, 1.0, &escapeDeaden);
        ramGetGUI().addSlider("escapeForce", 0.0, 1.0, &escapeForce);
        // ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &FMCube::onPanelChanged);
        
    }
    
	void setup()
	{
        for(int i = 0; i < NUM; i++){
            mSpheres.push_back(AtractSphere());
        }
        
    }
    
    void updatWithOscMessage(ofxOscMessage &m){
        
    }
    
	void update()
	{
        for(auto &v:mSpheres){
            v.mThresh = thresh;
            v.mForce = force;
            v.mDeaden = deaden;
            v.mEscapeForce = escapeForce;
            v.mEscapeDeaden = escapeDeaden;
        }
	}
    
	void draw()
	{
		
		ramBeginCamera();
        
        for(auto &v:mSpheres){
            ofSphere(v.mPos,5.0);
        }
        
		ramEndCamera();
	}
    
	void drawActor(const ramActor& actor)
	{
    
        for(int i = 0; i <actor.getNumNode(); i++){
            
            const ramNode &node = actor.getNode(i);
            
            float jointSize = (i == ramActor::JOINT_HEAD) ? 8.0 : 5.0;
 
            mSpheres[i].update(node.getGlobalPosition());
            
            
            ramBox(node, jointSize);
            ramLine(node);
            ofLine(mSpheres[i].mPos,node.getGlobalPosition());
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
    vector<AtractSphere>mSpheres;
    float thresh = 10.0;
    float force = 0.2;
    float deaden = 0.98;
    float escapeForce = 0.2;
    float escapeDeaden = 0.98;
};


#endif
