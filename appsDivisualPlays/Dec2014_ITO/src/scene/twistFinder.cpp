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
    
    ofPushMatrix();
    {
        ofVec3f v = rn[1].getGlobalPosition() - rn[0].getGlobalPosition();

        // rn[0]
        ofPushMatrix();
        {
            ofQuaternion q = rn[0].getGlobalOrientation();
            float tA, tX, tY, tZ;
            q.getRotate(tA, tX, tY, tZ);
            
            ofVec3f grVec = ofVec3f(0, 1, 0);
            ofVec3f gVec = grVec.getRotated(tA, ofVec3f(tX, tY, tZ));
            ofVec3f redVec = ofVec3f(1, 0, 0);
            ofVec3f rVec = redVec.getRotated(tA, ofVec3f(tX, tY, tZ));
            ofVec3f blueVec = ofVec3f(0, 0, 1);
            ofVec3f bVec = blueVec.getRotated(tA, ofVec3f(tX, tY, tZ));
            
            q.makeRotate(gVec.getNormalized(), v.getNormalized());
            float tA2, tX2, tY2, tZ2;
            q.getRotate(tA2, tX2, tY2, tZ2);
            
            gVec = gVec.getRotated(tA2, ofVec3f(tX2, tY2, tZ2));
            rVec = rVec.getRotated(tA2, ofVec3f(tX2, tY2, tZ2));
            bVec = bVec.getRotated(tA2, ofVec3f(tX2, tY2, tZ2));
            
            q.makeRotate(v, ofVec3f(1,0,0));
            float tA3, tX3, tY3, tZ3;
            q.getRotate(tA3, tX3, tY3, tZ3);
            
            gVec = gVec.getRotated(tA3, ofVec3f(tX3, tY3, tZ3));
            rVec = rVec.getRotated(tA3, ofVec3f(tX3, tY3, tZ3));
            bVec = bVec.getRotated(tA3, ofVec3f(tX3, tY3, tZ3));
            
            ofPushMatrix();
            ofSetColor(255);
            ofSetColor(0, 255, 0);
            ofLine(ofVec3f(0,0,0), gVec * 300);
            ofSetColor(255, 0, 0);
            ofLine(ofVec3f(0,0,0), rVec * 300);
            ofSetColor(0, 0, 255);
            ofLine(ofVec3f(0,0,0), bVec * 300);
            ofPopMatrix();
        }
        ofPopMatrix();
        
        // rn[1]
        ofPushMatrix();
        {
            ofTranslate(200, 0);
            
            ofQuaternion q = rn[1].getGlobalOrientation();
            float tA, tX, tY, tZ;
            q.getRotate(tA, tX, tY, tZ);

            ofVec3f grVec = ofVec3f(0, 1, 0);
            ofVec3f gVec = grVec.getRotated(tA, ofVec3f(tX, tY, tZ));
            ofVec3f redVec = ofVec3f(1, 0, 0);
            ofVec3f rVec = redVec.getRotated(tA, ofVec3f(tX, tY, tZ));
            ofVec3f blueVec = ofVec3f(0, 0, 1);
            ofVec3f bVec = blueVec.getRotated(tA, ofVec3f(tX, tY, tZ));
            
            q.makeRotate(gVec.getNormalized(), v.getNormalized());
            float tA2, tX2, tY2, tZ2;
            q.getRotate(tA2, tX2, tY2, tZ2);

            gVec = gVec.getRotated(tA2, ofVec3f(tX2, tY2, tZ2));
            rVec = rVec.getRotated(tA2, ofVec3f(tX2, tY2, tZ2));
            bVec = bVec.getRotated(tA2, ofVec3f(tX2, tY2, tZ2));

            q.makeRotate(v, ofVec3f(1,0,0));
            float tA3, tX3, tY3, tZ3;
            q.getRotate(tA3, tX3, tY3, tZ3);
            
            gVec = gVec.getRotated(tA3, ofVec3f(tX3, tY3, tZ3));
            rVec = rVec.getRotated(tA3, ofVec3f(tX3, tY3, tZ3));
            bVec = bVec.getRotated(tA3, ofVec3f(tX3, tY3, tZ3));

            ofPushMatrix();
            ofSetColor(255);
//            ofTranslate(rn[1].getGlobalPosition());
            ofSetColor(0, 255, 0);
            ofLine(ofVec3f(0,0,0), gVec * 300);
            ofSetColor(255, 0, 0);
            ofLine(ofVec3f(0,0,0), rVec * 300);
            ofSetColor(0, 0, 255);
            ofLine(ofVec3f(0,0,0), bVec * 300);
            ofPopMatrix();
        }
        ofPopMatrix();
    }
    ofPopMatrix();
}

void twistFinder::drawGraph(vector<ofVec3f> & vec, ofColor & drawColor, int elementNum){

    
    
    
    
//    ofSetColor(255);
//    ofLine(0, 0, 320, 0);
//    
//    for (int i = 0; i < vec.size(); i++ ) {
//        int val;
//        if (elementNum == 0) val = vec[i].x;
//        else if (elementNum == 1) val = vec[i].y;
//        else if (elementNum ==2) val = vec[i].z;
//        ofSetColor(drawColor);
//        ofLine(i, val, i, 0);
//    }
}





































