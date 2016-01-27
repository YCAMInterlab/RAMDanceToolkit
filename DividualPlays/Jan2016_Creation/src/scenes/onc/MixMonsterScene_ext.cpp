//
//  MixMonsterScene_ext.cpp
//  RAMDanceToolkit
//
//  Created by YoshitoONISHI on 1/27/16.
//
//

#include "MixMonsterScene_ext.h"

void MixMonsterScene_ext::setupControlPanel()
{
	needToReset = false;
	randomizeTopology = false;
	randomizeGeometry = false;
	randomLine = false;
	sidebyside = false;
	minScale = .5;
	maxScale = 2;
	randomizationAmount = .5;
	//    rotationVector = ofPoint(1,0,0);
	//    rotationAngle = 0;

	mEuler.x = 0;
	mEuler.y = 0;
	//    mEuler.z = 275;
	mEuler.z = 0;

	ofxUICanvas* panel = ramGetGUI().getCurrentUIContext();
	panel->addButton("Reset", &needToReset, 20, 20);
	panel->addButton("Random Line", &randomLine, 20, 20);
	panel->addButton("Randomize Topology", &randomizeTopology, 20, 20);
	panel->addButton("Randomize Geometry", &randomizeGeometry, 20, 20);
	panel->addButton("Side by Side", &sidebyside, 20, 20);
	panel->addSlider("Min scale", 0, 4, &minScale, 200, 20);
	panel->addSlider("Max scale", 0, 4, &maxScale, 200, 20);
	panel->addSlider("Randomization amount", 0, 1, &randomizationAmount, 200, 20);
	//    panel->addSlider("Rotate angle", -180, 180, &rotationAngle);
	//    panel->addSlider("Rotate Vector X", -1, 1, &rotationVector.x);
	//    panel->addSlider("Rotate Vector Y", -1, 1, &rotationVector.y);
	//    panel->addSlider("Rotate Vector Z", -1, 1, &rotationVector.z);
	cameraX = panel->addSlider("cameraX", -100, 200, 0.0f, 150, 8);
	cameraY = panel->addSlider("cameraY", -100, 300, 228.0f, 150, 8);
	cameraZ = panel->addSlider("cameraZ", -600, 600, 450.0f, 150, 8);
	interGap = panel->addSlider("gap", 0, 800, 450.0f, 150, 8);

	panel->addSlider("ANGLE X", 0.0f, 360.0f, &mEuler.x, 200.0f, dim);
	panel->addSlider("ANGLE Y", 0.0f, 360.0f, &mEuler.y, 200.0f, dim);
	panel->addSlider("ANGLE Z", 0.0f, 360.0f, &mEuler.z, 200.0f, dim);

	mex.setupControlPanel(this);
}

