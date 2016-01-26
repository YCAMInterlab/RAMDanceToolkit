//
//  UnityHakoniwaScene.h
//  afterWorkshop
//
//  Created by kezzardrix2 on 2014/12/19.
//
//

#ifndef afterWorkshop_UnityHakoniwaScene_h
#define afterWorkshop_UnityHakoniwaScene_h

#include "ofxSyphon.h"

class UnityHakoniwaScene : public ramBaseScene
{
    
public:
	
	string getName() const { return "UnityHakoniwaScene"; }
    
	void setupControlPanel()
	{
        ramGetGUI().addSlider("mode", 0, 2, &mMode);
    }
    
	void setup()
	{
        mClient.setup();
        mSender.setup("127.0.0.1",6666);
    }
    
    void updatWithOscMessage(ofxOscMessage &m){
        
    }
    
	void update()
	{
        if(ofGetKeyPressed()){
            ofSetWindowPosition(0, 1280);
        }
        
        ramSetViewPort(getFirstViewPort());
	}
    
	void draw()
	{
		mClient.draw(SINGLE_SCREEN_WIDTH,0.0);
	}
    
	void drawActor(const ramActor& actor)
	{
        const ramNodeArray &array = actor;
        
        ofxOscMessage m;
        m.setAddress("/ram/actor");
        m.addStringArg("fromOF");
        
        for(int j = 0; j < actor.getNumNode(); j++){
            
            const ramNode &node = actor.getNode(j);
   
            if((int)mMode == COLOR){
            
            if(j == ramActor::JOINT_LEFT_HAND){
                ofSetColor(0,255,0);
                ofLine(node.getGlobalPosition().x,node.getGlobalPosition().y,node.getGlobalPosition().z,
                       node.getGlobalPosition().x,node.getGlobalPosition().y - 1000.0,node.getGlobalPosition().z);
            }else if(j == ramActor::JOINT_RIGHT_HAND){
                ofSetColor(0,0,255);
                ofLine(node.getGlobalPosition().x,node.getGlobalPosition().y,node.getGlobalPosition().z,
                       node.getGlobalPosition().x,node.getGlobalPosition().y - 1000.0,node.getGlobalPosition().z);
            }else if(j == ramActor::JOINT_HEAD){
                ofSetColor(255,0,0);
                ofLine(node.getGlobalPosition().x,node.getGlobalPosition().y,node.getGlobalPosition().z,
                       node.getGlobalPosition().x,node.getGlobalPosition().y - 1000.0,node.getGlobalPosition().z);
            }else{
                ofSetColor(255,255,255);
                
                
            }
            
            }else if((int)mMode == MAZE){
                if(j == ramActor::JOINT_LEFT_HAND){
                    ofLine(node.getGlobalPosition().x,node.getGlobalPosition().y,node.getGlobalPosition().z,
                           node.getGlobalPosition().x,node.getGlobalPosition().y - 1000.0,node.getGlobalPosition().z);
                }
            }
            
            
            
            m.addFloatArg(node.getGlobalPosition().x);
            m.addFloatArg(node.getGlobalPosition().y);
            m.addFloatArg(node.getGlobalPosition().z);
            
            float angle;
            ofPoint axis;
            
            node.getGlobalOrientation().getRotate(angle, axis);
            m.addFloatArg(angle);
            m.addFloatArg(axis.x);
            m.addFloatArg(axis.y);
            m.addFloatArg(axis.z);
            
            float jointSize = (j == ramActor::JOINT_HEAD) ? 8.0 : 5.0;
            ofDrawBox(node.getGlobalPosition(), jointSize);
            ramLine(node);
           
        }
        
        mSender.sendMessage(m);
        
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
	}
	
private:
    ofxSyphonClient mClient;
    ofxOscSender mSender;
    
    enum MODE{
        COLOR,
        MAZE,
    };
    
    float mMode = COLOR;
};

#endif
