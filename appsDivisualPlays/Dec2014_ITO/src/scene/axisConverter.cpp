//
//  axisConverter.cpp
//  example-ramMotionExtractor
//
//  Created by itotaka on 1/16/15.
//
//

#include "axisConverter.h"

ofVec3f axisConverter::convertAxis(ofNode& node, ofNode& nodeOrigin) {
    
    this->node = node;
    
//    q.makeRotate(node.getPosition().normalize(), ofVec3f(0,-1,0));
    ofVec3f v = node.getPosition() - nodeOrigin.getPosition();
    
    qSC.makeRotate(node.getZAxis(), v.normalize());
    euler = qSC.getEuler();
    
    return euler;
    
}

void axisConverter::draw(){
    
    ofPushMatrix();
    
    float tA, tX, tY, tZ;
    q.getRotate(tA, tX, tY, tZ);
    ofRotate(tA, tX, tY, tZ);
    
    ofLine(ofVec3f(0,0,0), node.getPosition());
    node.draw();
    
    ofPopMatrix();
    
}






