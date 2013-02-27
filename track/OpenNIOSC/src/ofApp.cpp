#include "ofApp.h"

ofVec3f upDirection(0, -1, 0), downDirection = -upDirection;
ofVec3f leftDirection(-1, 0, 0), rightDirection = -leftDirection;
ofVec3f forwardDirection(0, 0, -1), backDirection = -forwardDirection;

// openni is in millimeters, ram is in centimeters
const float ramScale = .1;

const static int RAM_JOINT_COUNT = 23;

int jointMapping[] = {
	JOINT_TORSO, //JOINT_HIPS
	JOINT_TORSO, //JOINT_ABDOMEN,
	JOINT_NECK, //JOINT_CHEST,
	JOINT_NECK, //JOINT_NECK,
	JOINT_HEAD, //JOINT_HEAD,
	
	JOINT_LEFT_HIP, //JOINT_LEFT_HIP,
	JOINT_LEFT_KNEE, //JOINT_LEFT_KNEE,
	JOINT_LEFT_FOOT, //JOINT_LEFT_ANKLE,
	JOINT_LEFT_FOOT, //JOINT_LEFT_TOE,
	
	JOINT_RIGHT_HIP, //JOINT_RIGHT_HIP,
	JOINT_RIGHT_KNEE, //JOINT_RIGHT_KNEE,
	JOINT_RIGHT_FOOT, //JOINT_RIGHT_ANKLE,
	JOINT_RIGHT_FOOT, //JOINT_RIGHT_TOE,
	
	JOINT_LEFT_SHOULDER, //JOINT_LEFT_COLLAR,
	JOINT_LEFT_SHOULDER, //JOINT_LEFT_SHOULDER,
	JOINT_LEFT_ELBOW, //JOINT_LEFT_ELBOW,
	JOINT_LEFT_HAND, //JOINT_LEFT_WRIST,
	JOINT_LEFT_HAND, //JOINT_LEFT_HAND,
	
	JOINT_RIGHT_SHOULDER, //JOINT_RIGHT_COLLAR,
	JOINT_RIGHT_SHOULDER, //JOINT_RIGHT_SHOULDER,
	JOINT_RIGHT_ELBOW, //JOINT_RIGHT_ELBOW,
	JOINT_RIGHT_HAND, //JOINT_RIGHT_WRIST,
	JOINT_RIGHT_HAND, //JOINT_RIGHT_HAND,
};

string ramJointName[] =
{
	"HIPS",
	"ABDOMEN",
	"CHEST",
	"NECK",
	"HEAD",
	"LEFT_HIP",
	"LEFT_KNEE",
	"LEFT_ANKLE",
	"LEFT_TOE",
	"RIGHT_HIP",
	"RIGHT_KNEE",
	"RIGHT_ANKLE",
	"RIGHT_TOE",
	"LEFT_COLLAR",
	"LEFT_SHOULDER",
	"LEFT_ELBOW",
	"LEFT_WRIST",
	"LEFT_HAND",
	"RIGHT_COLLAR",
	"RIGHT_SHOULDER",
	"RIGHT_ELBOW",
	"RIGHT_WRIST",
	"RIGHT_HAND"
};

// if axis1 and axis2 are not orthogonal, only axis1 will be preserved
ofQuaternion makeRotate(const ofVec3f& axis1, const ofVec3f& axis2) {
	ofVec3f avg = (axis1 + axis2) / 2;
	ofVec3f z = axis1.getCrossed(axis2);
	ofVec3f x = axis1;
	ofVec3f y = z.getCrossed(x);
	ofMatrix4x4 mat;
	vector<ofVec3f> axes;
	axes.push_back(x.normalize());
	axes.push_back(y.normalize());
	axes.push_back(z.normalize());
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			mat(i, j) = axes[i][j];
		}
	}
	return mat.getRotate();
}

ofQuaternion inheritRotation(const ofQuaternion& parentOrientation, const ofVec3f& from, const ofVec3f& to) {
	ofVec3f bindFrom = parentOrientation * from;
	ofQuaternion orientation;
	orientation.makeRotate(bindFrom, to);
	orientation += parentOrientation;
	return orientation;
}

void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(120);
	
	openniCenter.set(0, 0, 2000);
	
	ofxXmlSettings settings;
	settings.loadFile("settings.xml");
	
	ofSetLogLevel(OF_LOG_VERBOSE);
	settings.pushTag("openNI");
	openNI.setDeviceID(settings.getValue("deviceID", 0));
	openNI.setup();
	openNI.addDepthGenerator();
	openNI.setRegister(false);
	openNI.setMirror(false);
	openNI.addUserGenerator();
	openNI.setMaxNumUsers(settings.getValue("maxNumUsers", 2));
	openNI.start();
	settings.popTag();
	
	settings.pushTag("osc");
	osc.setup(settings.getValue("hostname", "localhost"), settings.getValue("port", 10000));
	settings.popTag();
}

