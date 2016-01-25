//
//  MemeAScene.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2016/01/07.
//
//

#ifndef RAMDanceToolkit_MemeAScene_h
#define RAMDanceToolkit_MemeAScene_h

class dpVisMemeA : public ramBaseScene{
public:
    
    string getName() const { return "dpVisMemeA"; }
    
    void setup(){
        
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/toVis/memeA");
        
    }
    void setupControlPanel(){
         ramGetGUI().addToggle("isFill", &isFill);
         ramGetGUI().addSlider("x", 0.0, 1920.0, &x);
         ramGetGUI().addSlider("y", 0.0, 1080.0, &y);
    }
    
    void receiveOsc(){
        
        while(mReceiver.hasWaitingMessages()){
            
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
            
            if(m.getAddress() == "/dp/toVis/memeA"){
                /*cout << m.getArgAsFloat(0) << endl;
                cout << m.getArgAsInt32(0) << endl;
                cout << m.getArgAsString(0) << endl;*/
            }
        }
        
    }
    
    void update(){
        receiveOsc();
    }
    
    void draw(){
        if(isFill)ofFill();
        else ofNoFill();
        ofCircle(x,y,100);
    }
    
    void drawActor(const ramActor& actor){
        
    }
    
    void onPanelChanged(ofxUIEventArgs& e){}
    
private:
    ramOscReceiveTag mReceiver;
    
    float x = 0;
    float y = 0;
    
    bool isFill = true;
};


#endif
