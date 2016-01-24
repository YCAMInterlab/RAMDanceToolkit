//
//  VectorField.h
//  afterWorkshop
//
//  Created by kezzardrix2 on 2014/12/20.
//
//

#ifndef afterWorkshop_VectorField_h
#define afterWorkshop_VectorField_h


class dpVisMemeB : public ramBaseScene{
public:
    
    string getName() const { return "dpVisMemeB"; }
    
    void setup(){
        
        ramOscManager::instance().addReceiverTag(&mReceiver);
        mReceiver.addAddress("/dp/toVis/memeB");
        
    }
    void setupControlPanel(){
        /*ramGetGUI().addToggle("isDrawVF", &isDrawVF);
         ramGetGUI().addSlider("multHandPos", 1.0, 20.0, &mMult);
         ramGetGUI().addSlider("rad", 50.0, 500.0, &mRad);*/
    }
    
    void receiveOsc(){
        
        while(mReceiver.hasWaitingMessages()){
            
            ofxOscMessage m;
            mReceiver.getNextMessage(&m);
            
            if(m.getAddress() == "/dp/toVis/memeB"){
                x = m.getArgAsFloat(0);
                y = m.getArgAsFloat(1);
            }
            
        }
        
    }
    
    void update(){
        receiveOsc();
    }
    
    void draw(){
        ofCircle(x,y,100);
    }
    
    void drawActor(const ramActor& actor){
        
    }
    
    void onPanelChanged(ofxUIEventArgs& e){}
    
private:
    ramOscReceiveTag mReceiver;
    float x = 0.0;
    float y = 0.0;
};


#endif
