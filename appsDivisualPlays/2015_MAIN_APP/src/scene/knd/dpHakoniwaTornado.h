//
//  dpHakoniwaTornade.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/14.
//
//

#ifndef RAMDanceToolkit_dpHakoniwaTornade_h
#define RAMDanceToolkit_dpHakoniwaTornade_h

class dpHakoniwaTornado : public ramBaseScene{
public:
    
    string getName() const {return "dpHTornado";};
    
    void setupControlPanel(){
        ramGetGUI().addIntSlider("fan",0,255,&mFan);
        ramGetGUI().addToggle("bulb",&isBulb);
        ramGetGUI().addToggle("mist",&isMist);
        ramGetGUI().addSlider("fanRadMin",0.0,300.0,&mFanRadMin);
        ramGetGUI().addSlider("fanRadMax",0.0,300.0,&mFanRadMax);
        ramGetGUI().addSlider("mistThresh",10.0, 255, &mMistThresh);
        
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &dpHakoniwaTornado::onPanelChanged);
        
        mMotionExtractor.setupControlPanel(this,ofPoint(330,30));
        mMotionExtractor.load("motionExt_dpHakoniwaTornado.xml");
    }
    void setup(){
        mBulbSender.setup("192.168.20.70",8528);
        mFanSender.setup("192.168.20.71",8528);
        
    }
    
    void onPanelChanged(ofxUIEventArgs& e){
        string name = e.widget->getName();
        
        if(name == "fan")fan();
        
        if(name == "bulb")bulb();
        
        if(name == "mist")bulb();
    }
    
    void fan(){
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/tornado/fan");
        m.addIntArg(mFan);
        mFanSender.sendMessage(m);
        
    }
    
    void mist(){
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/tornado/mist");
        m.addIntArg((int)isMist);
        mBulbSender.sendMessage(m);
    }
    
    void bulb(){
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/tornado/bulb");
        m.addIntArg((int)isBulb);
        m.addIntArg((int)isMist);
        mBulbSender.sendMessage(m);
   
    }
    
    void update(){
        
        mMotionExtractor.update();
        bulb();
       /* fan();
        mist();
        cout << isBulb << endl;
        bulb();*/
    }
    
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
    
    void draw(){
        
        ramSetViewPort(dpGetFirstScreenViewPort());
        
        ofPoint a = mMotionExtractor.getPositionAt(0);
        ofPoint b = mMotionExtractor.getPositionAt(1);
        ofPoint c = mMotionExtractor.getPositionAt(2);
        
        ofPoint center;
        ofPoint normal;
        float radius;
        
        findCircle(a,b,c,center,normal,radius);
        
        mFan = ofMap(radius,mFanRadMin,mFanRadMax,0,255,true);
        
        if(mFan > mMistThresh){
            isMist = true;
        }else{
            isMist = false;
        }
        
        ramBeginCamera();
        ofPushMatrix();
        ofTranslate(center);
        rotateToNormal(normal);
        ofNoFill();
        ofCircle(0, 0, radius);
        ofPopMatrix();
        ramEndCamera();
    }
    
    void onDisabled(){
        isBulb = false;
        isMist = false;
        mFan = 0;
        
        fan();
        bulb();
        mist();
    }
    
private:
    ofxOscSender mBulbSender;
    ofxOscSender mFanSender;
    
    int mFan = 0;
    bool isBulb = false;
    bool isMist = false;
    
    ramMotionExtractor mMotionExtractor;
    
    float mFanRadMin = 15;
    float mFanRadMax = 45;
    float mMistThresh = 30.0;;
    
};

#endif
