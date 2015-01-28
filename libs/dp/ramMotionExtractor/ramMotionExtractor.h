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
	void setupControlPanel(ramBaseScene* scene_,
						   ofVec2f canvasPos = ofVec2f(240,30));
	void update();
	void draw();

	/*=== Getter ===*/
	int				getNumPort();
	bool			getIsExist(int port);

	ramNode			getNodeAt(int port);
	string			getActorNameAt(int port);
	string			getJointNameAt(int port);
	int				getJointIdAt(int port);

	ofVec3f			getPositionAt(int port,bool fixPosition = false);
	ofQuaternion	getRotationAt(int port);

	ofVec3f			getVelocityAt(int port);
	float			getVelocitySpeedAt(int port);
	ofQuaternion	getRotateVelocityAt(int port);
	float			getDistanceAt(int port_A, int port_B);
	float			getAreaAt(int port_A, int port_B, int port_C);

	/*=== Utilities ===*/
	void clearPorts();
	void save(string file);
	void load(string file);

	void pushFromID(int actorId, int jointId);
	void pushPort(ramMotionPort *mp, int actorId = -1);
	void popPort(ramNodeFinder &nf);
	void setActorList(vector<string>* lst);
	void refleshActorFromList();
	void mouseReleased(ofMouseEventArgs &arg);

	vector<ramMotionPort*>	mMotionPort;
	ofxUISortableList*		actorList;

protected:
	void guiEvent(ofxUIEventArgs &e);
	int getIndexFromName(string name);

	ofxUICanvasPlus*		parentGui;
	ofxUICanvas*			mGui;
	ramBaseScene*			mScenePtr;
	ofVec2f					mCurrentCanvasPos;
	float					mMotionSmooth;
	int						lastNumNodeArray;

	ramOscReceiveTag	receiver;
	bool				bEnableSync;
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
