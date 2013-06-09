//
//  testApp.cpp
//  RAMDanceToolkit
//
//  Created by motoishmz on 4/27/13.
//  Copyright (c) 2013 YCAMInterlab. All rights reserved.

#include "testApp.h"


static const string kSettingFilePath = "Settings.xml";
static const int kMaxPlaybackSize = 4;


//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	
	
	const float width_panel = 300;
	const float width_ui = width_panel - OFX_UI_GLOBAL_WIDGET_SPACING*2;
	const float width_input = 106;
	
	
	playbackers.clear();
	sender.setup("127.0.0.1", 10000);
	
	paused = false;
	reset_playhead = false;
	speed = 1.0;
	
	
	GUI = new ofxUICanvas(0, 0, width_panel, ofGetHeight());
	input_ip = new ofxUITextInput("osc_ip", "127.0.0.1", width_input);
	input_port = new ofxUITextInput("osc_port", "10000", width_input);
	
	
	// title
	GUI->addWidgetDown(new ofxUILabel("RAM OSC Server", OFX_UI_FONT_LARGE));
	GUI->addSpacer();
	
	
	// osc section
	GUI->addWidgetDown(new ofxUILabel("Send OSC message to...", OFX_UI_FONT_MEDIUM));
	GUI->addWidgetDown(new ofxUILabel("ip:", OFX_UI_FONT_MEDIUM));
	GUI->addWidgetRight(input_ip);
	GUI->addWidgetDown(new ofxUILabel("port:", OFX_UI_FONT_MEDIUM));
	GUI->addWidgetRight(input_port);
	GUI->addWidgetDown(new ofxUILabel("Press [setup osc sender] button", OFX_UI_FONT_SMALL));
	GUI->addWidgetDown(new ofxUILabel("after editing ip and port.", OFX_UI_FONT_SMALL));
	GUI->addWidgetDown(new ofxUILabelButton("Setup OSC Sender", false, width_ui));
	GUI->addSpacer();
	
	
	// loadfile section
	GUI->addWidgetDown(new ofxUILabel("Press [Load xml file] button or", OFX_UI_FONT_SMALL));
	GUI->addWidgetDown(new ofxUILabel("drag and drop xml files onto this screen", OFX_UI_FONT_SMALL));
	GUI->addWidgetDown(new ofxUILabel("to load playback data. Note that the max ", OFX_UI_FONT_SMALL));
	GUI->addWidgetDown(new ofxUILabel("number of playback files is "+ofToString(kMaxPlaybackSize)+".", OFX_UI_FONT_SMALL));
	GUI->addWidgetDown(new ofxUILabelButton("Load xml file", false, width_ui));
	GUI->addWidgetDown(new ofxUILabelButton("Clear", false, width_ui));
	GUI->addSpacer();
	
	
	// player control section
	GUI->addWidgetDown(new ofxUIToggle("Pause", &paused, 30, 30, 0, 0, OFX_UI_FONT_MEDIUM));
	GUI->addWidgetDown(new ofxUIButton("Reset playhead", &reset_playhead, 30, 30, 0, 0, OFX_UI_FONT_MEDIUM));
	GUI->addWidgetDown(new ofxUISlider("Playback speed", -3.0, 3.0, &speed, width_ui, 30));
	
	GUI->addWidgetDown(new ofxUIButton("Set playback speed: -3", false, 10, 10, 0, 0, OFX_UI_FONT_SMALL));
	GUI->addWidgetDown(new ofxUIButton("Set playback speed: -2", false, 10, 10, 0, 0, OFX_UI_FONT_SMALL));
	GUI->addWidgetDown(new ofxUIButton("Set playback speed: -1", false, 10, 10, 0, 0, OFX_UI_FONT_SMALL));
	GUI->addWidgetDown(new ofxUIButton("Set playback speed: 0",  false, 10, 10, 0, 0, OFX_UI_FONT_SMALL));
	GUI->addWidgetDown(new ofxUIButton("Set playback speed: 1",  false, 10, 10, 0, 0, OFX_UI_FONT_SMALL));
	GUI->addWidgetDown(new ofxUIButton("Set playback speed: 2",  false, 10, 10, 0, 0, OFX_UI_FONT_SMALL));
	GUI->addWidgetDown(new ofxUIButton("Set playback speed: 3",  false, 10, 10, 0, 0, OFX_UI_FONT_SMALL));
	GUI->addSpacer();
	
	
	GUI->loadSettings(kSettingFilePath);
	ofAddListener(GUI->newGUIEvent, this, &testApp::onValueChanged);
}

