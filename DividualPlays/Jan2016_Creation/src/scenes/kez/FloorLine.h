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

class ObstacleSphere{
private:
    int mSoundDurationCounter = 0;
    static const int MAX_SOUND_DUR = 120;
public:
    ofPoint mPos;
    float mRad;
    bool isTouched = false;
    
    ofSoundPlayer mPlayer;
    
    bool hasTouched = false;
    
    int soundID = 0;
    
    static const int MAX_SOUND_NUM = 4;
    ofColor mColors[MAX_SOUND_NUM];
    
    
    
    ObstacleSphere(){
        randomizePos();
        
        mColors[0].set(ofColor::red);
        mColors[1].set(ofColor::blue);
        mColors[2].set(ofColor::green);
        mColors[3].set(ofColor::magenta);
        
    }
    
    void reset(){
        isTouched = false;
    }
    
    void randomizePos(){
        mPos.x = ofRandom(-300,300);
        mPos.z = ofRandom(-300,300);
        
        mRad = ofRandom(20,70);
        
        mPos.y = mRad * 0.5;
        
        char str[255];
        
        soundID = (int)ofRandom(0,4);
        
        sprintf(str,"../../../../resources/Sounds/%d.aif",soundID + 1);
        
        mPlayer.loadSound(str);
        
        
        mPlayer.setVolume(0.5);
 
    }
    
    void update(ofPoint target){
        
        if(hasTouched == false && isTouched == true){
            mPlayer.play();
            mSoundDurationCounter = 0;
        }
        
        mSoundDurationCounter++;
        if(mSoundDurationCounter == MAX_SOUND_DUR)mPlayer.stop();
        
        //if(hasTouched == true && isTouched == false)mPlayer.stop();
        
        if((mPos - target).length() < mRad){
            isTouched = true;
        }
    }
    void draw(){
        ofPushStyle();
        ofFill();
        if(isTouched){
            ofSetColor(mColors[soundID]);
        }
        else ofSetColor(255,255,255);
        ofDrawSphere(mPos,mRad);
        ofPopStyle();
    }
    
    void checkTouched(){
        hasTouched = isTouched;
    }
};

class EscapePoint{
public:
    
    EscapePoint(ofPoint pt){
        setOrigin(pt);
    }
    
    void setOrigin(ofPoint pt){
        mOrigin = pt;
        mPos = pt;
        
        mAngle = atan2(-pt.z,-pt.x);
    }
    
    void resetFlag(){
        isCloseToTarget = false;
    }
    
    void checkTarget(ofPoint target){
        
        /*if(ofDist(mPos.x,mPos.z,target.x,target.z) < mThresh){
            mPos.x += (mPos.x - target.x) * speed;
            mPos.z += (mPos.z - target.z) * speed;
            isCloseToTarget = true;
        }*/
    }
    
    void checkAngle(float angle,float rad,ofPoint target){
        
        if(abs(mAngle - angle) < mThresh){
            //mPos.x += (mPos.x) * speed;
            //mPos.z += (mPos.z) * speed;
            
            float x = cos(angle) * -rad;
            float z = sin(angle) * -rad;
            
           // mPos.x += (x - mPos.x) * speed;
           // mPos.z += (z - mPos.z) * speed;
            
            mPos.x = x;
            mPos.z = z;
            
            isCloseToTarget = true;
        }
        
        mTarget = target;
    }
 
    void update(){
        
        if(isCloseToTarget == false){
        
            mPos += (mOrigin - mPos) * speed;
            
        }
    }
    
    void draw(){
        
        glVertex3f(mPos.x,mPos.y,mPos.z);
    
    }
    
    void drawLine(){
        if(isCloseToTarget){
        ofSetColor(255,255,255);
        ofLine(mPos.x,mPos.y,mPos.z,
               mTarget.x,mTarget.y,mTarget.z);
        }
    }
    
    ofPoint getPos(){
        return mPos;
    }
    
private:
    ofPoint mOrigin;
    ofPoint mPos;
    ofPoint mTarget;
    float speed = 0.1;
    float mThresh = TWO_PI / 360.0;
    bool isCloseToTarget = false;
    float mSize = 4;
    
    float mAngle;
};

class FloorLine : public ramBaseScene
{
    
public:
	
	string getName() const { return "FloorLine"; }
    