void MixMonsterScene_ext::setup()
{
	int treeArray[] = {             // add -1 before joint_hips. so 23. this is parent list.
		-1,
		ramActor::JOINT_HIPS,
		ramActor::JOINT_ABDOMEN,
		ramActor::JOINT_CHEST,
		ramActor::JOINT_NECK,
		ramActor::JOINT_HIPS,
		ramActor::JOINT_LEFT_HIP,
		ramActor::JOINT_LEFT_KNEE,
		ramActor::JOINT_LEFT_ANKLE,
		ramActor::JOINT_HIPS,
		ramActor::JOINT_RIGHT_HIP,
		ramActor::JOINT_RIGHT_KNEE,
		ramActor::JOINT_RIGHT_ANKLE,
		ramActor::JOINT_CHEST,
		ramActor::JOINT_LEFT_COLLAR,
		ramActor::JOINT_LEFT_SHOULDER,
		ramActor::JOINT_LEFT_ELBOW,
		ramActor::JOINT_LEFT_WRIST,
		ramActor::JOINT_CHEST,
		ramActor::JOINT_RIGHT_COLLAR,
		ramActor::JOINT_RIGHT_SHOULDER,
		ramActor::JOINT_RIGHT_ELBOW,
		ramActor::JOINT_RIGHT_WRIST,
		-1,                                                         // 2nd dancer hips as most ancestor joint.
		ramActor::NUM_JOINTS + ramActor::JOINT_HIPS,
		ramActor::NUM_JOINTS + ramActor::JOINT_ABDOMEN,
		ramActor::NUM_JOINTS + ramActor::JOINT_CHEST,
		ramActor::NUM_JOINTS + ramActor::JOINT_NECK,
		ramActor::NUM_JOINTS + ramActor::JOINT_HIPS,
		ramActor::NUM_JOINTS + ramActor::JOINT_LEFT_HIP,
		ramActor::NUM_JOINTS + ramActor::JOINT_LEFT_KNEE,
		ramActor::NUM_JOINTS + ramActor::JOINT_LEFT_ANKLE,
		ramActor::NUM_JOINTS + ramActor::JOINT_HIPS,
		ramActor::NUM_JOINTS + ramActor::JOINT_RIGHT_HIP,
		ramActor::NUM_JOINTS + ramActor::JOINT_RIGHT_KNEE,
		ramActor::NUM_JOINTS + ramActor::JOINT_RIGHT_ANKLE,
		ramActor::NUM_JOINTS + ramActor::JOINT_CHEST,
		ramActor::NUM_JOINTS + ramActor::JOINT_LEFT_COLLAR,
		ramActor::NUM_JOINTS + ramActor::JOINT_LEFT_SHOULDER,
		ramActor::NUM_JOINTS + ramActor::JOINT_LEFT_ELBOW,
		ramActor::NUM_JOINTS + ramActor::JOINT_LEFT_WRIST,
		ramActor::NUM_JOINTS + ramActor::JOINT_CHEST,
		ramActor::NUM_JOINTS + ramActor::JOINT_RIGHT_COLLAR,
		ramActor::NUM_JOINTS + ramActor::JOINT_RIGHT_SHOULDER,
		ramActor::NUM_JOINTS + ramActor::JOINT_RIGHT_ELBOW,
		ramActor::NUM_JOINTS + ramActor::JOINT_RIGHT_WRIST
	};
	treeBase.resize(2 * ramActor::NUM_JOINTS);
	lengthScale.resize(2 * ramActor::NUM_JOINTS, 1);
	bVisible.resize(2 * ramActor::NUM_JOINTS, true);
	//    bRotate.resize(2 * ramActor::NUM_JOINTS, false);
	for (int i = 0; i < treeBase.size(); i++) {
		treeBase[i] = treeArray[i];
		//          cout << "treeBase[" + ofToString(i) + "] " << treeBase[i] << endl;
		//          cout << "bVisible[" + ofToString(i) + "] " << bVisible[i] << endl;
	}
	treeSwap = treeBase;

}

void MixMonsterScene_ext::reset()
{
	treeSwap = treeBase;
	lengthScale.clear();
	lengthScale.resize(2 * ramActor::NUM_JOINTS, 1);
	bVisible.clear();
	bVisible.resize(2 * ramActor::NUM_JOINTS, true);
	//    bRotate.clear();
	//    bRotate.resize(2 * ramActor::NUM_JOINTS, false);

}

bool MixMonsterScene_ext::isAncestor(int ancestor, int child)
{
	int cur = child;
	while (treeSwap[cur] != -1) {
		if (cur == ancestor) {
			return true;
		}
		cur = treeSwap[cur];
	}
	return false;
}

vector<int> MixMonsterScene_ext::getNonChildren(int ancestor)
{
	vector<int> nonchildren;
	int n = ramActor::NUM_JOINTS;
	for (int child = 0; child < n; child++) {
		if (!isAncestor(ancestor, child)) {
			nonchildren.push_back(child);
		}
	}
	return nonchildren;
}

int MixMonsterScene_ext::getRandomNonChild(int i)
{
	vector<int> nonchildren = getNonChildren(i);
	int nonchild = nonchildren[ofRandom(0, nonchildren.size())];
	return nonchild;
}

void MixMonsterScene_ext::attach(int child, int parent)
{
	treeSwap[child] = parent;
}