//--------------------------------------------------------------
void testApp::update()
{
	for(int i=0; i<playbackers.size(); i++)
		playbackers[i].update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	for(int i=0; i<playbackers.size(); i++)
	{
		ramOscPlaybacker &playbacker = playbackers.at(i);
		
		sender.sendMessage(playbacker.getCurrentMessage());
		playbacker.debugDraw(ofPoint(320, i*100+20));
	}
}

//--------------------------------------------------------------
void testApp::exit()
{
	GUI->saveSettings(kSettingFilePath);
}

//--------------------------------------------------------------
void testApp::load(const string path)
{
	if (playbackers.size() > kMaxPlaybackSize)
		return;
	
	ramOscPlaybacker playbacker(path);
	
	if (playbacker.canPlayback())
	{
		if (paused)
			playbacker.pause();
		else
			playbacker.play();
		
		playbackers.push_back(playbacker);
	}
	else
	{
		cout << "couldn't playback: " << path << endl;
	}
}

//--------------------------------------------------------------
void testApp::setSpeed(const float new_speed)
{
	speed = new_speed;
	
	for(int i=0; i<playbackers.size(); i++)
		playbackers[i].setSpeed(speed);
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{
	for(int i=0; i<dragInfo.files.size(); i++)
	{
		const string filePath = dragInfo.files.at(i);
		load(filePath);
	}
}

//--------------------------------------------------------------
void testApp::onValueChanged(ofxUIEventArgs &e)
{
	const string name = e.widget->getName();
	
	if (name == "Setup OSC Sender")
	{
		const string ip = input_ip->getTextString();
		const int port = ofToInt(input_port->getTextString());
		
		sender.setup(ip, port);
	}
	
	if (name == "Load xml file")
	{
		if (static_cast<ofxUIButton *>(e.widget)->getValue() == false)
			return;
		
		ofFileDialogResult result = ofSystemLoadDialog("Select motiona data xml file.", false);
		
		if (result.bSuccess)
			load(result.getPath());
	}
	
	if (name == "Clear")
	{
		playbackers.clear();
	}
	
	if (name == "Pause")
	{
		for(int i=0; i<playbackers.size(); i++)
		{
			if (static_cast<ofxUIButton *>(e.widget)->getValue() == true)
				playbackers[i].pause();
			else
				playbackers[i].resume();
		}
	}
	
	if (name == "Reset playhead")
	{
		if (static_cast<ofxUIButton *>(e.widget)->getValue() == false)
			return;
		
		for(int i=0; i<playbackers.size(); i++)
			playbackers[i].setPlayhead(0.0);
	}
	
	if (name == "Playback speed")
	{
		for(int i=0; i<playbackers.size(); i++)
			playbackers[i].setSpeed(speed);
	}
	
	if (name == "Set playback speed: -3") { setSpeed(-3.0); }
	if (name == "Set playback speed: -2") { setSpeed(-2.0); }
	if (name == "Set playback speed: -1") { setSpeed(-1.0); }
	if (name == "Set playback speed: 0" ) { setSpeed(0.0); }
	if (name == "Set playback speed: 1" ) { setSpeed(1.0); }
	if (name == "Set playback speed: 2" ) { setSpeed(2.0); }
	if (name == "Set playback speed: 3" ) { setSpeed(3.0); }
}