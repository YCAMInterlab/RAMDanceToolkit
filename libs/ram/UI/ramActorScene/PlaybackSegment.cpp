// 
// PlaybackSegment.cpp - RAMDanceToolkit
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

#include "PlaybackSegment.h"

#pragma mark -
#pragma mark constructor, destructor

PlaybackSegment::PlaybackSegment(string segmentName)
{
    name = segmentName;
    
	init();
	
	btnHideActor = new ofxUIImageToggle(32, 32, &bHideActor, ramToResourcePath("Images/show.png"),"show");
	btnPlayActor = new ofxUIImageToggle(32, 32, &bPaused, ramToResourcePath("Images/play.png"),"pause");
	btnCueActor = new ofxUIImageButton(32, 32, &bNeedsResetPos, ramToResourcePath("Images/reset.png"),"resetPos");
	btnDeleteActor = new ofxUIImageButton(32, 32, false, ramToResourcePath("Images/delete.png"),"delete");
}

PlaybackSegment::~PlaybackSegment()
{
	delete btnHideActor;
	delete btnPlayActor;
	delete btnCueActor;
	delete btnDeleteActor;
}




#pragma mark -
#pragma mark public methods
ramActorUISegmentType PlaybackSegment::getType()
{
    return RAM_UI_SEGMENT_TYPE_PLAYBACK;
}

void PlaybackSegment::update()
{
	session.updatePlayhead();
}

ofxUICanvasPlus* PlaybackSegment::createPanel(const string targetName)
{
    name = targetName;
    
	const float width = ramGetGUI().kLength;
	const float height = ramGetGUI().kDim+3;
    const float padding = ramGetGUI().kXInit*2;
	
	
	ofxUICanvasPlus *child = new ofxUICanvasPlus();
	child->disableAppDrawCallback();
	child->disableAppEventCallbacks();
	child->disableKeyEventCallbacks();
	child->disableMouseEventCallbacks();
	
	
	/// section title
	child->addWidgetDown(new ofxUILabel(name, OFX_UI_FONT_MEDIUM));
	child->addSpacer(width-padding, 2);
	
	
	/// Icons   
	child->addWidgetDown(btnHideActor);
	child->addWidgetRight(btnPlayActor);
	child->addWidgetRight(btnCueActor);
	child->addWidgetRight(btnDeleteActor);
	
	
	/// actor color
	child->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	child->addSlider("R", 0, 1, &jointColor.r, 44, height);
	child->addSlider("G", 0, 1, &jointColor.g, 44, height);
	child->addSlider("B", 0, 1, &jointColor.b, 45, height);
	child->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
	
    
	/// actor position
	child->add2DPad("Position",
					ofPoint(-500, 500),
					ofPoint(-500, 500),
					&position,
					width-padding, 100);
	
	child->autoSizeToFitWidgets();
	
	ofAddListener(child->newGUIEvent, this, &PlaybackSegment::onValueChanged);
	
	return child;
}


void PlaybackSegment::pause(bool bPause)
{
    bPaused = bPause;
    
	if (!bPaused)
		session.play();
	else
		session.stop();
    
    btnPlayActor->setValue(bPaused);
}

void PlaybackSegment::loadCache()
{
	if ( !ofFile::doesFileExist(getCacheFilePath()) ) return;
	
	XML.clear();
	XML.loadFile(getCacheFilePath());
	
	/// color
	XML.pushTag("color");
	jointColor.r = XML.getValue("r", 0.8);
	jointColor.g = XML.getValue("g", 0.8);
	jointColor.b = XML.getValue("b", 0.8);
	XML.popTag();
	
	/// position
	XML.pushTag("position");
	position.x = XML.getValue("x", 0.0);
	position.y = XML.getValue("y", 0.0);
	XML.popTag();
	
	/// boolean state
	XML.pushTag("state");
	bHideActor = XML.getValue("hideActor", 0);
	btnHideActor->setValue(bHideActor);
	btnHideActor->stateChange();
	XML.popTag();
}

void PlaybackSegment::saveCache()
{
	XML.clear();
	
	/// color
	XML.addTag("color");
	XML.pushTag("color");
	XML.addValue("r", jointColor.r);
	XML.addValue("g", jointColor.g);
	XML.addValue("b", jointColor.b);
	XML.popTag();
	
	/// position
	XML.addTag("position");
	XML.pushTag("position");
	XML.addValue("x", position.x);
	XML.addValue("y", position.y);
	XML.popTag();
	
	/// boolean states
	XML.addTag("state");
	XML.pushTag("state");
	XML.addValue("hideActor", bHideActor);
	XML.popTag();
	
	XML.saveFile(getCacheFilePath());
}

void PlaybackSegment::deleteSelf()
{
    ofSendMessage("/PlaybackSegment/remove "+getName());
}





#pragma mark -
#pragma mark Events

void PlaybackSegment::onValueChanged(ofxUIEventArgs& e)
{
	const string widgetName = e.widget->getName();
	
	if (widgetName == "pause")
	{
		ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
		const bool value = toggle->getValue();
		pause(value);
	}
    
	if (widgetName == "delete")
	{
		ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
		const bool pressed = toggle->getValue();
		if (pressed)
			deleteSelf();
	}
	
	saveCache();
}




#pragma mark -
#pragma mark private methods

void PlaybackSegment::init()
{
	bPaused = false;
    BaseSegment::init();
}