void MixMonsterScene_ext::update()
{
	mex.update();

	ofMatrix4x4 mat;
	mat.rotate(mEuler.z, 0.0f, 0.0f, 1.0f);
	mat.rotate(mEuler.y, 0.0f, 1.0f, 0.0f);
	mat.rotate(mEuler.x, 1.0f, 0.0f, 0.0f);

	mRotation = mat.getRotate();

	if (needToReset) {
		reset();
	}
	if (randomizeTopology) {
		reset();
		int n = treeSwap.size();
		for (int i = 0; i < n; i++) {
			if (treeSwap[i] != -1 && ofRandom(1) < randomizationAmount) {
				treeSwap[i] = getRandomNonChild(i);
			}
		}
	}
	if (randomizeGeometry) {
		int n = lengthScale.size();
		for (int i = 0; i < n; i++) {
			if (ofRandom(1) < randomizationAmount) {
				lengthScale[i] = ofRandom(minScale, maxScale);
			}
			else {
				lengthScale[i] = 1;
			}
		}
	}
	if (randomLine) {
		reset();
		vector<int> all;
		for (int i = 1; i < 2 * ramActor::NUM_JOINTS; i++) {
			all.push_back(i);
		}
		ofRandomize(all);
		attach(all[0], ramActor::JOINT_HIPS);
		for (int i = 1; i < all.size(); i++) {
			attach(all[i], all[i - 1]);
		}
	}
	if (sidebyside) {

		reset();
		// Kumagai(0)
		treeSwap[ramActor::JOINT_RIGHT_COLLAR] = ramActor::JOINT_HIPS;
		treeSwap[ramActor::JOINT_LEFT_COLLAR] = ramActor::JOINT_CHEST; // just make sure. no need though.

		// Jareo(1)
		treeSwap[ramActor::NUM_JOINTS + ramActor::JOINT_RIGHT_COLLAR] = ramActor::JOINT_CHEST;
		treeSwap[ramActor::NUM_JOINTS + ramActor::JOINT_LEFT_COLLAR] = ramActor::JOINT_HIPS;

		bVisible[ramActor::JOINT_LEFT_HIP] = false;
		bVisible[ramActor::JOINT_LEFT_KNEE] = false;
		bVisible[ramActor::JOINT_LEFT_ANKLE] = false;
		bVisible[ramActor::JOINT_LEFT_TOE] = false;
		bVisible[ramActor::JOINT_RIGHT_HIP] = false;
		bVisible[ramActor::JOINT_RIGHT_KNEE] = false;
		bVisible[ramActor::JOINT_RIGHT_ANKLE] = false;
		bVisible[ramActor::JOINT_RIGHT_TOE] = false;
		bVisible[ramActor::NUM_JOINTS + ramActor::JOINT_HIPS] = false;
		bVisible[ramActor::NUM_JOINTS + ramActor::JOINT_ABDOMEN] = false;
		bVisible[ramActor::NUM_JOINTS + ramActor::JOINT_CHEST] = false;
		bVisible[ramActor::NUM_JOINTS + ramActor::JOINT_NECK] = false;
		bVisible[ramActor::NUM_JOINTS + ramActor::JOINT_HEAD] = false;
		bVisible[ramActor::NUM_JOINTS + ramActor::JOINT_LEFT_HIP] = false;
		bVisible[ramActor::NUM_JOINTS + ramActor::JOINT_LEFT_KNEE] = false;
		bVisible[ramActor::NUM_JOINTS + ramActor::JOINT_LEFT_ANKLE] = false;
		bVisible[ramActor::NUM_JOINTS + ramActor::JOINT_LEFT_TOE] = false;
		bVisible[ramActor::NUM_JOINTS + ramActor::JOINT_RIGHT_HIP] = false;
		bVisible[ramActor::NUM_JOINTS + ramActor::JOINT_RIGHT_KNEE] = false;
		bVisible[ramActor::NUM_JOINTS + ramActor::JOINT_RIGHT_ANKLE] = false;
		bVisible[ramActor::NUM_JOINTS + ramActor::JOINT_RIGHT_TOE] = false;

		//        bRotate[ramActor::JOINT_LEFT_COLLAR] = true;
		//        bRotate[ramActor::JOINT_LEFT_SHOULDER] = true;
		//        bRotate[ramActor::JOINT_LEFT_ELBOW] = true;
		//        bRotate[ramActor::JOINT_LEFT_WRIST] = true;
		//        bRotate[ramActor::JOINT_LEFT_HAND] = true;
		//        bRotate[ramActor::NUM_JOINTS + ramActor::JOINT_LEFT_COLLAR] = true;
		//        bRotate[ramActor::NUM_JOINTS + ramActor::JOINT_LEFT_SHOULDER] = true;
		//        bRotate[ramActor::NUM_JOINTS + ramActor::JOINT_LEFT_ELBOW] = true;
		//        bRotate[ramActor::NUM_JOINTS + ramActor::JOINT_LEFT_WRIST] = true;
		//        bRotate[ramActor::NUM_JOINTS + ramActor::JOINT_LEFT_HAND] = true;

	}

	getCameraManager().getActiveCamera().setGlobalPosition(cameraX->getValue(), cameraY->getValue(), cameraZ->getValue());

}

