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
    ramNode rn[2];
    rn[0] = motionExtractor.getNodeAt(0);
    rn[1] = motionExtractor.getNodeAt(1);
    
    
//    ofPushMatrix();
//    
//    {
//        ofTranslate(100, 0);
//        ofVec3f v = rn[1].getGlobalPosition() - rn[0].getGlobalPosition();
//
//        ofQuaternion q;
//        q.makeRotate(v, ofVec3f(1,0,0));
//        float tA, tX, tY, tZ;
//        q.getRotate(tA, tX, tY, tZ);
//
//        ofRotate(tA, tX, tY, tZ);
//
//        ofPushMatrix();
//        ofTranslate(-1*rn[0].getGlobalPosition());
//        rn[0].draw();
//        rn[1].draw();
//        ofSetColor(255);
//        ofLine(rn[0].getGlobalPosition(), rn[1].getGlobalPosition());
//        ofPopMatrix();
//    }
//    
//    ofPopMatrix();
    
    
    ofPushMatrix();
    {
        ofTranslate(100,0);
        ofVec3f v = rn[1].getGlobalPosition() - rn[0].getGlobalPosition();
        
        ofSetColor(255);
        ofLine (rn[0].getGlobalPosition(), rn[0].getGlobalPosition() + (v * 4));

        ofQuaternion q = rn[1].getGlobalOrientation();
        float tA, tX, tY, tZ;
        q.getRotate(tA, tX, tY, tZ);

        ofVec3f rdVec = ofVec3f(0,200,0);
        ofVec3f rVec = rdVec.getRotated(tA, ofVec3f(tX, tY, tZ));
                                        
        ofSetColor(0, 255, 0);
        ofLine (rn[1].getGlobalPosition(), rn[1].getGlobalPosition() + (rVec * 30));
        
        ofSetColor(255);
        
        rn[0].draw();
        rn[1].draw();
    
//        ofNoFill();
//        ofTranslate(rn[1].getGlobalPosition());
//        ofPushMatrix();
//        {
//            ofQuaternion q2;
//            q2.makeRotate(rn[1].getYAxis(), v.normalize());
////            cout << rn[1].getYAxis() << endl;
//            
//            float tA2, tX2, tY2, tZ2;
//            q2.getRotate(tA2, tX2, tY2, tZ2);
//            ofRotate(tA2, tX2, tY2, tZ2);
//
////            cout << tA2 << ", " << tX2 << ", " << tY2 << ", " << tZ2 << endl;
//            ofTranslate(-1*rn[1].getGlobalPosition());
//            rn[1].draw();
//        }
//        ofPopMatrix();
    }
    ofPopMatrix();

    
    

    
    
    ofPushMatrix();
    {
        ofTranslate(200,0);
        ofVec3f v = rn[1].getGlobalPosition() - rn[0].getGlobalPosition();
        
        ofSetColor(255);
        ofLine (rn[0].getGlobalPosition(), rn[0].getGlobalPosition() + (v * 4));
        
        ofQuaternion q2 = rn[1].getGlobalOrientation();
        float tA, tX, tY, tZ;
        q2.getRotate(tA, tX, tY, tZ);
        
        ofVec3f rdVec = ofVec3f(0,200,0);
        ofVec3f rVec = rdVec.getRotated(tA, ofVec3f(tX, tY, tZ));
        
        ofSetColor(0, 255, 0);
        ofLine (rn[1].getGlobalPosition(), rn[1].getGlobalPosition() + (rVec * 100));
        
        ofSetColor(255);
        
        rn[0].draw();
        
        ofQuaternion q3;
        
        rn[1].draw();
        
    }
    ofPopMatrix();
    
    
    
    
    
    

    
//    ofPushMatrix();
//    {
//    
//        ofTranslate(200,0);
//        ofVec3f v = rn[1].getGlobalPosition() - rn[0].getGlobalPosition();
//        
//        ofQuaternion q;
//        q.makeRotate(v, ofVec3f(1,0,0));
//        float tA, tX, tY, tZ;
//        q.getRotate(tA, tX, tY, tZ);
//        
//        ofRotate(tA, tX, tY, tZ);
//        
//        ofPushMatrix();
//        {
//            ofTranslate(-1*rn[0].getGlobalPosition());
//            rn[0].draw();
//            
//            ofPushMatrix();
//            {
//                ofTranslate(rn[1].getGlobalPosition());
//                ofPushMatrix();
//                {
//                    ofQuaternion q2;
//                    q2.makeRotate(-v, rn[1].getYAxis());
//                    float tA2, tX2, tY2, tZ2;
//                    q2.getRotate(tA2, tX2, tY2, tZ2);
//                    ofRotate(tA2, tX2, tY2, tZ2);
//                    
////                    cout << tA2 << ", " << tX2 << ", " << tY2 << ", " << tZ2 << endl;
//                    ofTranslate(-1*rn[1].getGlobalPosition());
//                    rn[1].draw();
//                }
//                    
//                ofPopMatrix();
//            }
//            ofPopMatrix();
//
//                ofSetColor(255);
//                ofLine(rn[0].getGlobalPosition(), rn[1].getGlobalPosition());
//        }
//        ofPopMatrix();
//    
//    
//    }
//    ofPopMatrix();
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    

//        ofPushMatrix();
//        {
//            ofVec3f v = rn[1].getGlobalPosition() - rn[0].getGlobalPosition();
//            ofQuaternion q = rn[1].getGlobalOrientation();
//            float tA, tX, tY, tZ;
//            q.getRotate(tA, tX, tY, tZ);
//        
//            ofVec3f Yaxis = ofVec3f(100,0,0);
//            ofVec3f YaxisR = Yaxis.getRotated(tA, ofVec3f(tX, tY, tZ));
//        
//            ofSetColor(255);
//            ofLine(ofVec3f(0,0,0), YaxisR);
//            
//            ofQuaternion q2;
//            q2.makeRotate(Yaxis, -v);
//            q2.getRotate(tA, tX, tY, tZ);
//            
//            ofRotate(tA, tX, tY, tZ);
//            
//            ofPushMatrix();
//            ofTranslate(-1*rn[1].getGlobalPosition());
//            rn[1].draw();
//            
//            ofPopMatrix();
//            
//        }
//        ofPopMatrix();
    
        
        
        
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
//    }
}
