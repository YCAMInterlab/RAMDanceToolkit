//
//  HakoniwaParallelLink_Base.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2014/12/20.
//
//

#include "HakoniwaParallelLink_Base.h"

HakoniwaParallelLink_Base::HakoniwaParallelLink_Base(){

//	mLinkManager.setup("cu.usbmodem1141");
//	mLinkManager.setupOsc("192.168.20.37", 12345);
    
//    mLinkManager.setup("cu.usbmodem1142");
//    mLinkManager.setupOsc("192.168.20.38", 12345);
//    mLinkManager.setupOsc("192.168.20.38", 8528);
//
    
	mLinkManager.height = 250.0;

	mLinkManager.area_clamp.set(80.0, 80.0, 80.0);
	mLinkManager.area_offset.set(0.0, 150.0, 0.0);
    
    //konno----------------
//    KsmrStep.setup("cu.usbmodem1141", 57600);
//    KsmrStep.setupOsc("192.168.20.37", 12345);
    KsmrStep.setupOsc("192.168.20.57", 8528);
    KsmrStep.addStepper("step", 400, 0);
       // KsmrStep.addStepper("step", 400, 0);
    KsmrStep.setStepperAll(true);
    KsmrStep.setupEasy();
    //konno----------------
    
}

void HakoniwaParallelLink_Base::update(){

	ofVec3f v = mActor.getNode(ramActor::JOINT_LEFT_ELBOW);


	mLinkManager.update();

	if (CalibratePose)		mLinkManager.setPlot_inClamp(ofVec3f(0.0,100.0,0.0));
	else if (ManualPose)	mLinkManager.setPlot_inClamp(mManualPosition);
	else					mLinkManager.setPlot_inClamp(v);

}

void HakoniwaParallelLink_Base::draw(){
	ofVec3f v = mActor.getNode(ramActor::JOINT_LEFT_ELBOW);

	ramBeginCamera();
	glEnable(GL_DEPTH_TEST);
	mLinkManager.draw();
	glDisable(GL_DEPTH_TEST);
	ramEndCamera();

}

void HakoniwaParallelLink_Base::setupControlPanel(){

    //konno--------------------
    ofxUICanvas* panel = ramGetGUI().getCurrentUIContext();
    //konno--------------------
    
	ramGetGUI().addLabel("System");
	ramGetGUI().getCurrentUIContext()->addSpacer();
	ramGetGUI().addSlider("arm1", 0.0, 200.0, &mLinkManager.armLength1);
	ramGetGUI().addSlider("arm2", 0.0, 200.0, &mLinkManager.armLength2);
	ramGetGUI().addSlider("radius", 0.0, 100.0, &mLinkManager.radius);
	ramGetGUI().addSlider("height", 0.0, 300.0, &mLinkManager.height);
	ramGetGUI().addSlider("plotRadius", 0.0, 100.0, &mLinkManager.plot_radius);

	ramGetGUI().addLabel("Calibration");
	ramGetGUI().getCurrentUIContext()->addSpacer();
	ramGetGUI().addSlider("Clamp_X", 0.0, 200.0, &mLinkManager.area_clamp.x);
	ramGetGUI().addSlider("Clamp_Y", 0.0, 200.0, &mLinkManager.area_clamp.y);
	ramGetGUI().addSlider("Clamp_Z", 0.0, 200.0, &mLinkManager.area_clamp.z);
	ramGetGUI().addSlider("OffsetX", -200.0, 200.0, &mLinkManager.area_offset.x);
	ramGetGUI().addSlider("OffsetY", -200.0, 200.0, &mLinkManager.area_offset.y);
	ramGetGUI().addSlider("OffsetZ", -200.0, 200.0, &mLinkManager.area_offset.z);

	ramGetGUI().addSlider("ManualX", -200.0, 200.0, &mManualPosition.x);
	ramGetGUI().addSlider("ManualY", -200.0, 200.0, &mManualPosition.y);
	ramGetGUI().addSlider("ManualZ", -200.0, 200.0, &mManualPosition.z);

	CalibratePose	= false;
	ManualPose		= false;

	ramGetGUI().addToggle("ManualPosition", &ManualPose);
	ramGetGUI().addToggle("CalibratePosition", &CalibratePose);
	ramGetGUI().addButton("Calibration");
	ramGetGUI().addToggle("Enable", &mLinkManager.enableSync);
    
    //konno------------------
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
    //konno------------------

	ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent,
				  this, &HakoniwaParallelLink_Base::onPanelChanged);

}

void HakoniwaParallelLink_Base::drawActor(const ramActor &actor)
{
	mActor = actor;
    
    //konno-----------
    if((ocsTurn == true )&& (ofGetFrameNum() % 3 == 0)){
        
//        KsmrStep.move(800,true);
        
        //normal--------------------------------------
//        ofVec3f pos = actor.getNode(nodeNumber).getGlobalOrientation().getEuler();
//        KsmrStep.go_to(pos[axisNum]);
//        cout << pos[0] << endl;
        //normal--------------------------------------
        
        //converter ver--------------------------------------
        ofNode nodePosition = actor.getNode(nodeNumber);
        ofNode nodePositionBefore = actor.getNode(nodeNumberBefore);
        ofVec3f euler = converter.convertAxis(nodePosition, nodePositionBefore);
        KsmrStep.go_to(euler[axisNum]);
        cout << euler[axisNum] << endl;
        //converter ver--------------------------------------
        
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
        
//        KsmrStep.go_to(euler[axisNum]);
    }
    
    //konno-----------
}

void HakoniwaParallelLink_Base::onPanelChanged(ofxUIEventArgs& e){
	ofxUIWidget* w = e.widget;

	if (w->getName() == "Calibration"){
		mLinkManager.calibrate();
	}

	//cout << "Ev" << endl;

}