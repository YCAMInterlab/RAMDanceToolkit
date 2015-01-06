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

	/*=== Must call for use ===*/
	void setupControlPanel(ramBaseScene* scene_, ofVec2f canvasPos = ofVec2f(240,30));
	void update();
	void draw();

	/*=== Getter ===*/
	int				getNumPort();

	ramNode			getNodeAt(int port);
	string			getActorNameAt(int port);
	string			getJointNameAt(int port);
	int				getJointIdAt(int port);

	ofVec3f			getPositionAt(int port);
	ofQuaternion	getRotationAt(int port);

	ofVec3f			getVelocityAt(int port);
	float			getVelocitySpeedAt(int port);
	ofQuaternion	getRotateVelocityAt(int port);
	float			getDistanceAt(int port_A, int port_B);

	/*=== Utilities ===*/
	void clearPorts();
	void save(string file);
	void load(string file);

protected:
	void guiEvent(ofxUIEventArgs &e);

	ofxUICanvas*			mGui;
	ramBaseScene*			mScenePtr;
	float					mMotionSmooth;
	vector<ramMotionPort*>	mMotionPort;

};


class ramMotionPort{
public:

	ramMotionPort(const ramNodeIdentifer &nodeIdent){
		init(nodeIdent);
	}
	~ramMotionPort(){

	}

	void			init(ramNodeFinder nodeF);
	void			update(float smooth);

	bool			vecInitialize;
	bool			isBlank;

	ramNodeFinder	mFinder;
	ofNode			mCurrentNode;
	ofNode			mBefNode;

	ofVec3f			mVelocity;
	ofVec3f			mVelocitySmoothed;
	ofQuaternion	mRotateVec;
	ofQuaternion	mRotateVecSmoothed;

};

#endif /* defined(__RAMDanceToolkit__ramMotionExtractor__) */
