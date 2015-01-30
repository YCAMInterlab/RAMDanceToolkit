//
//  HakoniwaVibrator.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2014/12/19.
//
//

#ifndef RAMDanceToolkit_HakoniwaVibrator_h
#define RAMDanceToolkit_HakoniwaVibrator_h

class HakoniwaVibrator : public ramBaseScene{
public:
    string getName() const { return "HakoniwaVibrator"; }
    void setup(){
        mVibePos.set(200,100,0);
        mSender.setup("192.168.20.52", 8528);
    }
    
    
    void setupControlPanel(){
        ramGetGUI().addSlider("thresh", 10.0, 500.0, &mThresh);
        ramGetGUI().addSlider("amount", 1.0, 50.0, &mVibeAmnt);
        ramGetGUI().addSlider("val1", 0.0, 255.0, &mVibeVal[0]);
        ramGetGUI().addSlider("val2", 0.0, 255.0, &mVibeVal[1]);
        ramGetGUI().addSlider("val3", 0.0, 255.0, &mVibeVal[2]);
    }
    
    void update(){
        
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/vibe");
        m.addIntArg((int)mVibeVal[0]);
        m.addIntArg((int)mVibeVal[1]);
        m.addIntArg((int)mVibeVal[2]);
        mSender.sendMessage(m);
        
        /*if(ofGetMousePressed()){
            ofxOscMessage m;
            m.setAddress("/dp/hakoniwa/vibe");
            m.addIntArg(1);
            mSender.sendMessage(m);
        }else{
            ofxOscMessage m;
            m.setAddress("/dp/hakoniwa/vibe");
            m.addIntArg(0);
            mSender.sendMessage(m);
        }*/
    }
    void draw(){
        ramBeginCamera();
        ofPushStyle();
        ofSetColor(255);
        
        mVibePosVibed = mVibePos;
        mVibePosVibed += ofPoint(ofRandom(-1.0,1.0),
                            ofRandom(-1.0,1.0),
                            ofRandom(-1.0,1.0)) * mDist * mVibeAmnt;
        
        ofDrawBox(mVibePosVibed, mVibeBoxSize);
        ofLine(mVibePosVibed.x,mVibePosVibed.y,mVibePosVibed.z,
               mVibePosVibed.x,mVibePosVibed.y + 1000,mVibePosVibed.z);
        
        ofPopStyle();
        ramEndCamera();
    }
    void drawActor(const ramActor& actor)
	{
        for(int i = 0; i < actor.getNumNode(); i++){
            
            const ramNode &node = actor.getNode(i);
            
            if(i == ramActor::JOINT_LEFT_HAND){
                ofLine(node.getGlobalPosition().x,node.getGlobalPosition().y,node.getGlobalPosition().z,
                      mVibePosVibed.x,mVibePosVibed.y,mVibePosVibed.z);
                float dist = (node.getGlobalPosition() - mVibePosVibed).length();
                mDist = ofMap(dist,0,mThresh,0.0,1.0,true);
               // mDist *= mDist;
                
                ofxOscMessage m;
                m.setAddress("/dp/hakoniwa/vibe");
               
                //int isVibe = (mDist > 0.5)? 1 : 0;
                
                m.addIntArg(mDist * 256);
              //  mSender.sendMessage(m);
            }
            
            ofPushMatrix();
            float jointSize = (i == ramActor::JOINT_HEAD) ? 8.0 : 5.0;
            ofDrawBox(node.getGlobalPosition(), jointSize);
            ramLine(node);
            ofPopMatrix();
            
        }
        
    }
   
    void onPanelChanged(ofxUIEventArgs& e){}
private:
    ofxOscSender mSender;
    ofPoint mVibePos;
    ofPoint mVibePosVibed;
    float mVibeBoxSize = 20;
    float mDist = 0.0;
    
    float mThresh = 100.0;
    float mVibeAmnt = 10.0;
    
    float mVibeVal[3];
};

#endif
