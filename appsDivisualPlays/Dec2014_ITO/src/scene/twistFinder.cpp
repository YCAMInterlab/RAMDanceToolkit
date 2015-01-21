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

void twistFinder::debugDraw4(ramMotionExtractor & motionExtractor, int nodeNum) {
    
    ramNode rn;
    rn = motionExtractor.getNodeAt(nodeNum);
    
    ofNode parent, childR, childG, childB;
    ofQuaternion q = rn.getOrientationQuat();

    childR.setPosition(100, 0, 0);
    childR.setParent(parent);
    childG.setPosition(0, 100, 0);
    childG.setParent(parent);
    childB.setPosition(0,0,100);
    childB.setParent(parent);
    parent.setOrientation(q);
    
//    ofQuaternion r;
//    r.makeRotate(childG.getGlobalPosition().getNormalized(), ofVec3f(0,1,0));
//    parent.rotate(r);
    
    ofPushMatrix();
    ofTranslate(300, 300);
    ofNoFill();
    ofSetColor(255,0,0);
    childR.draw();
    ofSetColor(0,255,0);
    childG.draw();
    ofSetColor(0,0,255);
    childB.draw();
    ofSetColor(255);
    parent.draw();
    ofPopMatrix();
    
    float distY = childB.getGlobalPosition().y - childG.getGlobalPosition().y;
    ofVec2f p = ofVec2f(childR.getGlobalPosition().x, childR.getGlobalPosition().z).getNormalized();

    if (nodeNum == 0) {
        his0.push_back(distY);
        if (his0.size() > 400) his0.erase(his0.begin());
        ofTranslate(500, 300);
        drawGraph(his0, ofColor::red);
    }
    
    if (nodeNum == 1) {
        his1.push_back(distY);
        if (his1.size() > 400) his1.erase(his1.begin());
        ofTranslate(500, 300);
        drawGraph(his1, ofColor::red);
    }
    
    ofPushMatrix();

    
    ofPopMatrix();
    
//    cout << "node" << nodeNum << ": " <<  distY << endl;
    
    ofPushMatrix();
    ofTranslate(300, 600);
    ofSetColor(255,0,0);
    ofLine(0,0, p.x*100,-p.y*100);
    ofSetColor(150);
    ofLine(0,0, 100,0);
    ofPopMatrix();

    p = p.getNormalized();
    
    float angleA, angleB;
    angleA = acos(p.dot(ofVec2f(1,0)));
    if (p.y < 0) angleA *= -angleA;
    
    angleA = angleA * 180 / PI;
    
    ofDrawBitmapString(ofToString(angleA), 200, 600);
    
    
    
//
//    angleB = acos(rVec[1].dot(ofVec3f(0,0,1)));
//    if (rVec[1].y < 0) angleB = (2*PI) - angleB;
//    
//    hisSub.push_back((angleB - angleA)*10 + (2 * PI * 10));
//    if (hisSub.size() > 400) hisSub.erase(hisSub.begin());
//    
//    his1.push_back(angleA * 20);
//    if (his1.size() > 400) his1.erase(his1.begin());
//    
//    his0.push_back(angleB * 20);
//    if (his0.size() > 400) his0.erase(his0.begin());

    

}

void twistFinder::debugDraw3(ramMotionExtractor & motionExtractor) {

    ramNode rn[2];
    rn[0] = motionExtractor.getNodeAt(0);
//    rn[1] = motionExtractor.getNodeAt(1);

    ofNode parent, parent2, childR, childG, childB;
//    ofQuaternion q = rn[0].getGlobalOrientation().inverse() * rn[1].getGlobalOrientation();
//    ofQuaternion q = rn[0].getGlobalOrientation().inverse() * rn[1].getGlobalOrientation();
    ofQuaternion q = rn[0].getOrientationQuat();
//    ofQuaternion p = rn[0].getOrientationQuat();

    childR.setPosition(100, 0, 0);
    childR.setParent(parent);
    childG.setPosition(0, 100, 0);
    childG.setParent(parent);
    childB.setPosition(0,0,100);
    childB.setParent(parent);
    parent.setOrientation(q);
//    parent2.setOrientation(p);
    
//    cout << childG.getPosition() << endl;
//    
    ofQuaternion r;
    r.makeRotate(childG.getGlobalPosition().getNormalized(), ofVec3f(0,1,0));
    parent.rotate(r);
    
    ofPushMatrix();
    ofTranslate(300, 300);
    ofNoFill();
    ofSetColor(255,0,0);
    childR.draw();
    ofSetColor(0,255,0);
    childG.draw();
    ofSetColor(0,0,255);
    childB.draw();
    ofSetColor(255);
    parent.draw();
//    ofTranslate(0, 100);
//    parent2.draw();
    ofPopMatrix();
    
}


