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
}

HakoniwaOscTest::~HakoniwaOscTest()
{
    
}

void HakoniwaOscTest::update()
{
}

void HakoniwaOscTest::draw()
{
    bool blink = (bool)(int)fmod(ofGetElapsedTimef(), 2.f);
    
    ofxOscMessage m;
    m.setAddress("/dp/hakoniwa/oscTest");
    m.addIntArg(blink);
    m.addIntArg(blink);
    mOscSender.sendMessage(m);
    
    blink ? ofFill() : ofNoFill();
    ofRect(0, 0, ofGetWidth(), ofGetHeight());
}
