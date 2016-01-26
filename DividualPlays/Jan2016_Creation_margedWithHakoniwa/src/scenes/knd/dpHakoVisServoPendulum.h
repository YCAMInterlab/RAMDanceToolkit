//
//  dpHakoVisRecordRibbon.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/10.
//
//

#ifndef RAMDanceToolkit_dpHakoVisRecordRibbon_h
#define RAMDanceToolkit_dpHakoVisRecordRibbon_h

#include "KezLuminanceFilter.h"
#include "KezAccumlateWeightedFilter.h"
#include "ofxBlur.h"
#include "dpSyphonClientManager.h"

class dpHakoVisServoPendulum : public ramBaseScene{
public:
    string getName() const { return "dpVisServoPendulum"; }
    
	void setupControlPanel(){
        
        ramGetGUI().addSlider("speed",1.0,20.0,&mSpeed);
        ramGetGUI().addSlider("lumi_thresh",0.0,1.0,&mLumiThresh);
        ramGetGUI().addToggle("drawRawCam", &mIsDrawRawCam);
        ramGetGUI().addSlider("offsetX",0.0,SINGLE_SCREEN_WIDTH,&mOffsetX);
        
       // ramGetGUI().addSlider("dst",0.0,1.0,&mDst);
       // ramGetGUI().addSlider("src",0.0,1.0,&mSrc);
        
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &dpHakoVisServoPendulum::onPanelChanged);
    }
    void setup(){
    
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/cameraUnit/ServoPendulum/vector/total");
    
        mAccumlateWeightedFilter.setup();
        
        mFbo.allocate(SINGLE_SCREEN_WIDTH,SINGLE_SCREEN_HEIGHT,GL_RGB32F);
        mBufferFbo.allocate(SINGLE_SCREEN_WIDTH,SINGLE_SCREEN_HEIGHT,GL_RGB32F);
        
        mFbo.begin();
        ofClear(0,0);
        mFbo.end();
        
        mBufferFbo.begin();
        ofClear(0,0);
        mBufferFbo.end();
        
        mBlur.setup(SINGLE_SCREEN_WIDTH,SINGLE_SCREEN_HEIGHT,10,0.2,4);
        mBlur.setBrightness(2.0);
        mBlur.setup(SINGLE_SCREEN_WIDTH, SINGLE_SCREEN_HEIGHT);
        mLumi.setup();

        mFinalFbo.allocate(SINGLE_SCREEN_WIDTH, SINGLE_SCREEN_HEIGHT,GL_RGB);
        
        string frag = STRINGIFY(
                                uniform sampler2DRect src;

                                void main(void){
                                    
                                    vec2 st = gl_TexCoord[0].st;
                                    vec4 color = texture2DRect(src,st);
                                    if(dot(color.rgb,vec3(0.333)) < 0.01)discard;
                                    
                                    gl_FragColor = color;
                                
                                }
                                );
        mDiscards.setupShaderFromSource(GL_FRAGMENT_SHADER, frag);
        mDiscards.linkProgram();
    }
    
    void receiveOsc(){
        
        while(mReceiver.hasWaitingMessages()){
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
        
        }
    }
    
    void update(){
        
        receiveOsc();
        
        mAccumlateWeightedFilter.setSpeed(mSpeed);
        mLumi.setThresh(mLumiThresh);
   
    }
    
  
    
    void draw(){

        ofDisableDepthTest();
        mFbo.begin();
        ofClear(0);
        ofSetColor(255);
        dpSyphonClientManager::instance().drawWithSideCrop(mOffsetX, 0);
        mFbo.end();
        
        mAccumlateWeightedFilter.begin();
        
        mLumi.begin(mBufferFbo.getTextureReference());
        mFbo.draw(0,0,SINGLE_SCREEN_WIDTH,SINGLE_SCREEN_HEIGHT);
        mLumi.end();
        
        mAccumlateWeightedFilter.end();
        mAccumlateWeightedFilter.process();
        
        mBlur.begin();
        ofClear(0);
        ofPushMatrix();
        ofScale(1.0,1.0);
        mAccumlateWeightedFilter.draw();
        ofPopMatrix();
        mBlur.end();
        
        
        mFinalFbo.begin();
        ofClear(0,0);
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        mAccumlateWeightedFilter.draw();
        mBlur.draw();
        if(mIsDrawRawCam)dpSyphonClientManager::instance().drawWithSideCrop(mOffsetX, 0);
        mFinalFbo.end();
        
        mDiscards.begin();
        mFinalFbo.draw(0,0);
        mDiscards.end();
        
        drawToBuffer();
 
    }
    
    
    
    void onEnabled(){
        drawToBuffer();
        mAccumlateWeightedFilter.clear();
    }
    
    void onPanelChanged(ofxUIEventArgs& e){
        
        string name = e.widget->getName();
    

    }
    
private:

    ofFbo mFbo;
    ofFbo mBufferFbo;
    
    KezLuminanceFilter mLumi;
    KezAccumlateWeightedFilter mAccumlateWeightedFilter;
    
    ramOscReceiveTag mReceiver;
    
    ofxBlur mBlur;
    
    float mSpeed = 3.0;
    float mLumiThresh = 0.1;
    bool mIsDrawRawCam = true;
    float mOffsetX = 0.0;
    
    void drawToBuffer(){
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        mBufferFbo.begin();
        ofClear(0);
        ofSetColor(255);
        dpSyphonClientManager::instance().drawWithSideCrop(mOffsetX, 0);
        mBufferFbo.end();
    }
    
    ofFbo mFinalFbo;
    ofShader mDiscards;
};

#endif
