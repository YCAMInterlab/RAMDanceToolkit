//
//  twistFinder.cpp
//  example-ramMotionExtractor
//
//  Created by itotaka on 1/17/15.
//
//

#include "twistFinder.h"

double twistFinder::findTwist(ramNode & node, float bendingLimit, float divisionForLimit){
    
    ofNode parent, childR, childG, childB;
    ofQuaternion q = node.getOrientationQuat();
    
    childR.setPosition(100, 0, 0);
    childR.setParent(parent);
    childG.setPosition(0, 100, 0);
    childG.setParent(parent);
    childB.setPosition(0,0,100);
    childB.setParent(parent);
    parent.setOrientation(q);
    
    float distY = childB.getGlobalPosition().y - childG.getGlobalPosition().y;
    ofVec2f p = ofVec2f(childR.getGlobalPosition().x, childR.getGlobalPosition().z).getNormalized();

    double angle;
    angle = acos(p.dot(ofVec2f(1,0)));
    if (p.y < 0) angle *= -1;
    
    if (distY > (bendingLimit + divisionForLimit)){
        int valOver = distY - bendingLimit;
        angle /= ((double)valOver/divisionForLimit);
    }
    
    angle = angle * 180 / PI;
    return angle;

}

void twistFinder::debugDraw(ramMotionExtractor & motionExtractor, int nodeNum) {
    
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
    
    double angleA, angleB;
    angleA = acos(p.dot(ofVec2f(1,0)));
    if (p.y < 0) angleA *= -1;
    
    if (distY > 40){
        int valOver = distY - 30;
        angleA /= ((double)valOver/10.0f);
    }
    
    ofPushMatrix();
    ofTranslate(300, 500);
//    ofSetColor(255,0,0);
//    ofLine(0,0, p.x*100,-p.y*100);
    ofSetColor(255, 0, 0);
    ofLine(0,0, cos(angleA)*100, -sin(angleA)*100);
    ofSetColor(150);
    ofLine(0,0, 100,0);
    ofPopMatrix();
    
    angleA = angleA * 180 / PI;
    ofDrawBitmapString(ofToString(angleA), 200, 500);
    
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
}

