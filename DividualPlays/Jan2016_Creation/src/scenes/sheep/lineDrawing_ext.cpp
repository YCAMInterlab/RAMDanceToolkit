//
//  lineDrawing_ext.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2016/01/24.
//
//

#include "lineDrawing_ext.h"

void LineDrawing_ext::setupControlPanel()
{
	ofxUICanvas* gui = ramGetGUI().getCurrentUIContext();
	for (int i = 0;i < NUM_LINE;i++)
	{
		gui->addSlider("Curve"+ofToString(i), -300.0, 300.0, &lines[i].curve);
		gui->addSlider("ext_to"+ofToString(i), 0, 1000, &lines[i].extend_to);
		gui->addSlider("ext_from"+ofToString(i), 0, 1000, &lines[i].extend_from);
		gui->addSlider("Red_"+ofToString(i), 0.0, 1.0, &lines[i].color.r);
		gui->addSlider("Green_"+ofToString(i), 0.0, 1.0, &lines[i].color.g);
		gui->addSlider("Blue_"+ofToString(i), 0.0, 1.0, &lines[i].color.b);
		
		gui->addSpacer();
	}
	
	mex.setupControlPanel(this);
	mex.load("motionExt_"+getName()+".xml");
	
	for (int i = 0;i < NUM_LINE;i++) lines[i].setup();
}

void LineDrawing_ext::update()
{

	
	for (int i = 0;i < NUM_LINE;i++)
	{
		if (mex.getJointIdAt(0 + i * 4) >= 0)
		{
			lines[i].nodeLine.from.name	= mex.getActorNameAt(0 + i * 4);
			lines[i].nodeLine.from.index = mex.getJointIdAt(0 + i * 4);
		}
		if (mex.getJointIdAt(1 + i * 4) >= 0)
		{
			lines[i].nodeLine.control0.name	= mex.getActorNameAt(1 + i * 4);
			lines[i].nodeLine.control0.index = mex.getJointIdAt(1 + i * 4);
		}
		if (mex.getJointIdAt(2 + i * 4) >= 0)
		{
			lines[i].nodeLine.control1.name	= mex.getActorNameAt(2 + i * 4);
			lines[i].nodeLine.control1.index = mex.getJointIdAt(2 + i * 4);
		}
		if (mex.getJointIdAt(3 + i * 4) >= 0)
		{
			lines[i].nodeLine.to.name	= mex.getActorNameAt(3 + i * 4);
			lines[i].nodeLine.to.index = mex.getJointIdAt(3 + i * 4);
		}
	}
	
	mex.update();
}

void LineDrawing_ext::draw()
{
	ramBeginCamera();

	ofPushStyle();
	for (int i = 0;i < NUM_LINE;i++)
	{
		lines[i].draw();
	}
	ofPopStyle();
	
	mex.draw();
	
	ramEndCamera();
}

void LineDrawing_ext::exit()
{
	
}

void LineDrawing_ext::onValueChanged(ofxUIEventArgs &e)
{
	
}