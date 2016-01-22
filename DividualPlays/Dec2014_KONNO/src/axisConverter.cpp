//
//  axisConverter.cpp
//  AxisChanger
//
//  Created by itotaka on 10/11/14.
//
//

#include "axisConverter.h"

ofVec3f axisConverter::convertAxis(ofNode& node, ofNode& nodeOrigin) {

    this->node = node;
    
    q.makeRotate(node.getGlobalPosition().normalize(), ofVec3f(0,-1,0));
    ofVec3f v = node.getGlobalPosition() - nodeOrigin.getGlobalPosition();

    qSC.makeRotate(v.normalize(), node.getZAxis());
    euler = qSC.getEuler();
    
    return euler;
    
}

void axisConverter::draw(){
    
    ofPushMatrix();
    
    float tA, tX, tY, tZ;
    q.getRotate(tA, tX, tY, tZ);
    ofRotate(tA, tX, tY, tZ);
    
    ofLine(ofVec3f(0,0,0), node.getGlobalPosition());
    node.draw();
    
    ofPopMatrix();
    
}
