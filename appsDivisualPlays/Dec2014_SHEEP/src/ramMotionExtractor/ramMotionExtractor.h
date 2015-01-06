//
//  ramMotionExtractor.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2014/12/26.
//
//

#ifndef __RAMDanceToolkit__ramMotionExtractor__
#define __RAMDanceToolkit__ramMotionExtractor__

#include "ramMain.h"

class ramMotionPort;

class ramMotionExtractor{
public:

	void setupControlPanel(ramBaseScene* scene_, ofVec2f canvasPos = ofVec2f(240,30));

	void update();
	void draw();
	void guiEvent(ofxUIEventArgs &e);
	void setDefaultNode(ramNode &node);

	int getNumPort(){return mMotionPort.size();};

	ramNode			getNodeAt(int port);
	string			getActorNameAt(int port);
	string			getJointNameAt(int port);
	int				getJointIdAt(int port);

	ofVec3f			getPositionAt(int port);
	ofQuaternion	getRotationAt(int port);

	ofVec3f			getVelocityAt(int port);
	ofQuaternion	getRotateVelocityAt(int port);
	float			getDistanceAt(int port_A, int port_B);

	float			getVelocitySpeedAt(int port);

	ofxUICanvas*			mGui;
	ramBaseScene*			mScenePtr;
	vector<ramMotionPort*>	mMotionPort;
};


class ramMotionPort{
public:

	ramMotionPort(const ramNodeIdentifer &nodeIdent){
		init(nodeIdent);
	}
	~ramMotionPort(){

	}

	void init(ramNodeFinder nodeF){
		mFinder = nodeF;
		mVelocitySmoothed.set(0,0,0);
		mVelocity.set(0,0,0);
		isBlank = false;
		vecInitialize = false;
	}
	void update();
	bool			vecInitialize;
	bool			isBlank;
	ramNodeFinder	mFinder;
	ofNode			mCurrentNode;
	ofNode			mBefNode;

	float			speedSmoothed;

	ofVec3f			mVelocity;
	ofVec3f			mVelocitySmoothed;
	ofQuaternion	mRotateVec;
	ofQuaternion	mRotateVecSmoothed;
};

#endif /* defined(__RAMDanceToolkit__ramMotionExtractor__) */