void MixMonsterScene_ext::draw()
{
	ramBeginCamera();

	vector<ramNodeArray> all = getActorManager().getAllNodeArrays();
	vector<ramNodeArray> actors;
	actors.clear();
	for (int i = 0; i < all.size(); i++) {
		bool bActEnable = false;
		for (int q = 0; q < mex.getNumPort(); q++)
			if (mex.getActorNameAt(q) == all.at(i).getName()) bActEnable = true;

		if (!bActEnable) continue;

		actors.push_back(all.at(i));
	}

	ramNode rn[2 * ramActor::NUM_JOINTS];

	for (int i = 0; i < actors.size(); i++) {
		ramNodeArray& na = actors.at(i);
		//        ofQuaternion base = na.getNode(ramActor::JOINT_HIPS).getOrientationQuat();
		//        ofQuaternion rotated = base * mRotation;
		//        na.getNode(ramActor::JOINT_HIPS).setOrientation(rotated);
		ofQuaternion rotated = mRotation;
		na.getNode(ramActor::JOINT_HIPS).setOrientation(rotated);

		for (int j = 0; j < ramActor::NUM_JOINTS; j++) {
			rn[(i* ramActor::NUM_JOINTS) +j] = na.getNode(j);
		}
	}

	rn[ramActor::JOINT_HIPS].setGlobalPosition(0, 0, 0);
	rn[ramActor::NUM_JOINTS + ramActor::JOINT_HIPS].setGlobalPosition(0, 0, 0);

	for (int i = 0; i < treeSwap.size(); i++) {
		if (treeSwap[i] != -1) {
			rn[i].setParent(rn[treeSwap[i]]);
			rn[i].setScale(lengthScale[i]);
		}
	}

	// draw shadow.
	ofPushMatrix();
	ofScale(1, 0.001, 1);
	for (int i = 0; i < 2 * ramActor::NUM_JOINTS; i++) {
		ramNode &node = rn[i];
		ofSetColor(0);
		ofSetLineWidth(3);
		if (node.hasParent() && bVisible[i]) {
			ofLine(node, *node.getParent());
		}
	}
	ofPopMatrix();
	ofPopStyle();


	// draw lines
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(0, 80, 0);

	ofNoFill();
	for (int i = 0; i < 2 * ramActor::NUM_JOINTS; i++) {
		ramNode &node = rn[i];
		ofSetColor(255);
		ofSetLineWidth(3);
		if (node.hasParent() && bVisible[i]) {
			ofLine(node, *node.getParent());
		}
	}
	ofPopMatrix();

	// draw both sides.
	for (int k = 0; k < actors.size(); k++) {

		ramNodeArray& na = actors.at(k);
		ofQuaternion rotated = mRotation;
		na.getNode(ramActor::JOINT_HIPS).setOrientation(rotated);
		ofPushMatrix();
		ofTranslate(-0.5 * interGap->getValue() + (interGap->getValue() * k), 0, 0);
		ofTranslate(-1 * na.getNode(ramActor::JOINT_HIPS) + ofPoint(0, 80, 0));
		ofPushStyle();

		ofNoFill();
		for (int i = 0; i < ramActor::NUM_JOINTS; i++) {
			ramNode &node = na.getNode(i);
			ofSetColor(255);
			ofSetLineWidth(2);
			if (node.hasParent()) {
				ofLine(node, *node.getParent());
			}
		}
		ofPopStyle();
		ofPopMatrix();
	}
	ramEndCamera();
}

void MixMonsterScene_ext::onPanelChanged(ofxUIEventArgs &e)
{
	const string name = e.widget->getName();

	// do something...
}


void MixMonsterScene_ext::drawActor(const ramActor& actor)
{

}

void MixMonsterScene_ext::drawRigid(const ramRigidBody &rigid)
{

}

void MixMonsterScene_ext::onActorSetup(const ramActor &actor)
{

}

void MixMonsterScene_ext::onActorExit(const ramActor &actor)
{

}

void MixMonsterScene_ext::onRigidSetup(const ramRigidBody &rigid)
{

}

void MixMonsterScene_ext::onRigidExit(const ramRigidBody &rigid)
{

}