void ofApp::update(){
	openNI.update();
	if(openNI.isNewFrame()) {
		ofxOscBundle bundle;
		int deviceId = openNI.getDeviceID();
		int numTrackedUsers = openNI.getNumTrackedUsers();
		
		for(int userIndex = 0; userIndex < numTrackedUsers; userIndex++) {
			ofxOpenNIUser& user = openNI.getTrackedUser(userIndex);
			if(user.isTracking() && user.isSkeleton()) {
				ofxOscMessage msg;
				msg.setAddress("/ram/skeleton");
				int userId = user.getXnID();
				string actorName = "OpenNI " + ofToString(userId) + " @" + ofToString(deviceId);
				int numJoints = user.getNumJoints();
				msg.addStringArg(actorName);
				msg.addIntArg(RAM_JOINT_COUNT);
				
				// should use accelerometer to right things
				// or a custom slider to position people
				float floorOffset = 0;
				vector<ofVec3f> positions(JOINT_COUNT);
				for(int openniIndex = 0; openniIndex < JOINT_COUNT; openniIndex++) {
					ofxOpenNIJoint& joint = user.getJoint((Joint) openniIndex);
					positions[openniIndex] = joint.getWorldPosition();
					positions[openniIndex].x *= -1; // openni is mirrored left/right
					if(openniIndex == 0 || positions[openniIndex].y < floorOffset) {
						floorOffset = positions[openniIndex].y;
					}
				}
				
				vector<ofQuaternion> orientations(JOINT_COUNT);
				
				ofVec3f torsoToNeck = positions[JOINT_NECK] - positions[JOINT_TORSO];
				ofVec3f torsoToLeftHip = positions[JOINT_LEFT_HIP] - positions[JOINT_TORSO];
				ofVec3f torsoToRightHip = positions[JOINT_RIGHT_HIP] - positions[JOINT_TORSO];
				ofVec3f neckToTorso = positions[JOINT_TORSO] - positions[JOINT_NECK];
				ofVec3f neckToRightShoulder = positions[JOINT_RIGHT_SHOULDER] - positions[JOINT_NECK];
				ofVec3f headToNeck = positions[JOINT_NECK] - positions[JOINT_HEAD];
				ofVec3f leftShoulderToLeftElbow = positions[JOINT_LEFT_ELBOW] - positions[JOINT_LEFT_SHOULDER];
				ofVec3f leftElbowToLeftHand = positions[JOINT_LEFT_HAND] - positions[JOINT_LEFT_ELBOW];
				ofVec3f rightShoulderToRightElbow = positions[JOINT_RIGHT_ELBOW] - positions[JOINT_RIGHT_SHOULDER];
				ofVec3f rightElbowToRightHand = positions[JOINT_RIGHT_HAND] - positions[JOINT_RIGHT_ELBOW];
				ofVec3f leftHipToLeftKnee = positions[JOINT_LEFT_KNEE] - positions[JOINT_LEFT_HIP];
				ofVec3f leftKneeToLeftFoot = positions[JOINT_LEFT_FOOT] - positions[JOINT_LEFT_KNEE];
				ofVec3f rightHipToRightKnee = positions[JOINT_RIGHT_KNEE] - positions[JOINT_RIGHT_HIP];
				ofVec3f rightKneeToRightFoot = positions[JOINT_RIGHT_FOOT] - positions[JOINT_RIGHT_KNEE];
				
				orientations[JOINT_TORSO] = makeRotate(torsoToNeck, torsoToRightHip);
				orientations[JOINT_NECK] = makeRotate(neckToTorso, neckToRightShoulder);
				orientations[JOINT_HEAD] = makeRotate(headToNeck, neckToRightShoulder);
				orientations[JOINT_LEFT_SHOULDER] = makeRotate(leftShoulderToLeftElbow, neckToRightShoulder);
				orientations[JOINT_LEFT_ELBOW] = makeRotate(leftElbowToLeftHand, leftShoulderToLeftElbow);
				orientations[JOINT_LEFT_HAND] = orientations[JOINT_LEFT_ELBOW];
				orientations[JOINT_RIGHT_SHOULDER] = makeRotate(rightShoulderToRightElbow, neckToRightShoulder);
				orientations[JOINT_RIGHT_ELBOW] = makeRotate(rightElbowToRightHand, rightShoulderToRightElbow);
				orientations[JOINT_RIGHT_HAND] = orientations[JOINT_RIGHT_ELBOW];
				orientations[JOINT_LEFT_HIP] = makeRotate(leftHipToLeftKnee, torsoToLeftHip);
				orientations[JOINT_LEFT_KNEE] = makeRotate(leftKneeToLeftFoot, leftHipToLeftKnee);
				orientations[JOINT_LEFT_FOOT] = orientations[JOINT_LEFT_KNEE];
				orientations[JOINT_RIGHT_HIP] = makeRotate(rightHipToRightKnee, torsoToRightHip);
				orientations[JOINT_RIGHT_KNEE] = makeRotate(rightKneeToRightFoot, rightHipToRightKnee);
				orientations[JOINT_RIGHT_FOOT] = orientations[JOINT_RIGHT_KNEE];		
				
				for(int ramIndex = 0; ramIndex < RAM_JOINT_COUNT; ramIndex++) {
					int openniIndex = jointMapping[ramIndex];
					ofVec3f position = positions[openniIndex]; // process a copy
					position -= openniCenter;
					position.y -= floorOffset;
					position *= ramScale;
					msg.addStringArg(ramJointName[ramIndex]);
					msg.addFloatArg(position.x);
					msg.addFloatArg(position.y);
					msg.addFloatArg(position.z);
					// send zero orientation
					ofQuaternion& orientation = orientations[openniIndex];
					ofVec3f axis;
					float angle;
					orientation.getRotate(angle, axis);
					msg.addFloatArg(angle);
					msg.addFloatArg(axis.x);
					msg.addFloatArg(axis.y);
					msg.addFloatArg(axis.z);				
				}
				msg.addFloatArg(ofGetElapsedTimef());
				bundle.addMessage(msg);
			}
		}
		osc.sendBundle(bundle);
	}
}

void ofApp::draw() {
	ofSetColor(255);
	openNI.drawDepth();
	for(int i = 0; i < openNI.getNumTrackedUsers(); i++) {
		openNI.getTrackedUser(i).drawSkeleton();
	}
}

void ofApp::userEvent(ofxOpenNIUserEvent & event) {
	ofLogNotice() << getUserStatusAsString(event.userStatus) << "for user" << event.id << "from device" << event.deviceID;
}

void ofApp::exit() {
	openNI.stop();
}
