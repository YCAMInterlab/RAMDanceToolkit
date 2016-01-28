//
//  FloorLine.h
//  afterWorkshop
//
//  Created by 神田竜 on 2014/08/04.
//
//

#ifndef afterWorkshop_FloorLine_h
#define afterWorkshop_FloorLine_h

#include "Drop.h"
#include "ramCenteredActor.h"

class SoundSphere{
private:
    
    void sendMessage(int enable){
        ofxOscMessage m;
        m.setAddress("/dp/floorLine/sound");
        m.addIntArg(mID);
        m.addIntArg(enable);
        m.addFloatArg(mPos.x);
        m.addFloatArg(mPos.y);
        m.addFloatArg(mPos.z);
        
        mSenders[0]->sendMessage(m);
        mSenders[1]->sendMessage(m);
    }
    
    void randomizePosXY(){
        mPos.x = ofRandom(-300,300);
        mPos.z = ofRandom(-300,300);
        mRad = ofRandom(20,70);
    }
    
    bool hasTouched = false;

public:
    ofPoint mPos;
    float mRad;

    bool isTouched = false;
    
    int mID = 0;

    ofColor mColor;
    
    ofxOscSender *mSenders[2];
    
    SoundSphere(int id){
        
        mID = id;
        
        randomizePosOnGround();
        
        int rnd = mID % 4;
        
        if(rnd == 0){
            mColor.set(ofColor::red);
        }else if(rnd == 1){
            mColor.set(ofColor::blue);
        }else if(rnd == 2){
            mColor.set(ofColor::green);
        }else if(rnd == 3){
            mColor.set(ofColor::magenta);
        }
    }
    
    void reset(){
        sendMessage(0);
    }
    
    void setSenders(ofxOscSender *sender1, ofxOscSender *sender2){
        mSenders[0] = sender1;
        mSenders[1] = sender2;
    }
    
    void randomizePosOnGround(){
        
        randomizePosXY();
        
        mPos.y = mRad * 0.5;
        
    }
    
    void randomizePos(){
        
        randomizePosXY();
        
        mPos.y = mRad * 0.5 + ofRandom(0,300);
        
    }
    
    
    void resetTouched(){
        isTouched = false;
    }
    
    void checkTouched(){
        
        if(isTouched && hasTouched == false){
            sendMessage(1);
            hasTouched = true;
        }
        
        if(isTouched == false && hasTouched){
            sendMessage(0);
            hasTouched = false;
        }
    }
    
    void update(ofPoint target){
    
        if((mPos - target).length() < mRad){
            
            isTouched = true;
            
        }

    }
    void draw(){
        
        ofPushStyle();
        ofFill();
        
        if(isTouched){
            ofSetColor(mColor);
        }
        else ofSetColor(255,255,255);
        
        ofDrawSphere(mPos,mRad);
        ofPopStyle();
    
    }
  
};

class FloorLine : public ramBaseScene
{
    
public:
	
	string getName() const { return "FloorLine"; }
    
	void setupControlPanel()
	{
     
        ramGetGUI().addSlider("LeftShoulder",1.0,10.0,&mLShoulderScale);
        ramGetGUI().addSlider("RightShoulder",1.0,10.0,&mRShoulderScale);
        
        ramGetGUI().addSlider("LeftHip",1.0,10.0,&mLHipScale);
        ramGetGUI().addSlider("RightHip",1.0,10.0,&mRHipScale);
        
        ramGetGUI().addToggle("isSoundSphere", &mIsSphere);
        
        ramGetGUI().addButton("randomizePosOnGround");
        ramGetGUI().addButton("randomizePos");
        
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &FloorLine::onPanelChanged);
 
        
    }
    
	void setup()
	{
        
        for(int i = 0; i < NUM_SPHERE; i++){
            mSpheres.push_back(SoundSphere(i));
        }
        
        setupOscSenders();
        
    }
    
    void updatWithOscMessage(ofxOscMessage &m){
        
    }
    
	void update()
	{
        for(auto &v:mSpheres){
            v.resetTouched();
        }
	}
    
	void draw()
	{
	
	}
    
	void drawActor(const ramActor& actor)
	{

            const ramNodeArray &array = actor;
        
            ramActor centered = mCentered.update(array);
        
            for(int j = 0; j < centered.getNumNode(); j++){
                
                ramNode &node = centered.getNode(j);
                
                if(j == ramActor::JOINT_LEFT_SHOULDER)node.setScale(mLShoulderScale);
                
                if(j == ramActor::JOINT_RIGHT_SHOULDER)node.setScale(mRShoulderScale);
                
                if(j == ramActor::JOINT_LEFT_HIP)node.setScale(mLHipScale);
                
                if(j == ramActor::JOINT_RIGHT_HIP)node.setScale(mRHipScale);
                
                ofPoint pos = node.getGlobalPosition();
               
                if(pos.y < 0.0)node.setGlobalPosition(pos.x, 0.0, pos.z);
                
            }
      
            if(mIsSphere){
                for(auto &v:mSpheres){
                    v.update(centered.getNode(ramActor::JOINT_LEFT_HAND).getGlobalPosition());
                    v.update(centered.getNode(ramActor::JOINT_RIGHT_HAND).getGlobalPosition());
                    v.update(centered.getNode(ramActor::JOINT_LEFT_TOE).getGlobalPosition());
                    v.update(centered.getNode(ramActor::JOINT_RIGHT_TOE).getGlobalPosition());
                }
            }
        
            drawExtendActor(centered);
            drawSpheres();
	}
    
    void drawSpheres(){
        if(mIsSphere){
            ofEnableLighting();
            ofLight light;
            light.enable();
            for(auto &v:mSpheres){
                v.draw();
                v.checkTouched();
            }
            ofDisableLighting();
        }
    }
    
    void drawExtendActor(ramActor &centered){
        for(int i = 0; i < centered.getNumNode(); i++){
            
            ramNode &node = centered.getNode(i);
            
            ofPushMatrix();
            float jointSize = (i == ramActor::JOINT_HEAD) ? 8.0 : 5.0;
            ofDrawBox(node.getGlobalPosition(), jointSize);
            ramLine(node);
            ofPopMatrix();
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
    
    void onDisabled(){
        for(auto &v:mSpheres){
            v.reset();
        }
    }
    
    
    void onPanelChanged(ofxUIEventArgs& e)
	{
		string name = e.widget->getName();

        if(name == "randomizePosOnGround"){
            
           ofxUIButton *b = static_cast<ofxUIButton *>(e.widget);
            
            if(b->getValue()){
            
                for(auto &v:mSpheres){
                    v.randomizePosOnGround();
                }
            }
        }
        
        if(name == "randomizePos"){
            
            ofxUIButton *b = static_cast<ofxUIButton *>(e.widget);
            
            if(b->getValue()){
                
                for(auto &v:mSpheres){
                    v.randomizePos();
                }
            }
        }
	}
	
private:
    
    float mLShoulderScale = 6.0;
    float mRShoulderScale = 6.0;
    
    float mLHipScale = 6.0;
    float mRHipScale = 6.0;

    ramCenteredActor mCentered;
    
    static const int NUM_SPHERE = 6;
    
    vector<SoundSphere>mSpheres;
    bool mIsSphere = true;
    
    ofxOscSender mSenderToSound[2];
    
    void setupOscSenders(){
        mSenderToSound[0].setup("192.168.20.9",10000);
        mSenderToSound[1].setup("192.168.20.10",10000);
        
        for(auto &v:mSpheres){
            v.setSenders(&mSenderToSound[0], &mSenderToSound[1]);
        }
        
    }
};


#endif
