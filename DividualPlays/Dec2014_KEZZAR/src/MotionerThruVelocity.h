//
//  SimpleVelocitySend.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2014/12/22.
//
//

#ifndef RAMDanceToolkit_SimpleVelocitySend_h
#define RAMDanceToolkit_SimpleVelocitySend_h

class MotionerThruVelocity : public ramBaseScene{
public:
    
    string getName() const { return "MotionerThruVelocity"; }
    
    void setup(){
        mSender.setup("192.168.20.37",10012);
    }
    void update(){
        int numActor = getNumNodeArray();
        
        for(int i = 0; i < numActor; i++){
            const ramNodeArray &array = getNodeArray(i);
            
            ofxOscMessage m;
            m.setAddress("/dp/hakoniwa/motioner/velocity");
            m.addIntArg(i);
            for(int j = 0; j < array.getNumNode(); j++){
                const ramNode node = array.getNode(j);
 
                if(j == ramActor::JOINT_NECK
                   || j == ramActor::JOINT_LEFT_ANKLE
                   || j == ramActor::JOINT_RIGHT_ANKLE
                   || j == ramActor::JOINT_LEFT_WRIST
                   || j == ramActor::JOINT_RIGHT_WRIST){
                    
                    m.addFloatArg(node.getVelocity().length());
                }
            
            }
            
            mSender.sendMessage(m);
        }
    }
    void draw(){}
private:
    ofxOscSender mSender;
};

#endif
