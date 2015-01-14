//
//  testScenes.h
//  2015_MAIN_APP
//
//  Created by Ovis aries on 2015/01/14.
//
//

#ifndef _015_MAIN_APP_testScenes_h
#define _015_MAIN_APP_testScenes_h

struct : public ramBaseScene {
	void draw()
	{
		ofPushStyle();
		ofEnableAlphaBlending();
		ofSetColor(ofColor::cyan, 128);
		ofDrawBox(ofVec3f::zero(), 100.f);
		ofPopStyle();
	}

	void drawActor(const ramActor &actor)
	{
		ofPushStyle();
		ofEnableAlphaBlending();
		ofSetColor(ofColor::cyan, 128);
		ramDrawBasicActor(actor);
		ofPopStyle();
	}
	string getName() const { return "TestSceneA"; }
} testSceneA;

struct : public ramBaseScene {
	void draw()
	{
		ofPushStyle();
		ofEnableAlphaBlending();
		ofSetColor(ofColor::magenta, 128);
		ofDrawBox(ofVec3f::zero(), 100.f);
		ofPopStyle();
	}

	void drawActor(const ramActor &actor)
	{
		ofPushStyle();
		ofEnableAlphaBlending();
		ofSetColor(ofColor::magenta, 128);
		ramDrawBasicActor(actor);
		ofPopStyle();
	}
	string getName() const { return "TestSceneB"; }
} testSceneB;

struct : public ramBaseScene {
	void draw()
	{
		ofPushStyle();
		ofEnableAlphaBlending();
		ofSetColor(ofColor::yellow, 128);
		ofDrawBox(ofVec3f::zero(), 100.f);
		ofPopStyle();
	}

	void drawActor(const ramActor &actor)
	{
		ofPushStyle();
		ofEnableAlphaBlending();
		ofSetColor(ofColor::yellow, 128);
		ramDrawBasicActor(actor);
		ofPopStyle();
	}
	string getName() const { return "TestSceneC"; }
} testSceneC;

struct : public ramBaseScene {

	void draw()
	{
		ofPushStyle();
		ofEnableAlphaBlending();
		ofSetColor(ofColor::white, 128);
		ofDrawBox(ofVec3f::zero(), 100.f);
		ofPopStyle();
	}

	void drawActor(const ramActor &actor)
	{
		ofPushStyle();
		ofEnableAlphaBlending();
		ofSetColor(ofColor::white, 128);
		ramDrawBasicActor(actor);
		ofPopStyle();
	}
	string getName() const { return "TestSceneD"; }
} testSceneD;

#endif
