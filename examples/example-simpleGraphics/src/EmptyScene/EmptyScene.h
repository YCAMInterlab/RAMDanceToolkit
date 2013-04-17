// 
// EmptyScene.h - RAMDanceToolkit
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

class EmptyScene : public ramBaseScene
{

	string key_toggle;
	bool bColor;

public:

	EmptyScene()
	{
		setSceneName("Empty Scene");

		// ---

		key_toggle = "toggle test";
		bColor = false;
	}


	void setup()
	{

	}

	void refreshControlPanel(ramControlPanel& gui)
	{
		gui.addPanel( getSceneName() );
		gui.addToggle( key_toggle );

		guiPtr = &gui;
	}

	void update()
	{
		if( guiPtr->hasValueChanged( key_toggle ) )
		{
			bColor = guiPtr->getValueB( key_toggle );
		}
	}

	void draw()
	{
		ofBackground( bColor ? ramColor::WHITE : ramColor::BLACK );

		if (!bEnabled) return;

		ramBeginCamera();
		ofSetColor( ramColor::GRAY );
		ofDrawBitmapString( "Empty!", ofVec3f(0,0,0) );
		ramEndCamera();
	}

	void drawActor( ramActor& actor )
	{

	}

	void drawActor(const ramActor &actor)
	{

	}

	void drawFloor()
	{

	}
};

