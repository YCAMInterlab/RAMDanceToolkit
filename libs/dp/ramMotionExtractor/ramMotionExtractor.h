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
	void mouseReleased(ofMouseEventArgs &arg);

	/*=== Getter ===*/
	int				getNumPort();
	bool			getIsExist(int port);

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
	int getIndexFromName(string name);

	ofxUICanvasPlus*		parentGui;
	ofxUICanvas*			mGui;
	ofxUISortableList*		actorList;
	ramBaseScene*			mScenePtr;
	ofVec2f					mCurrentCanvasPos;
	float					mMotionSmooth;
	int						lastNumNodeArray;
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
	void			refleshActorFromIndex();
	void			refleshActorFromName();

	bool			vecInitialize;
	bool			isBlank;

	int				mActorIndex;
	ramNodeFinder	mFinder;
	ofNode			mCurrentNode;
	ofNode			mBefNode;

	ofVec3f			mVelocity;
	ofVec3f			mVelocitySmoothed;
	ofQuaternion	mRotateVec;
	ofQuaternion	mRotateVecSmoothed;

};

#endif /* defined(__RAMDanceToolkit__ramMotionExtractor__) */
