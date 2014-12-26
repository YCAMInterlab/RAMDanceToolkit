//
//  HakoniwaGearMove.cpp
//  RAMDanceToolkit
//
//  Created by ycam on 2014/12/25.
//
//

#include "HakoniwaGearMove.h"

HakoniwaGearMove::HakoniwaGearMove(){
    KsmrStep.setupOsc("192.168.20.51", 8528);
    KsmrStep.addStepper("step", 400, 0);
    KsmrStep.setStepperAll(true);
    KsmrStep.setupEasy();
}

void HakoniwaGearMove::update(){
}

void HakoniwaGearMove::draw(){
}

void HakoniwaGearMove::setupControlPanel(){
    ofxUICanvas* panel = ramGetGUI().getCurrentUIContext();
    ramGetGUI().addToggle("oscTurn", &ocsTurn);
    ramGetGUI().addToggle("kimeuchiTurn", &kimeuchiTurn);
    ramGetGUI().addSlider("kimeuchiVal", 10.0, 100.0, &kimeuchiVal);
    ramGetGUI().addToggle("just turn", &turn);
    ramGetGUI().addSlider("justTrurnSpeed", 0.0, 800.0, &justTrurnSpeed);
    ramGetGUI().addToggle("x", &x);
    ramGetGUI().addToggle("y", &y);
    ramGetGUI().addToggle("z", &z);
    for (int c=0; c<ramActor::NUM_JOINTS; c++)
    {
        mNodeVisibility[c] = (c == ramActor::JOINT_HIPS);
        mToggles[c] = panel->addToggle(ramActor::getJointName(c), &mNodeVisibility[c], 8, 8);
    }
}

void HakoniwaGearMove::drawActor(const ramActor &actor)
{
    mActor = actor;
    
    if((ocsTurn == true )&& (ofGetFrameNum() % 3 == 0)){
        ofNode nodePosition = actor.getNode(nodeNumber);
        ofNode nodePositionBefore = actor.getNode(nodeNumberBefore);
        ofVec3f euler = converter.convertAxis(nodePosition, nodePositionBefore);
        KsmrStep.go_to(euler[axisNum]);
        cout << euler[axisNum] << endl;
    }
    
    for(int i = 0; i < 23; i++){
        if(mNodeVisibility[i] == true){
            nodeNumber = i;
            if(i > 0){
                nodeNumberBefore = i-1;
            }else{
                nodeNumberBefore = 22;
            }
        }
    }
    
    if(x == true && y == false && z == false){
        axisNum = 0;
    }else if(x == false && y == true && z == false){
        axisNum = 1;
    }else if(x == false && y == false && z == true){
        axisNum = 2;
    }
    
    if(turn == true){
        KsmrStep.move(justTrurnSpeed, true);
    }
    
    if(kimeuchiTurn == true){
        count++;
        ofNode nodePosition = actor.getNode(nodeNumber);
        ofNode nodePositionBefore = actor.getNode(nodeNumberBefore);
        ofVec3f euler = converter.convertAxis(nodePosition, nodePositionBefore);
        
        if(count > 5){
            euler[axisNum] = beforeVal;
            count = 0;
        }
        
        if(euler[axisNum] - beforeVal > kimeuchiVal){
            KsmrStep.move(1600, true);
            cout << "turn!!" << endl;
        }else{
            cout << "No turn" << endl;
        }
    }
}

void HakoniwaGearMove::onPanelChanged(ofxUIEventArgs& e){   
}