	void setupControlPanel()
	{
        
            // mTog.push_back(1);
           // ramGetGUI().addToggle(ramActor::getJointName(i), &mTog[i]);
       
        ramGetGUI().addSlider("LeftShoulder",1.0,10.0,&mLShoulderScale);
        ramGetGUI().addSlider("RightShoulder",1.0,10.0,&mRShoulderScale);
        
        ramGetGUI().addSlider("LeftHip",1.0,10.0,&mLHipScale);
        ramGetGUI().addSlider("RightHip",1.0,10.0,&mRHipScale);
        
        ramGetGUI().addToggle("isExtendDraw", &isExtendDraw);
        ramGetGUI().addToggle("isPushUp", &isPushUp);
        ramGetGUI().addToggle("isEscape", &isEscape);
        ramGetGUI().addSlider("escape rad",50,400,&mHankei);
        ramGetGUI().addToggle("isDrop", &isDrop);
        ramGetGUI().addSlider("handDropThresh",0.0,200.0,&mHandThresh);
        ramGetGUI().addToggle("isObstacleSphere", &isObstacle);
        
        
        ramGetGUI().addButton("randomObstacleSphere");
        
         ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &FloorLine::onPanelChanged);
 
        
    }
    
	void setup()
	{
        
        for(int i = 0; i < PT_NUM; i++){
            float theta = ofMap(i,0,PT_NUM,0,TWO_PI);
            float x = cos(theta) * mHankei;
            float z = sin(theta) * mHankei;
            float y = posY;
            
            EscapePoint pt(ofPoint(x,y,z));
            mPts.push_back(pt);
        }
        
        for(int i = 0; i < DROP_NUM; i++){
            mDrops.push_back(Drop());
        }
        
        for(int i = 0; i < OB_NUM; i++){
            mObstacle.push_back(ObstacleSphere());
        }
        
    }
    
    void updatWithOscMessage(ofxOscMessage &m){
        
    }
    
	void update()
	{
        for(auto &v:mDrops){
            v.update();
        }
        
        //ofSoundUpdate();
	}
    
	void draw()
	{
		
		ramBeginCamera();
        
        if(isEscape){
            
        ofPushMatrix();
        //ofScale(10,1,10);
        ofPushStyle();
        ofNoFill();
        
        ofSetColor(255,255,255);
        
        ofBeginShape();
        for(int i = 0; i < mPts.size(); i++){
            ofVertex(mPts[i].getPos().x,mPts[i].getPos().y,mPts[i].getPos().z);
        }
        
        ofVertex(mPts[0].getPos().x,mPts[0].getPos().y,mPts[0].getPos().z);
        
        ofEndShape();
        ofPopStyle();
    
        glPointSize(6);
        glEnable(GL_POINT_SMOOTH);
        glBegin(GL_POINTS);
        
        glColor4f(1,1,1,1);
        
        for(auto &v:mPts){
            v.draw();
        }
        glEnd();
            
            for(auto &v:mPts){
              //  v.drawLine();
            }
        
        ofPopMatrix();
            
        }
        
        if(isDrop){
            ofEnableBlendMode(OF_BLENDMODE_ADD);
            ofDisableDepthTest();
            for(auto &v:mDrops){
                v.draw();
            }
            ofEnableDepthTest();
        }
        
		ramEndCamera();
	}
    
	void drawActor(const ramActor& actor)
	{
        for(auto &v:mPts){
            v.resetFlag();
        }
        
    
        for(auto &v:mObstacle){
            v.reset();
        }
        
       // const vector<ramNodeArray> &arrays = getAllNodeArrays();
        
        //for(int i = 0; i < arrays.size(); i++){
            const ramNodeArray &array = actor;
        
            // ramActor tmpActor(array);
            
            ramActor centered = mCentered.update(array);
            
            if(!isExtendDraw)ramDrawBasicActor(centered);
            
            for(int j = 0; j < centered.getNumNode(); j++){
                
                ramNode &node = centered.getNode(j);
                
                if(j == ramActor::JOINT_LEFT_SHOULDER)node.setScale(mLShoulderScale);
                
                if(j == ramActor::JOINT_RIGHT_SHOULDER)node.setScale(mRShoulderScale);
                
                if(j == ramActor::JOINT_LEFT_HIP)node.setScale(mLHipScale);
                
                if(j == ramActor::JOINT_RIGHT_HIP)node.setScale(mRHipScale);
           
             
                if(j == ramActor::JOINT_LEFT_HAND){
                    drawToUnderground(node);
                }
                
                ofPoint pos = node.getGlobalPosition();
                float angle = atan2(-pos.z,-pos.x);
                
                if(!isPushUp){
                    if(pos.y < 0.0)node.setGlobalPosition(pos.x, 0.0, pos.z);
                }
                
                //if(j == ramActor::JOINT_RIGHT_ELBOW)node.setScale(mRShoulderScale);
                
                if(j == ramActor::JOINT_LEFT_HAND || j == ramActor::JOINT_RIGHT_HAND || j == ramActor::JOINT_LEFT_TOE || j == ramActor::JOINT_RIGHT_TOE){
                    
                    if(ofDist(pos.x,pos.z,0,0) >= mHankei){
                    
                        ofPoint tmp = node.getGlobalPosition();
                        
                        for(auto &v:mPts){
                            v.checkAngle(angle,ofDist(pos.x,pos.z,0,0),tmp);
                        }
                    
                    }
                }
                
            }
            
            //ramNode &node = centered.getNode(ramActor::JOINT_LEFT_HAND);
            
            float offset = 0.0;
            
            pushUp(centered.getNode(ramActor::JOINT_LEFT_HAND), offset);
            pushUp(centered.getNode(ramActor::JOINT_RIGHT_HAND), offset);
            pushUp(centered.getNode(ramActor::JOINT_LEFT_TOE), offset);
            pushUp(centered.getNode(ramActor::JOINT_RIGHT_TOE), offset);
            
            checkHandVelocity(centered);
        
        if(isObstacle){
            for(auto &v:mObstacle){
                v.update(centered.getNode(ramActor::JOINT_LEFT_HAND).getGlobalPosition() + ofPoint(0,offset));
                v.update(centered.getNode(ramActor::JOINT_RIGHT_HAND).getGlobalPosition() + ofPoint(0,offset));
                v.update(centered.getNode(ramActor::JOINT_LEFT_TOE).getGlobalPosition() + ofPoint(0,offset));
                v.update(centered.getNode(ramActor::JOINT_RIGHT_TOE).getGlobalPosition() + ofPoint(0,offset));
            }
        }
            
            if(isExtendDraw){
                for(int i = 0; i < centered.getNumNode(); i++){
                
                    ramNode &node = centered.getNode(i);
                
                    ofPushMatrix();
                    ofTranslate(0, offset);
                    float jointSize = (i == ramActor::JOINT_HEAD) ? 8.0 : 5.0;
                    ofDrawBox(node.getGlobalPosition(), jointSize);
                    ramLine(node);
                    ofPopMatrix();
                }
            }

        
        for(auto &v:mPts){
            v.update();
        }
        
            if(isObstacle){
                ofEnableLighting();
                ofLight light;
                light.enable();
                for(auto &v:mObstacle){
                    v.draw();
                    v.checkTouched();
                }
                ofDisableLighting();
            }
	}
    
    void pushUp(const ramNode &node,float &offset){
        if(node.getGlobalPosition().y < 0.0)offset = max(offset,abs(node.getGlobalPosition().y));
    }
    
    void checkHandVelocity(const ramNodeArray &array){
        ramNode leftHand = array.getNode(ramActor::JOINT_LEFT_HAND);
        ramNode rightHand = array.getNode(ramActor::JOINT_RIGHT_HAND);
        
        ofPoint lPos = leftHand.getGlobalPosition();
        ofPoint rPos = rightHand.getGlobalPosition();
        
        if((lPos - pLeftHand).length() > mHandThresh)dropFired(lPos);
        
        if((rPos - pRightHand).length() > mHandThresh)dropFired(rPos);
        
        pLeftHand = lPos;
        pRightHand = rPos;
        
    }
    
    void dropFired(ofPoint pos){
        mDrops[mDropCounter].fired(pos,ofPoint(0,-2.0,0));
        mDropCounter++;
        mDropCounter %= DROP_NUM;
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
		
		if (name == "hankei")
		{
			for(int i = 0; i < PT_NUM; i++){
                float theta = ofMap(i,0,PT_NUM,0,TWO_PI);
                float x = cos(theta) * mHankei;
                float z = sin(theta) * mHankei;
                float y = posY;
                
                mPts[i].setOrigin(ofPoint(x,y,z));
            }
		}
        
        
        
        
        if(name == "randomObstacleSphere"){
            
           ofxUIButton *b = static_cast<ofxUIButton *>(e.widget);
            
            if(b->getValue()){
            
                for(auto &v:mObstacle){
                    v.randomizePos();
                }
            }
        }
	}
	
private:
    
    static const int PT_NUM = 360;
    vector<EscapePoint>mPts;
    float posY = 100.0;
    float mHankei = 100.0;
    
    float mLShoulderScale = 1.0;
    float mRShoulderScale = 1.0;
    
    float mLHipScale = 1.0;
    float mRHipScale = 1.0;
    
    bool isDrawOnlyShadow = false;
    
    ramCenteredActor mCentered;
    
    static const int DROP_NUM = 200;
    int mDropCounter = 0;
    
    ofPoint pLeftHand;
    ofPoint pRightHand;
    
    vector<Drop>mDrops;
    
    float mHandThresh = 16.0;
    
    bool isDrop = false;
    bool isEscape = false;
    bool isExtendDraw = true;
    
    bool isPushUp = false;
    
    static const int OB_NUM = 6;
    vector<ObstacleSphere>mObstacle;
    bool isObstacle = false;
    
    void drawToUnderground(ramNode &node){

        ofLine(node.getGlobalPosition().x,node.getGlobalPosition().y,node.getGlobalPosition().z,
        node.getGlobalPosition().x,node.getGlobalPosition().y - 1000.0,node.getGlobalPosition().z);
        
    }
    
};


#endif
