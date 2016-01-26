//
//  ThreePointFlow.h
//  afterWorkshop
//
//  Created by 神田竜 on 2014/08/09.
//
//

#ifndef afterWorkshop_ThreePointFlow_h
#define afterWorkshop_ThreePointFlow_h

#include "GeometryHelpers.h"
#include "KezSlidePoint.h"

void rotateToNormal(ofVec3f normal) {
    normal.normalize();
    
    float rotationAmount;
    ofVec3f rotationAngle;
    ofQuaternion rotation;
    
    ofVec3f axis(0, 0, 1);
    rotation.makeRotate(axis, normal);
    rotation.getRotate(rotationAmount, rotationAngle);
    ofRotate(rotationAmount, rotationAngle.x, rotationAngle.y, rotationAngle.z);
}

class TailSphere{
public:
    static const int NUM = 30;
    
    KezSlidePoint mPos;
    ofPoint mTarget;
    int mTargetCounter = 0;
    vector<ofPoint>mTail;
    
    float mTheta = ofRandom(0.0,TWO_PI);
    
    float mRad = ofRandom(0.3,1.0);
    
    float mRotateSpeed = 0.1;
    
    TailSphere(float theta,float rad){
        for(int i = 0; i < NUM; i++){
            mTail.push_back(ofPoint());
        }
        
        mTheta = theta;
        mRad = rad;
    }
    
    void setRotateSpeed(float speed){
        mRotateSpeed = speed;
    }
    
    void setRad(float rad){
        mRad = rad;
    }
    
    void set(ofPoint pos){
        
        mPos.set(pos);
        mTarget = pos;
    }
    
    int getTarget(){
        return mTargetCounter;
    }
    
    void update(ofPoint pos){
        
        mPos.set(pos);
        mTail[0] = mPos;
        
        for(int i = 1; i < NUM; i++){
            ofPoint tmp = mTail[i - 1];
            mTail[i-1] = mTail[i];
            mTail[i] = tmp;
            
        }
        
        /*if((mTarget - mTarget).length() < 1.0){
            mTargetCounter++;
            mTargetCounter %= 2;
        }*/
        
        mTheta += mRotateSpeed;
        
        mPos.update();
    }
    void draw(){
        ofDrawSphere(mTail[0],2);
        for(int i = 1; i < NUM; i++){
            ofLine(mTail[i-1],mTail[i]);
        }
    }
};

class TailSphereController{
private:
    int mTargetID[3];
    static const int NUM = 12;
    vector<TailSphere>mTail;
    bool isEnable = true;
    
public:
    
    bool isInner = false;
    
    
    TailSphereController(){
        for(int i = 0; i < NUM; i++){
            mTail.push_back(TailSphere(ofMap(i,0,NUM,0,TWO_PI),
                                       ofMap(i,0,NUM,0.1,1.0)));
        }
    
    }
    
    void setEnable(bool enable){
        isEnable = enable;
    }
    
    int getTargetID(int i){
        if(i >= 0 && i < 3)return mTargetID[i];
    }
    
    void setTargtID(int a,int b,int c){
        mTargetID[0] = a;
        mTargetID[1] = b;
        mTargetID[2] = c;
    }
    
    void setRotateSpeed(float speed){
        for(auto &v:mTail){
            v.setRotateSpeed(speed);
        }
    }
    
    void setRad(float min,float max){
        for(int i = 0; i < NUM; i++){
            mTail[i].setRad(ofMap(i,0,NUM,min,max));
        }
    }
  
    void draw(ofPoint center,float radius,ofPoint normal){
        
        /*ofPushMatrix();
        ofTranslate(center);
        rotateToNormal(normal);
        ofNoFill();
        ofCircle(0,0,0,radius);
        ofPopMatrix();*/
        
        if(isEnable){
        
        for(auto &v:mTail){
            
            float x = cos(v.mTheta) * radius * v.mRad;
            float y = sin(v.mTheta) * radius * v.mRad;
            
            v.update(ofPoint(x,y));
            
            //v.set(target);
            ofPushMatrix();
            ofTranslate(center);
            rotateToNormal(normal);
            v.draw();
            ofPopMatrix();
        }
            
            cout << radius << endl;
        
        }
    }
};

class ThreePointFlow : public ramBaseScene
{
    
public:
	
	string getName() const { return "ThreePointFlow"; }
    
