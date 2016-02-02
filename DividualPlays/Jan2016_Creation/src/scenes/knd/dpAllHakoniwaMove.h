//
//  dpAllHakoniwaMove.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2016/01/27.
//
//

#ifndef RAMDanceToolkit_dpAllHakoniwaMove_h
#define RAMDanceToolkit_dpAllHakoniwaMove_h

#include "dpServoPendulumThread.h"
#include "ofxKsmrStepManager.h"

class dpAllHakoniwaMove : public ramBaseScene{
public:
    string getName() const {return "dpAllHakoniwaMove";};
    
    void setupControlPanel(){
        
        ramGetGUI().addToggle("MagLooper", &mIsMagLooper);
        ramGetGUI().addToggle("MagPendulum", &mIsMagPendulum);
        ramGetGUI().addToggle("Struggle", &mIsStruggle);
        ramGetGUI().addToggle("SandStorm", &mIsSandStorm);
        ramGetGUI().addToggle("ServoPendulum", &mIsServoPendulum);
        ramGetGUI().addToggle("Gear", &mIsGear);
        ramGetGUI().addToggle("Tornade", &mIsTornade);
        
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &dpAllHakoniwaMove::onPanelChanged);

    }
    
    void onPanelChanged(ofxUIEventArgs& e){
        const string name = e.widget->getName();
        
        sendMagPendulum(mIsMagPendulum);
        sendStruggle(mIsStruggle);
        sendSandStorm(mIsSandStorm);
        sendServoPendulum();
        sendTornade(mIsTornade);
        sendMaglooper(mIsMagLooper);
        sendGear(mIsGear);
        
        if (name == "ServoPendulum") {
            if(mIsServoPendulum){
                
                mServoThread.start();
                
            }else{
                
                 mServoThread.stop();
                
            }
        }
        
    }
    
    void setup(){
        
        mSender[MAG_PENDULUM].setup(MAG_PENDULUM_ONOFF_IP, 8528);
        mSender[STRUGGLE].setup(STRUGGLE_IP, 8528);
        mSender[SAND_STORM].setup(SAND_STORM_IP, 8528);
       // mSender[SERVO_PENDULUM].setup(SERVO_PENDULUM_IP,8528);
        mSender[TORNADE].setup(TORNADE_IP,8528);
        
        mSender[MAG_LOOPER_1].setup(MAGLOOPER_IP,MAG_LOOPER_1_PORT);
        mSender[MAG_LOOPER_2].setup(MAGLOOPER_IP,MAG_LOOPER_2_PORT);
        
        mSender[GEAR].setup(GEAR_IP, 8528);
        
        mServoThread.setup();
        
        setupStepManager();
        
        mLightSender.setup(LIGHTING_IP,10000);
    
    }
    
    void setupStepManager(){
        
        mStepManager.setupOsc(GEAR_IP, 8528);
        mStepManager.sendByteSimply = true;
        
        mStepManager.addStepper("unit1", 400, 0);
        mStepManager.addStepper("unit2", 400, 1);
        mStepManager.addStepper("unit3", 400, 2);
        mStepManager.resetAllDevices();
        
        mStepManager.setParam_maxSpeed(0x0075);
        mStepManager.setParam_Accel(0x0010);
        mStepManager.setParam_Decel(0x0010);
        mStepManager.setMicroSteps(7);
        
        mStepManager.setMicroSteps(5);
        
        mStepManager.setStepperAll(true);
        mStepManager.absPos(0);
        mStepManager.hardStop();
        
    }
    
    void sendMagPendulum(bool enable){
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/magpen");
        
        int frame = ofGetFrameNum() % 60;
        
        if(enable){
            
            m.addIntArg(0);
            m.addIntArg(0);
            m.addIntArg(frame > 30);
            m.addIntArg(frame > 50);
            m.addIntArg(frame < 30);
            m.addIntArg(0);
            
        }else{
            
            m.addIntArg(0);
            m.addIntArg(0);
            m.addIntArg(0);
            m.addIntArg(0);
            m.addIntArg(0);
            m.addIntArg(0);
            
        }
        
        mSender[MAG_PENDULUM].sendMessage(m);

    }
    
    void sendStruggle(bool enable){
        
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/struggle");
      
        if(ofGetFrameNum() % 60 > 30){
            m.addIntArg(0);
            m.addIntArg(1);
        }else{
            m.addIntArg(1);
            m.addIntArg(0);
        }
        
        if(enable){
            m.addIntArg(255);
        }else{
            m.addIntArg(0);
        }
        
        mSender[STRUGGLE].sendMessage(m);
    }
    
    void sendSandStorm(bool enable){
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/sand");
        
        float val[3];
        
        if(enable){
            val[0] = ofMap(cos(ofGetFrameNum() * 0.05),-1.0,1.0,750, 2170);
            val[1] = ofMap(cos(ofGetFrameNum() * 0.03),-1.0,1.0,750, 2170);
            val[2] = ofMap(cos(ofGetFrameNum() * 0.08),-1.0,1.0,750, 2170);
        
            m.addIntArg(val[0]); // pulse width(usec) for motor1
            m.addIntArg(val[1]); // pulse width(usec) for motor2
            m.addIntArg(val[2]); // pulse width(usec) for motor3
        
        }else{
            
            for(auto &v:val){
                v = 0;
            }
            
        }
        
        mSender[SAND_STORM].sendMessage(m);
    }
    
    void sendGear(bool enable){
        
        if(enable){
            mStepManager.setStepperAll(true);
            mStepManager.run(11000, true);
            mStepManager.setStepperAll(false);
        }else{
            mStepManager.setStepperAll(true);
            mStepManager.hardStop();
            mStepManager.setStepperAll(false);
        }
        
    }
    
    void sendServoPendulum(){
        mServoThread.setSpeed(1.0);
        mServoThread.setRange(30);
    }
    
    void sendTornade(bool enable){
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/tornado/fan");
        m.addIntArg(enable * 255);
        mSender[TORNADE].sendMessage(m);
    }
    
    void sendMaglooper(bool enable){
        if(enable){
            if(ofGetFrameNum() % 10 == 0){
                
                ofxOscMessage m;
                m.setAddress( "/dp/hakoniwa/magnetLooper" );
                m.addIntArg(0);
                m.addIntArg(0);
                m.addIntArg(int(20));
                mSender[MAG_LOOPER_1].sendMessage(m);
                
                ofxOscMessage n;
                n.setAddress( "/dp/hakoniwa/magnetLooper2" );
                n.addIntArg(0);
                n.addIntArg(0);
                n.addIntArg(int(20));
                
                mSender[MAG_LOOPER_2].sendMessage(n);
            }
        }else{
            
            ofxOscMessage m;
            m.setAddress( "/dp/hakoniwa/magnetLooper" );
            m.addIntArg(1);
            m.addIntArg(1);
            m.addIntArg(int(20));
            mSender[MAG_LOOPER_1].sendMessage(m);
            
            ofxOscMessage n;
            n.setAddress( "/dp/hakoniwa/magnetLooper2" );
            n.addIntArg(1);
            n.addIntArg(1);
            n.addIntArg(int(20));
            
            mSender[MAG_LOOPER_2].sendMessage(n);

        }
    }
    
    void sendAll(bool enable){
        sendMagPendulum(enable);
        sendStruggle(enable);
        sendSandStorm(enable);
        sendServoPendulum();
        sendTornade(enable);
        sendMaglooper(enable);
        sendGear(enable);
    }
    
    void update(){
    }
    void receiveOsc(){}
    
    void onEnabled(){
        
        mServoThread.start();
        sendAll(true);
        
        mIsMagLooper = true;
        mIsMagPendulum = true;
        mIsStruggle = true;
        mIsSandStorm = true;
        mIsServoPendulum = true;
        mIsGear = true;
        mIsTornade = true;
        
        sendLightingMessage(true);
    }
    
    void onDisabled(){
        sendAll(false);
        sendAll(false);
        mServoThread.stop();
        sendLightingMessage(false);
    }
    
    void sendLightingMessage(bool enable){
        ofxOscMessage m;
        m.setAddress("/dp/light/allHakoniwa");
        m.addIntArg(enable);
        mLightSender.sendMessage(m);
    }
    
private:

    enum HAKONIWA_SENDER_NAME{
        MAG_LOOPER_1,
        MAG_LOOPER_2,
        MAG_PENDULUM,
        STRUGGLE,
        SAND_STORM,
        GEAR,
    //    SERVO_PENDULUM,
        TORNADE,
        SENDER_NUM
    };
    
    ofxOscSender mSender[SENDER_NUM];
    
    dpServoPendulumThread mServoThread;
    
    bool mIsMagLooper;
    bool mIsMagPendulum;
    bool mIsStruggle;
    bool mIsSandStorm;
    bool mIsServoPendulum;
    bool mIsGear;
    bool mIsTornade;
    
    ofxKsmrStepManager		mStepManager;
    
    ofxOscSender mLightSender;
    
};

#endif
