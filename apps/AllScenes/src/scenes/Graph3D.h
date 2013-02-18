#pragma once

#include "ramUpsideDown.h"

class Graph3D : public ramBaseScene
{
public:
	
	Graph3D() {}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		
		panel->addWidgetDown(new ofxUILabel(getName(), OFX_UI_FONT_LARGE));
		
		panel->addSpacer(gui.kLength, 2);
		panel->addWidgetDown(new ofxUILabel("No parameters", OFX_UI_FONT_MEDIUM));
	}
	
	void setup()
	{
		
	}
	
	void update()
	{
		
	}
	
	void drawHUD()
	{
		if (getNumNodeArray() == 0) return;
		
		ramNodeArray &arr = getNodeArray(0);
		
		{
			float ww = ofGetWidth() / 6;
			float hh = ofGetHeight() / 4;
			
			for (int y = 0; y < 4; y++)
			{
				for (int x = 0; x < 6; x++)
				{
					int index = y * 6 + x;
					if (index >= 23) continue;
					
					ramNode &node = arr.getNode(index);
					
					ofPushView();
					ofCamera cam;
					
					cam.setPosition(0, 0, 500);
					cam.begin(ofRectangle(x * ww, y * hh, ww, hh));
					
					ofMatrix4x4 m = node.getGlobalOrientation();
					m.postMultTranslate(ofVec3f(ww / 2, hh / 2, 0));
					
					glPushMatrix();
					
					glMultMatrixf(m.getPtr());
					
					ofDrawAxis(100);
					
					ofNoFill();
					ofSetColor(255, 30);
					ofBox(100);
					
					glPopMatrix();
					
					string str = ramActor::getJointName(index);
					ofDrawBitmapString(str, 0, -200);
					
					cam.end();
					
					ofPopView();
				}
			}
		}
	}
	
	void drawActor(ramActor& actor)
	{
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
		
	}
	
	const string getName() { return "Graph 3D"; }
};

