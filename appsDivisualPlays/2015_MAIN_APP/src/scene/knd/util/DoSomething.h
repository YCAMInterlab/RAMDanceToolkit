//
//  DoSomething.h
//  2015_MAIN_APP
//
//  Created by kezzardrix2 on 2015/01/23.
//
//

#ifndef _015_MAIN_APP_DoSomething_h
#define _015_MAIN_APP_DoSomething_h

class DoSomething{
public:
    void setup(){
        mFont.loadFont("../../../resources/fonts/AkkoStd-Thin.otf",FONT_SIZE);
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/ram/do_something");
        
        mStr = "Do Nothing";
    }
    
    void start(){
        mLife = LIFE;
        enable = true;
        mStrCounter = 0;
    }
    
    void receiveOsc(){
        while(mReceiver.hasWaitingMessages()){
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
            
            if(m.getAddress() == "/ram/do_something"){
                start();
                mBeginTime = ofGetElapsedTimef();
            }
        }
    }
    
    void update(){
        
        receiveOsc();
        
        if(enable){
            
            mLife--;
            
            float elapsed = (ofGetElapsedTimef() - mBeginTime);
            
            mStrCounter = ofMap(elapsed,0,1.0,0,mStr.size(),true);
            
            if(mStrCounter > mStr.size())mStrCounter = mStr.size();
        }
        
        if(mLife < 0)enable = false;
    }
    
    void draw(){
        if(enable){
            ofSetColor(255,255,255);
            string str = mStr.substr(0,mStrCounter);
            mFont.drawString(ofToString(str),120,SINGLE_SCREEN_HEIGHT * 0.5 + FONT_SIZE * 0.25);
            mFont.drawString(ofToString(str),120 + SINGLE_SCREEN_WIDTH,SINGLE_SCREEN_HEIGHT * 0.5 + FONT_SIZE * 0.25);
        }
    }
    
private:
    ofTrueTypeFont mFont;
    bool enable = false;
    
    static const int LIFE = 1800;
    int mLife = LIFE;
    
    static const int FONT_SIZE = 200;
    bool isDraw = false;
    
    ramOscReceiveTag mReceiver;
    string mStr;
    int mStrCounter = 0;
    
    float mBeginTime;
};

#endif
