//
//  dpHakoniwatornado.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/14.
//
//

#ifndef RAMDanceToolkit_dpHakoniwatornado_h
#define RAMDanceToolkit_dpHakoniwatornado_h

class dpHakoniwaTornado : public ramBaseScene{
public:
    
    string getName() const {return "dpHTornado";};
    
    void setupControlPanel(){
        ramGetGUI().addToggle("fan",&mFan);
        ramGetGUI().addToggle("bulb",&mBulb);
        
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &dpHakoniwaTornado::onPanelChanged);
    }
    void setup(){
        mBulbSender.setup("192.168.20.70",8528);
        mFanSender.setup("192.168.20.71",8528);
        
    }
    
    void onPanelChanged(ofxUIEventArgs& e){
        string name = e.widget->getName();
        
        if(name == "fan")fan();
        
        if(name == "bulb")bulb();
    }
    
    void update(){
        
    }
    
    void fan(){
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/tornado/fan");
        m.addIntArg((int)mFan);
        mFanSender.sendMessage(m);
    }
    
    void bulb(){
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/tornado/bulb");
        m.addIntArg((int)mBulb);
        mBulbSender.sendMessage(m);
    }
    
    void draw(){}
private:
    ofxOscSender mBulbSender;
    ofxOscSender mFanSender;
    
    bool mFan = false;
    bool mBulb = false;
    
};

#endif
