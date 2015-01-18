//
//  twistFinder.cpp
//  example-ramMotionExtractor
//
//  Created by itotaka on 1/17/15.
//
//

#include "twistFinder.h"

void twistFinder::findTwist(ramNode & node, ramNode & nodeOrigin){  // check rotation on a line between A & B

    ofQuaternion q;
    ofQuaternion qOrigin;

    ofVec3f v = node.getPosition() - nodeOrigin.getPosition();
    q.makeRotate(node.getZAxis(), v.normalize());
    qOrigin.makeRotate(nodeOrigin.getZAxis(), v.normalize());

}

void twistFinder::debugDraw(ramMotionExtractor & motionExtractor){

    //experimental
    
    
    ramNode node[ramActor::NUM_JOINTS];
    ramNodeArray na;

    if (ramActorManager::instance().getNumNodeArray() > 0) {
        na = ramActorManager::instance().getNodeArray(0);
        
        node[0] = na.getNode(ramActor::JOINT_LEFT_COLLAR);
        node[1] = na.getNode(ramActor::JOINT_LEFT_SHOULDER);
        node[2] = na.getNode(ramActor::JOINT_LEFT_ELBOW);
        node[3] = na.getNode(ramActor::JOINT_LEFT_WRIST);

        ofNoFill();
        for (int i = 0; i < 4; i++) node[i].draw();
        
//        ofPushMatrix();
//        
//        {
//            ofTranslate(100, 0);
//            ofVec3f v = node[3].getGlobalPosition() - node[2].getGlobalPosition();
//
//            ofQuaternion q;
//            q.makeRotate(v, ofVec3f(1,0,0));
//            float tA, tX, tY, tZ;
//            q.getRotate(tA, tX, tY, tZ);
//
//            ofRotate(tA, tX, tY, tZ);
//
//            ofPushMatrix();
//            ofTranslate(-1*node[2].getGlobalPosition());
//            node[2].draw();
//            node[3].draw();
//            ofSetColor(255);
//            ofLine(node[2].getGlobalPosition(), node[3].getGlobalPosition());
//            ofPopMatrix();
//        }
//        
//        ofPopMatrix();
//
        

        ofPushMatrix();
        {
            ofVec3f v = node[3].getGlobalPosition() - node[2].getGlobalPosition();
            ofQuaternion q = node[3].getGlobalOrientation();
            float tA, tX, tY, tZ;
            q.getRotate(tA, tX, tY, tZ);
        
            ofVec3f Yaxis = ofVec3f(0,100,0);
            Yaxis.getRotated(tA, ofVec3f(tX, tY, tZ));
        
            ofSetColor(255);
            ofLine(ofVec3f(0,0,0), Yaxis);
            
            ofQuaternion q2;
            q2.makeRotate(Yaxis, -v);
            q2.getRotate(tA, tX, tY, tZ);
            
            ofRotate(tA, tX, tY, tZ);
            
            ofPushMatrix();
            ofTranslate(-1*node[3].getGlobalPosition());
            node[3].draw();
            
            ofPopMatrix();
            
            
            
        }
        ofPopMatrix();
        
        
        
        
        
        
        
        
        
        
//        ofPushMatrix();
//        {
//            
//            ofTranslate(100, 0);
//            ofVec3f v = node[3].getGlobalPosition() - node[2].getGlobalPosition();
//            {
//                ofQuaternion q;
//                q.makeRotate(v, ofVec3f(1,0,0));
//                float tA, tX, tY, tZ;
//                q.getRotate(tA, tX, tY, tZ);
//                ofRotate(tA, tX, tY, tZ);
//            }
//            ofPushMatrix();
//            {
//                ofTranslate(-1*node[2].getGlobalPosition());
//                node[2].draw();
//            
//                ofTranslate(node[3].getGlobalPosition());
//                ofPushMatrix();
//                {
//                    {
//                        ofQuaternion q;
//                        q.makeRotate(node[3].getYAxis(), -v);
//                        float tA, tX, tY, tZ;
//                        q.getRotate(tA, tX, tY, tZ);
//                        ofRotate(tA, tX, tY, tZ);
//                    }
//                    ofTranslate(-1*node[3].getGlobalPosition());
//                    node[3].draw();
//                }
//                ofPopMatrix();
//                
////            ofSetColor(255);
////            ofLine(node[2].getGlobalPosition(), node[3].getGlobalPosition());
////            ofPopMatrix();
//            }
//            ofPopMatrix();  // ofRotate
//
//        }
//        ofPopMatrix();      //ofTranslate(100,0)
//    }
    }
}
