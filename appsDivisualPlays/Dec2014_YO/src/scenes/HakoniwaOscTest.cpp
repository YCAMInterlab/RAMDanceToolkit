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
    mOscSender.setup("192.168.20.51", 8528); //test
}

HakoniwaOscTest::~HakoniwaOscTest()
{
    
}

void HakoniwaOscTest::update()
{
}

void HakoniwaOscTest::draw()
{
    mLed ? ofFill() : ofNoFill();
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
}

void HakoniwaOscTest::setupControlPanel()
{
    ramGetGUI().addToggle("LED", &mLed);
    ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &HakoniwaOscTest::onPanelChanged);
}


void HakoniwaOscTest::onPanelChanged(ofxUIEventArgs& e)
{
    const string name = e.widget->getName();
    if (name == "LED") {
        ofxOscMessage m;
        m.setAddress("/dp/hakoniwa/oscTest");
        m.addIntArg(mLed);
        mOscSender.sendMessage(m);
    }
}
