// 
// Stamp.h - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

class Stamp : public ramBaseScene
{
	
	ramStamp mStamp;
	bool mShowActor;
	bool mShowBox;
	
	float line_width;
	
	ofFloatColor color;
	ofFloatColor timer_display_color;
	
	float timer_duration;
	
	bool use_idle_timer;
	
public:
	
	string getName() const { return "Stamp"; }
	
	Stamp() : mShowActor(true), mShowBox(true) {}
	
	void setupControlPanel()
	{
		ramGetGUI().addToggle("Idle Timer", &use_idle_timer);
		mStamp.setupControlPanel();
		
        ramGetGUI().addSeparator();
        
		ramGetGUI().addToggle("Show Actor", &mShowActor);
		ramGetGUI().addColorSelector("Box line color", &color);
		ramGetGUI().addSlider("Line width", 0, 6, &line_width);
		
		use_idle_timer = false;
		timer_duration = 0.5;
    }
	
	void setup()
	{
		mStamp.clear();
	}
	
	void update()
	{
		const int num = getNumNodeArray();
        
        if(num > 0 && !use_idle_timer)
        {
            mStamp.update( getNodeArray(ofRandom(0, num)) );
        }
	}
	
	void draw()
	{
		ramBeginCamera();
		
		for (int i=0; i<mStamp.getSize(); i++)
		{
			ramNodeArray& nodeArray = mStamp.getStamp(i);
			
			if (mShowActor)
			{
				ramDrawNodes(nodeArray);
			}
			
			if (color.a > 0)
			{
				ofPushStyle();
				ofSetColor(color);
				ofSetLineWidth(line_width);
				ramDrawActorCube(nodeArray);
				ofPopStyle();
			}
		}
		
		ramEndCamera();
	}
	
};