void twistFinder::debugDraw2(ramMotionExtractor & motionExtractor){

    //experimental
    ramNode rn[2];
    rn[0] = motionExtractor.getNodeAt(0);
    rn[1] = motionExtractor.getNodeAt(1);

    ofMatrix4x4 a[2];
    a[0] = rn[0].getGlobalTransformMatrix();
    a[1] = rn[1].getGlobalTransformMatrix();
    ofNode n[2];
    
    n[0].setTransformMatrix(a[0]);
    n[1].setTransformMatrix(a[1]);
    
    ofVec3f v = n[0].getYAxis();
    ofVec3f b = a[0]*v;
    
    ofLine(n[0].getGlobalPosition(), n[0].getGlobalPosition() + (b * 100));
    
    ofPushMatrix();
    ofTranslate(100, 0);
    n[0].draw();
    n[1].draw();
    ofPopMatrix();
}

void twistFinder::debugDraw(ramMotionExtractor & motionExtractor) {

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
            ofVec3f rVec[2];
            calculateVector(rn[0], rn[1], rVec[0]);
            calculateVector(rn[1], rn[0], rVec[1]);

            float angleA, angleB;
            angleA = acos(rVec[0].dot(ofVec3f(0,0,1)));
            if (rVec[0].y < 0) angleA = (2*PI) - angleA;

            angleB = acos(rVec[1].dot(ofVec3f(0,0,1)));
            if (rVec[1].y < 0) angleB = (2*PI) - angleB;

            hisSub.push_back((angleB - angleA)*10 + (2 * PI * 10));
            if (hisSub.size() > 400) hisSub.erase(hisSub.begin());
            
            his1.push_back(angleA * 20);
            if (his1.size() > 400) his1.erase(his1.begin());
            
            his0.push_back(angleB * 20);
            if (his0.size() > 400) his0.erase(his0.begin());

            ofPushMatrix();
            ofTranslate(500,300);
            drawGraph(his0, ofColor::green);
            ofDrawBitmapString("0:green", -30,0);
//            ofTranslate(0,200);
            drawGraph(his1, ofColor::red);
            ofDrawBitmapString("1:red", -30,0);
            ofTranslate(0,200);
            drawGraph(hisSub, ofColor::white);
            ofDrawBitmapString("1red-0red", -30,0);
            ofPopMatrix();
            
            ofPushMatrix();
            ofTranslate(1200, 500);
//            ofSetColor(0, 255, 0);
//            ofLine(ofVec3f(0,0,0), gVec[0] * 300);
            ofDrawBitmapString("green:node0(red), red:node1(red)", -30,0);
            ofSetColor(0, 255, 0);
            ofLine(0,0, rVec[0].z * 200, rVec[0].y * 200);
//            ofLine(ofVec3f(0,0,0), rVec[0] * 300);
            ofSetColor(255, 0, 0);
            ofLine(0,0, rVec[1].z * 200, rVec[1].y * 200);
//            ofLine(ofVec3f(0,0,0), bVec[0] * 300);
            ofPopMatrix();

//            ofPushMatrix();
//            ofTranslate(300,0);
//            ofTranslate(1000, 500);
//            ofSetColor(0, 255, 0);
////            ofLine(ofVec3f(0,0,0), gVec[1] * 300);
////            ofSetColor(255, 0, 0);
//            ofLine(ofVec3f(0,0,0), rVec[1] * 300);
//            ofSetColor(0, 0, 255);
//            ofLine(ofVec3f(0,0,0), bVec[1] * 300);
//            ofPopMatrix();
        }
        ofPopMatrix();
        
        // rn[1]
//        ofPushMatrix();
//        {
//            ofTranslate(800, 500);
//            ofVec3f gVec, rVec, bVec;
//            calculateVector(1, rn[0], rn[1], rVec, gVec, bVec);
//
//            float angle;
//            angle = acos(rVec.dot(ofVec3f(0,0,1)));
//            if (rVec.y < 0) angle *= -1;
//            
//            his1.push_back(angle * 40);
//            if (his1.size() > 400) his1.erase(his1.begin());
//            
//            ofPushMatrix();
//            ofTranslate(-500,200);
//            ofPopMatrix();
        
