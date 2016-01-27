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

class dpAllHakoniwaMove : public ramBaseScene{
public:
    string getName() const {return "dpAllHakoniwaMove";};
    void setupControlPanel(){}
    void setup(){
        
        mSender[MAG_PENDULUM].setup(MAG_PENDULUM_ONOFF_IP,8528);
        mSender[STRUGGLE].setup(STRUGGLE_IP,8528);
        mSender[SAND_STORM].setup(SAND_STORM_IP,8528);
       // mSender[SERVO_PENDULUM].setup(SERVO_PENDULUM_IP,8528);
        mSender[TORNADE].setup(TORNADE_IP,8528);
        
        mSender[MAGLOOPER_1].setup(MAGLOOPER_IP,MAGLOOPER_1_PORT);
        mSender[MAGLOOPER_2].setup(MAGLOOPER_IP,MAGLOOPER_2_PORT);
        
        mServoThread.setup();
        
    
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
      
        m.addIntArg(0);
        m.addIntArg(1);
        
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
                mSender[MAGLOOPER_1].sendMessage(m);
                
                ofxOscMessage n;
                n.setAddress( "/dp/hakoniwa/magnetLooper2" );
                n.addIntArg(0);
                n.addIntArg(0);
                n.addIntArg(int(20));
                
                mSender[MAGLOOPER_2].sendMessage(n);
            }
        }else{
            
            ofxOscMessage m;
            m.setAddress( "/dp/hakoniwa/magnetLooper" );
            m.addIntArg(1);
            m.addIntArg(1);
            m.addIntArg(int(20));
            mSender[MAGLOOPER_1].sendMessage(m);
            
            ofxOscMessage n;
            n.setAddress( "/dp/hakoniwa/magnetLooper2" );
            n.addIntArg(1);
            n.addIntArg(1);
            n.addIntArg(int(20));
            
            mSender[MAGLOOPER_2].sendMessage(n);

        }
    }
    
    void sendAll(bool enable){
        sendMagPendulum(enable);
        sendStruggle(enable);
        sendSandStorm(enable);
        sendServoPendulum();
        sendTornade(enable);
        sendMaglooper(enable);
    }
    
    void update(){
        sendAll(true);
    }
    void receiveOsc(){}
    
    void onEnabled(){
        mServoThread.start();
    }
    void onDisabled(){
        sendAll(false);
        sendAll(false);
        mServoThread.stop();
    }
    
private:

    enum HAKONIWA_NAME{
        MAGLOOPER_1,
        MAGLOOPER_2,
        MAG_PENDULUM,
        STRUGGLE,
        SAND_STORM,
    //    SERVO_PENDULUM,
        TORNADE,
        HAKONIWA_NUM
    };
    
    ofxOscSender mSender[HAKONIWA_NUM];
    
    dpServoPendulumThread mServoThread;
    
};

#endif