//    ofPushMatrix();
//    {
//        ofTranslate(100,0);
//        ofVec3f v = rn[1].getGlobalPosition() - rn[0].getGlobalPosition();
//
//        ofSetColor(255);
//        ofLine (rn[0].getGlobalPosition(), rn[0].getGlobalPosition() + (v * 4));
//
//        ofQuaternion q = rn[1].getGlobalOrientation();
//        float tA, tX, tY, tZ;
//        q.getRotate(tA, tX, tY, tZ);
//
//        ofVec3f rdVec = ofVec3f(0,200,0);
//        ofVec3f rVec = rdVec.getRotated(tA, ofVec3f(tX, tY, tZ));
//
//        ofSetColor(0, 255, 0);
//        ofLine (rn[1].getGlobalPosition(), rn[1].getGlobalPosition() + (rVec * 30));
//
//        ofSetColor(255);
//
//        rn[0].draw();
//        rn[1].draw();
//
//    }
//    ofPopMatrix();
//
//    ofPushMatrix();
//    {
//        ofTranslate(200,0);
//        ofVec3f v = rn[1].getGlobalPosition() - rn[0].getGlobalPosition();
//
//        ofQuaternion q;
//        q.makeRotate(v, ofVec3f(1,0,0));
//        float tA3, tX3, tY3, tZ3;
//        q.getRotate(tA3, tX3, tY3, tZ3);
//        ofRotate(tA3, tX3, tY3, tZ3);
//
//        ofTranslate(-rn[0].getGlobalPosition());
//
//        ofPushMatrix();
//        {
//            ofSetColor(255);
//            ofLine (rn[0].getGlobalPosition(), rn[0].getGlobalPosition() + (v * 4));
//
//            ofQuaternion q2 = rn[1].getGlobalOrientation();
//            float tA, tX, tY, tZ;
//            q2.getRotate(tA, tX, tY, tZ);
//
//            ofVec3f rdVec = ofVec3f(0,200,0);
//            ofVec3f rVec = rdVec.getRotated(tA, ofVec3f(tX, tY, tZ));
//
//            ofSetColor(0, 255, 0);
//            ofLine (rn[1].getGlobalPosition(), rn[1].getGlobalPosition() + (rVec * 100));
//
//            ofSetColor(255);
//
//            rn[0].draw();
//
//            ofQuaternion q3;
//            q3.makeRotate(rVec.getNormalized(), v.getNormalized());
//            float tA2, tX2, tY2, tZ2;
//            q3.getRotate(tA2, tX2, tY2, tZ2);
//
//            ofTranslate(rn[1].getGlobalPosition());
//            ofRotate(tA2, tX2, tY2, tZ2);
//            ofTranslate(-rn[1].getGlobalPosition());
//            rn[1].draw();
//        }
//        ofPopMatrix();
//    }
//    ofPopMatrix();
//










//        ofPushMatrix();
//        ofSetColor(255);
//        ofTranslate(rn[1].getGlobalPosition());
//        ofSetColor(0, 255, 0);
//        ofLine(ofVec3f(0,0,0), gVec * 300);
//        ofSetColor(255, 0, 0);
//        ofLine(ofVec3f(0,0,0), rVec * 300);
//        ofSetColor(0, 0, 255);
//        ofLine(ofVec3f(0,0,0), bVec * 300);
//        ofPopMatrix();



//        ofPushMatrix();
//        ofSetColor(255);
//        ofTranslate(rn[1].getGlobalPosition());
//        ofSetColor(0, 255, 0);
//        ofLine(ofVec3f(0,0,0), gVec * 300);
//        ofSetColor(255, 0, 0);
//        ofLine(ofVec3f(0,0,0), rVec * 300);
//        ofSetColor(0, 0, 255);
//        ofLine(ofVec3f(0,0,0), bVec * 300);
//        ofPopMatrix();

//        ofQuaternion q;



//        ofPushMatrix();
//        ofSetColor(255);
//        ofTranslate(rn[1].getGlobalPosition());
//        ofSetColor(0, 255, 0);
//        ofLine(ofVec3f(0,0,0), gVec * 300);
//        ofSetColor(255, 0, 0);
//        ofLine(ofVec3f(0,0,0), rVec * 300);
//        ofSetColor(0, 0, 255);
//        ofLine(ofVec3f(0,0,0), bVec * 300);
//        ofPopMatrix();

//        ofQuaternion q3;



//        ofPushMatrix();
//        ofSetColor(255);
//        ofTranslate(rn[1].getGlobalPosition());
//        ofSetColor(0, 255, 0);
//        ofLine(ofVec3f(0,0,0), gVec * 300);
//        ofSetColor(255, 0, 0);
//        ofLine(ofVec3f(0,0,0), rVec * 300);
//        ofSetColor(0, 0, 255);
//        ofLine(ofVec3f(0,0,0), bVec * 300);
//        ofPopMatrix();

//        ofQuaternion q;

