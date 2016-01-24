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