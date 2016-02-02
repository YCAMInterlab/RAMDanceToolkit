//
//  BigBox_ext.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2016/01/25.
//
//

#include "BigBox_ext.h"

void BigBox_ext::setupControlPanel()
{
	
	ofxUICanvas* gui = ramGetGUI().getCurrentUIContext();
	gui->addSlider("masterScale", 0.0, 1000.0, &boxScale);
	gui->addSlider("lineWidth", 0.0, 5.0, &lineWidth);
	gui->addSlider("color_r", 0.0, 1.0, &color.r);
	gui->addSlider("color_g", 0.0, 1.0, &color.g);
	gui->addSlider("color_b", 0.0, 1.0, &color.b);
	
	for (int i = 0;i < BOXSC_NUM;i++)
	{
		gui->addSlider("boxScale"+ofToString(i), 0.0, 1000.0, &boxScales[i]);
		boxScales[i] = 100.0;
	}
	
	
	mex.setupControlPanel(this);
	
	for (int i = 0;i < NUMNODE_BBE;i++) portScale[i] = 0.0;
	
	color.set(1.0, 1.0, 1.0);
	boxScale = 100.0;
	lineWidth = 3.0;
}

void BigBox_ext::update()
{
	mex.update();
}

void BigBox_ext::draw()
{
	ramBeginCamera();
	mex.draw();
	
	ofNoFill();
	
	for (int i = 0;i < mex.getNumPort();i++)
		portNodes[i] = mex.getNodeAt(i);
	
	ofPushStyle();
	ofSetLineWidth(lineWidth);
	ofSetColor(color);
	for (int i = 0;i < NUMNODE_BBE;i++)
	{
		if (i < mex.getNumPort()) portScale[i] += (boxScales[(i < BOXSC_NUM ? i : 0)] - portScale[i]) / 5.0;
		else portScale[i] += (0 - portScale[i]) / 5.0;
			
		ofPushMatrix();
		ofMultMatrix(portNodes[i].getGlobalTransformMatrix());
		ofDrawBox(0, 0, 0, portScale[i]);
		ofPopMatrix();

	}
	ofPopStyle();
	
	ofFill();
	ramEndCamera();
}