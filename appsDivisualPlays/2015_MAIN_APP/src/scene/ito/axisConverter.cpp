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










/*
 
 ramNode rn[10];
 for (int i = 0; i < 10; i++){
 rn[i] = mMotionExtractor.getNodeAt(i);
 }
 
 ofPushMatrix();
 ofNoFill();
 ofTranslate(200,0);
 
 for (int i = 0; i < 10; i++) {
 if (rn[i].getPosition().length() !=0) rn[i].draw();
 }
 
 ofSetColor(255);
 
 ofVec3f axisEdge[3][10];
 
 for (int i = 0; i < 10; i++) {
 //        ofTranslate(rn[i].getPosition());
 if (rn[i].getPosition().length() != 0) {
 ofQuaternion q = rn[i].getGlobalOrientation();
 float tA, tX, tY, tZ;
 q.getRotate(tA, tX, tY, tZ);
 
 ofTranslate(0,-100,0);
 
 ofPushMatrix();
 ofRotate(tA, tX, tY, tZ);
 //        ofLine(0, 0, 0, tX * 100.0f, tY * 100.0f, tZ * 100.0f);
 //            ofLine(0,0,0,100,0,0);
 
 ofSetColor(255,0,0);
 ofLine (-20,0,0,20,0,0);
 ofSetColor(0,255,0);
 ofLine (0,-20,0,0,20,0);
 ofSetColor(0,0,255);
 ofLine (0,0,-20,0,0,20);   // 先端の座標、赤(20,0,0)青(0,20,0)緑(0,0,20)に対して、xyZでグラフ表示してみる <-解析のため。
 
 ofPopMatrix();
 
 ofVec3f redPos = ofVec3f(20,0,0);
 axisEdge[0][i] = redPos.getRotated(tA, ofVec3f(tX, tY, tZ));
 ofVec3f greenPos = ofVec3f(0,20,0);
 axisEdge[1][i] = greenPos.getRotated(tA, ofVec3f(tX, tY, tZ));
 ofVec3f bluePos = ofVec3f(0,0,20);
 axisEdge[2][i] = bluePos.getRotated(tA, ofVec3f(tX, tY, tZ));
 
 ofSetColor(255,0,0);
 ofDrawBox(axisEdge[0][i], 10, 10, 10);
 ofSetColor(0,255,0);
 ofDrawBox(axisEdge[1][i], 10, 10, 10);
 ofSetColor(0,0,255);
 ofDrawBox(axisEdge[2][i], 10, 10, 10);
 
 ofPopMatrix();
 }
 }
 
 vecRed.push_back(axisEdge[0][0]);
 vecGreen.push_back(axisEdge[1][0]);
 vecBlue.push_back(axisEdge[2][0]);
 if (vecRed.size() > 320) vecRed.erase(vecRed.begin());
 if (vecGreen.size() > 320) vecGreen.erase(vecGreen.begin());
 if (vecBlue.size() > 320) vecBlue.erase(vecBlue.begin());
 
 ofPopMatrix();
 
 ramEndCamera();
 
 ofPushMatrix();
 
 ofTranslate(500, 200);
 ofColor r = ofColor(255,0,0);
 drawGraph(vecRed, r, 0);
 
 ofTranslate(0, 100);
 ofColor g = ofColor(0,255,0);
 drawGraph(vecRed, g, 0);
 
 ofTranslate(0, 100);
 ofColor b = ofColor(0,0,255);
 drawGraph(vecRed, b, 0);
 
 ofPopMatrix();

 
 
 
 
 
 void dpHakoniwaMagnetPendulum::drawGraph(vector<ofVec3f> & vec, ofColor & drawColor, int elementNum){
 
 ofSetColor(255);
 ofLine(0, 0, 320, 0);
 
 for (int i = 0; i < vecRed.size(); i++ ) {
 int val;
 if (elementNum == 0) val = vec[i].x;
 else if (elementNum == 1) val = vec[i].y;
 else if (elementNum ==2) val = vec[i].z;
 ofSetColor(drawColor);
 ofLine(i, val, i, 0);
 }
 }

 */