//            ofPushMatrix();
//            ofSetColor(0, 255, 0);
//            ofLine(ofVec3f(0,0,0), gVec * 300);
//            ofSetColor(255, 0, 0);
//            ofLine(ofVec3f(0,0,0), rVec * 300);
//            ofSetColor(0, 0, 255);
//            ofLine(ofVec3f(0,0,0), bVec * 300);
//            ofPopMatrix();
//        }
//        ofPopMatrix();

        
    }
    ofPopMatrix();
}

void twistFinder::calculateVector(const ramNode & node0, const ramNode & node1, ofVec3f & vec){

    ofNode parent,childR, childG;

    ofVec3f v = node1.getGlobalPosition() - node0.getGlobalPosition();
    ofQuaternion q = node0.getGlobalOrientation();

    childR.setPosition(1, 0, 0);
    childR.setParent(parent);
    childG.setPosition(0, 1, 0);
    childG.setParent(parent);

    parent.setOrientation(q);

    q.makeRotate(childG.getGlobalPosition().getNormalized(), v.getNormalized());
    parent.rotate(q);
    
    q.makeRotate(v, ofVec3f(-1,0,0));
    parent.rotate(q);

    vec = childR.getGlobalPosition();
    
}

void twistFinder::drawGraph(vector<float> & history, ofColor drawColor){

    ofSetColor(255);
    ofNoFill();
    ofRect(0, 0, 400, 400);
    
    ofDrawBitmapString(ofToString(history[history.size()-1]), 400,400);

    if (history.size() > 0) {
        for (int i = 1; i < history.size(); i++ ) {
            ofSetColor(drawColor);
            ofLine(i, 200-history[i], i-1, 200-history[i-1]);
        }
    }

    int mYL = ofClamp(ofGetMouseY(), 300, 700);
    
    ofSetColor(255);
    ofLine(0, mYL-300, 400, mYL-300);
    ofDrawBitmapString(ofToString(200-(mYL-300)), 400, mYL-300);

    
    
    
//    for (int i = 0; i < vec.size(); i++ ) {
//        int val;
//        if (elementNum == 0) val = vec[i].x * 100;
//        else if (elementNum == 1) val = vec[i].y * 100;
//        else if (elementNum ==2) val = vec[i].z * 100;
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




/*
 void twistFinder::calculateVector(const ramNode & node0, const ramNode & node1, ofVec3f & redVec, ofVec3f & greenVec, ofVec3f & blueVec){
 
 ofVec3f v = node1.getGlobalPosition() - node0.getGlobalPosition();
 
 ofVec3f redAxis = ofVec3f(1, 0, 0);
 ofVec3f greenAxis = ofVec3f(0, 1, 0);
 ofVec3f blueAxis = ofVec3f(0, 0, 1);
 
 float tA, tX, tY, tZ;
 
 ofQuaternion q = node0.getGlobalOrientation();
 q.getRotate(tA, tX, tY, tZ);
 
 ofNode parent,childR, childG;
 childR.setPosition(1, 0, 0);
 childR.setParent(parent);
 childG.setPosition(0, 1, 0);
 childG.setParent(parent);
 parent.setOrientation(q);
 
 //    redAxis = redAxis.getRotated(tA, ofVec3f(tX, tY, tZ));
 greenAxis = childG.getGlobalPosition();
 //    greenAxis = greenAxis.getRotated(tA, ofVec3f(tX, tY, tZ));
 //    blueAxis = blueAxis.getRotated(tA, ofVec3f(tX, tY, tZ));
 
 q.makeRotate(greenAxis.getNormalized(), v.getNormalized());
 q.getRotate(tA, tX, tY, tZ);
 parent.setOrientation(q);
 
 //    redAxis = redAxis.getRotated(tA, ofVec3f(tX, tY, tZ));
 redAxis = child.getGlobalPosition();
 greenAxis = greenAxis.getRotated(tA, ofVec3f(tX, tY, tZ));
 blueAxis = blueAxis.getRotated(tA, ofVec3f(tX, tY, tZ));
 
 q.makeRotate(v, ofVec3f(-1,0,0));
 q.getRotate(tA, tX, tY, tZ);
 parent.setOrientation(q);
 
 //    redAxis = redAxis.getRotated(tA, ofVec3f(tX, tY, tZ));
 redAxis = child.getGlobalPosition();
 greenAxis = greenAxis.getRotated(tA, ofVec3f(tX, tY, tZ));
 blueAxis = blueAxis.getRotated(tA, ofVec3f(tX, tY, tZ));
 
 redVec = redAxis;
 greenVec = greenAxis;
 blueVec = blueAxis;
 
 }

 
 */
