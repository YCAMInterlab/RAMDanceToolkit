//
//  BrokenBody_ext.cpp
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2016/01/25.
//
//

#include "BrokenBody_ext.h"

void BrokenBody_ext::setupControlPanel()
{
	ofxUICanvasPlus* gui = ramGetGUI().getCurrentUIContext();

	gui->addSlider("thresh", 0.1, 30.0, &thresh);
	
	gui->addWidgetDown(new ofxUILabel("BodyColor", OFX_UI_FONT_MEDIUM));
	gui->addSlider("R", 0, 1, &bodyColor.r, 70, 16);
	gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	gui->addSlider("G", 0, 1, &bodyColor.g, 70, 16);
	gui->addSlider("B", 0, 1, &bodyColor.b, 70, 16);
	gui->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
	gui->autoSizeToFitWidgets();
	
	mex.setupControlPanel(this);
}

void BrokenBody_ext::update()
{
	ramActorManager &am = ramGlobalShortcut::getActorManager();
	
	int numNodes = 0;
	for (int i = 0;i < am.getNumNodeArray();i++)
		numNodes += am.getNodeArray(i).getNumNode();
	
	if (mBoxes.size() != numNodes)
	{
		mBoxes.clear();
		for (int i = 0;i < numNodes;i++)
			mBoxes.push_back(BrokenBox_ext());
	}
	
	mex.update();
}

void BrokenBody_ext::draw()
{
	
	ramBeginCamera();

	ramActorManager &am = ramGlobalShortcut::getActorManager();
	int cnt = 0;
	for (int i = 0;i < am.getNumNodeArray();i++)
	{
		ramActor act = am.getNodeArray(i);
		bool bActEnable = false;
		for (int q = 0;q < mex.getNumPort();q++)
			if (mex.getActorNameAt(q) == act.getName()) bActEnable = true;
		
		if (!bActEnable) continue;
		
		for (int j = 0;j < act.getNumNode();j++)
		{
			ramNode & tmpNode = act.getNode(j);
			
			float jointSize = (j == ramActor::JOINT_HEAD) ? 8.0 : 5.0;
			float length = tmpNode.getVelocity().length();
			int idx = cnt;
			
			mBoxes[idx].bFixed = (length < thresh);
			mBoxes[idx].update(tmpNode.getGlobalPosition());
			
			tmpNode.setGlobalPosition(mBoxes[idx].mPos);
			
			ofSetColor(bodyColor);
			ramBox(tmpNode, jointSize);
			ramLine(tmpNode);
			
			cnt++;
		}
	}
	
	mex.draw();
	ramEndCamera();
}