	void setupControlPanel()
	{
        
        for(auto &v:isEnable){
            v = true;
        }
        
        ramGetGUI().addToggle("under", &isEnable[0]);
        ramGetGUI().addToggle("left", &isEnable[1]);
        ramGetGUI().addToggle("right", &isEnable[2]);
        ramGetGUI().addToggle("isShowActor", &isShowActor);
        ramGetGUI().addToggle("isCentered", &isCentered);
        ramGetGUI().addSlider("speed", 0.01, 0.3, &mRotSpeed);
        ramGetGUI().addSlider("radMin", 0.01, 4.0, &mRadMin);
        ramGetGUI().addSlider("radMax", 0.01, 4.0, &mRadMax);

         ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &ThreePointFlow::onPanelChanged);
        // ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &FMCube::onPanelChanged);
        
    }
    
	void setup()
	{
        
        for(int i = 0; i < NUM; i++){
            mController.push_back(TailSphereController());
        }
        
        mController[0].setTargtID(ramActor::JOINT_LEFT_TOE, ramActor::JOINT_ABDOMEN, ramActor::JOINT_RIGHT_TOE);
        mController[1].setTargtID(ramActor::JOINT_RIGHT_ANKLE, ramActor::JOINT_CHEST, ramActor::JOINT_LEFT_WRIST);
        mController[2].setTargtID(ramActor::JOINT_RIGHT_COLLAR, ramActor::JOINT_ABDOMEN, ramActor::JOINT_RIGHT_WRIST);
        //mController[0].isInner = true;
    }
    
    void updatWithOscMessage(ofxOscMessage &m){
        
    }
    
	void update()
	{
        for(int i = 0; i < NUM; i++){
            mController[i].setEnable(isEnable[i]);
        }
	}
    
	void draw()
	{
        
	}
    
	void drawActor(const ramActor& actor)
	{
        
        ramCenteredActor centered;
        ramActor tmp;
        if(isCentered)tmp = centered.update(actor);
        else tmp = actor;
        
        for(auto &v:mController){
            
        ofPoint a = tmp.getNode(v.getTargetID(0)).getGlobalPosition();
        ofPoint b = tmp.getNode(v.getTargetID(1)).getGlobalPosition();
        ofPoint c = tmp.getNode(v.getTargetID(2)).getGlobalPosition();
        
        ofPoint center;
        ofPoint normal;
        float radius;
        //ofTriangle(a,b,c);
        
        if(v.isInner)calcInCircle(a,b,c,center,normal,radius);
        else findCircle(a,b,c,center,normal,radius);
        
        ofNoFill();
        
            v.draw(center,radius,normal);
        }
        
        /*for(auto &v:mTail){
            
            float x = cos(v.mTheta) * radius * v.mRad;
            float y = sin(v.mTheta) * radius * v.mRad;
            
            v.update(ofPoint(x,y));
            
            //v.set(target);
            ofPushMatrix();
            ofTranslate(center);
            rotateToNormal(normal);
            v.draw();
            ofPopMatrix();
        }*/
        
        if(isShowActor){
            ofFill();
            ramDrawBasicActor(tmp);
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
		
        if(name == "speed"){
            for(auto &v:mController ){
                v.setRotateSpeed(mRotSpeed);
            }
        }
        
        if(name == "radMin" || name == "radMax"){
            for(int i = 0; i < mController.size(); i++){
                mController[i].setRad(mRadMin, mRadMax);
            }
        }
		
	}

    void calcInCircle(ofPoint A,ofPoint B,ofPoint C,ofPoint &center,ofPoint &normal,float &rad){
   
        ofVec3f ab = A - B, cb = C - B;
        normal = ab.getCrossed(cb);
        normal.normalize();
        
        float c = (A - B).length();
        float a = (B - C).length();
        float b = (C - A).length();
        
        float sum = a + b + c;
        
        center = ofPoint((A.x * a + B.x * b + C.x * c) / sum,
                            (A.y * a + B.y * b + C.y * c) / sum,
                         (A.z * a + B.z * b + C.z * c) / sum);
        
        float m = sum * 0.5;
        float S = sqrt((m - a) * (m - b) * (m - c) * m);
        rad = S / ((sum) * 0.5);
        
       
    }
	
private:
    static const int NUM = 3;
    vector<TailSphereController>mController;
    bool isInner = true;
    bool isEnable[3];
    bool isShowActor = false;
    bool isCentered = true;
    float mRotSpeed = 0.1;
    float mRadMin = 0.1;
    float mRadMax = 1.0;
};



#endif
