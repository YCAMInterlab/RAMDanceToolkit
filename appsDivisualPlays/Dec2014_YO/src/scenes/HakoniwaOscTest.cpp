//
//  HakoniwaOscTest.cpp
//  RAMDanceToolkit
//
//  Created by Onishi Yoshito on 7/22/14.
//
//

#include "HakoniwaOscTest.h"

HakoniwaOscTest::HakoniwaOscTest()
{
    mOscSender.setup("192.168.20.51", 8528);
    m6 = m7 = m8 = m9 = false;
    mLed = false;
}

HakoniwaOscTest::~HakoniwaOscTest()
{
    
}

void HakoniwaOscTest::update()
{
}

void HakoniwaOscTest::draw()
{
    ofDisableDepthTest();
    mLed ? ofFill() : ofNoFill();
    ofRect(10, 10, ofGetWidth()-20, ofGetHeight()-20);
}

void HakoniwaOscTest::setupControlPanel()
{
    ramGetGUI().addLabel("HakoniwaOscTest");
    ramGetGUI().addToggle("9:LED", &mLed);
    ramGetGUI().addSeparator();
    
    ramGetGUI().addLabel("HakoniwaColorOfWater");
    ramGetGUI().addToggle("6", &m6);
    ramGetGUI().addToggle("7", &m7);
    ramGetGUI().addToggle("8", &m8);
    ramGetGUI().addToggle("9", &m9);

    ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &HakoniwaOscTest::onPanelChanged);
}

void HakoniwaOscTest::onPanelChanged(ofxUIEventArgs& e)
{
    const string name = e.widget->getName();
    // HakoniwaOscTest
    if (name == "9:LED") {
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/oscTest");
        m.addIntArg(mLed);
        mOscSender.sendMessage(m);
    }
    // HakoniwaColorOfWater
    else if (name == "6") {
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/colorOfWater/6");
        m.addIntArg(m6);
        mOscSender.sendMessage(m);
    }
    else if (name == "7") {
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/colorOfWater/7");
        m.addIntArg(m7);
        mOscSender.sendMessage(m);
    }
    else if (name == "8") {
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/colorOfWater/8");
        m.addIntArg(m8);
        mOscSender.sendMessage(m);
    }
    else if (name == "9") {
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/colorOfWater/9");
        m.addIntArg(m9);
        mOscSender.sendMessage(m);
    }
}
