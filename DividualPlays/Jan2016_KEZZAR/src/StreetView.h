//
//  StreetView.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2014/12/20.
//
//

#ifndef RAMDanceToolkit_StreetView_h
#define RAMDanceToolkit_StreetView_h

#include "ofxStreetView.h"
#include "SlidePoint.h"

class StreetView : public ramBaseScene{
public:
    
    string getName() const { return "StreetView"; }
    
	void setupControlPanel()
	{
        ramGetGUI().addSlider("z", 0.0, -500.0, &mZ);
        ramGetGUI().addSlider("changeThresh", 1.0, 30.0, &mChangeThresh);
        ramGetGUI().addButton("change");
        
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &StreetView::onPanelChanged);
    }
    
    void setup(){
        for(int i = 0; i < 2; i++){
            mStreetView[i].setLatLon(34.170173,131.466739);  // ycam
            mStreetView[i].setZoom(3);
            mAlpha[i].imSet(0);
            mAlpha[i].speed = 0.2;
        }
        
        mAlpha[mActiveNum].imSet(1.0);
        //ofSetLogLevel(OF_LOG_VERBOSE);
    }
    void update(){
        for(int i = 0; i < 2; i++){
            mStreetView[i].update();
            mAlpha[i].update();
        }
        
        bool check = mStreetView[0].getLoadedAgain();
        
      //  cout << check << endl;
        
        if(check){
            
            mAlpha[0].set(255.0);
            mAlpha[1].set(0.0);
        }
        
        if(mStreetView[1].getLoadedAgain()){
            
            mAlpha[0].set(0.0);
            mAlpha[1].set(255.0);
        }
        
        
        ramSetViewPort(getFirstViewPort());
    }
    void draw(){
        
        ofEasyCam cam;
        cam.begin(getSecondViewPort());
        
        int numActor = getNumNodeArray();
        
        for(int i = 0; i < numActor; i++){
            const ramNodeArray &array = getNodeArray(i);
    
            for(int j = 0; j < array.getNumNode(); j++){
                const ramNode node = array.getNode(j);
            
                if(j == ramActor::JOINT_NECK){
                    
                    mQuat = node.getGlobalOrientation();
                    
                    if(node.getVelocity().length() > mChangeThresh){
                        changePanoID();
                    }
                    
                    mQuat.getRotate(mAngle, mAxis);
                    
                    ofPushMatrix();
                    //ofTranslate(SINGLE_SCREEN_WIDTH + SINGLE_SCREEN_WIDTH * 0.5,SINGLE_SCREEN_HEIGHT * 0.5,mZ);
                    ofTranslate(0,0,mZ);
                    
                    ofRotateX(mQuat.getEuler().x);
                    ofRotateY(mQuat.getEuler().y * -1.0);
                    ofRotateZ(mQuat.getEuler().z);
                    
                    //ofRotate(mAngle, mAxis.x,mAxis.y,mAxis.z);
                    ofRotateX(90);
                    ofRotateY(180);
                    
                    ofDisableDepthTest();
                    for(int i = 0; i < 2; i++){
                        mStreetView[i].draw(mAlpha[i].val);
                    }
                    
                    ofPopMatrix();
                }
                
                
            }
        }
        
      
    
        cam.end();
      
    }
    
    void changePanoID(){
        
        string str = mStreetView[mActiveNum].getCloseLinkTo(90.0);
        
        if(str != ""){
            //mAlpha[mActiveNum].set(0);
            //cout << "cur" << mActiveNum << endl;
            mActiveNum++;
            mActiveNum %= 2;
            //cout << "next" << mActiveNum << endl;
           // mAlpha[mActiveNum].set(1.0);
            mStreetView[mActiveNum].setLoadedNone();
            mStreetView[mActiveNum].setZoom(3);
            mStreetView[mActiveNum].setPanoId(str);

        }
   
    }
    
    void drawActor(const ramActor& actor){}
    void onPanelChanged(ofxUIEventArgs& e)
	{
		string name = e.widget->getName();
        
        if(name == "change"){
            changePanoID();
        }
	}
private:
    
    ofxStreetView mStreetView[2];
    int mActiveNum = 0;
    
    Slide mAlpha[2];
    
    float mZ = 200.0;
    
    ofPoint mAxis;
    float mAngle;
    ofQuaternion mQuat;
    
    float mChangeThresh = 5.0;
};

#endif
