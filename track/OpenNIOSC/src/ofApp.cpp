#include "ofApp.h"

ofVec3f upDirection(0, -1, 0), downDirection = -upDirection, leftDirection(-1, 0, 0), rightDirection = -leftDirection;
ofVec3f defaultDirection[] = {
	ofVec3f(0), //JOINT_TORSO = 0,
	upDirection, //JOINT_NECK,
	upDirection, //JOINT_HEAD,
	
	rightDirection, //JOINT_LEFT_SHOULDER,
	downDirection, //JOINT_LEFT_ELBOW,
	downDirection, //JOINT_LEFT_HAND,
	
	leftDirection, //JOINT_RIGHT_SHOULDER,
	downDirection, //JOINT_RIGHT_ELBOW,
	downDirection, //JOINT_RIGHT_HAND,
	
	rightDirection, //JOINT_LEFT_HIP,
	downDirection, //JOINT_LEFT_KNEE,
	downDirection, //JOINT_LEFT_FOOT,
	
	leftDirection, //JOINT_RIGHT_HIP,
	downDirection, //JOINT_RIGHT_KNEE,
	downDirection, //JOINT_RIGHT_FOOT,
};

int jointParents[] = {
	-1,
	JOINT_TORSO,
	JOINT_NECK,
	
	JOINT_NECK,
	JOINT_LEFT_SHOULDER,
	JOINT_LEFT_ELBOW,
	
	JOINT_NECK,
	JOINT_RIGHT_SHOULDER,
	JOINT_RIGHT_ELBOW,
	
	JOINT_TORSO,
	JOINT_LEFT_HIP,
	JOINT_LEFT_KNEE,
	
	JOINT_TORSO,
	JOINT_RIGHT_HIP,
	JOINT_RIGHT_KNEE
};

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

const static int RAM_JOINT_COUNT = 23;

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

vector<ofQuaternion> getOrientation(const vector<ofVec3f>& positions) {
	vector<ofQuaternion> orientations;
	for(int i = 0; i < positions.size(); i++) {
		if(jointParents[i] == -1) {
			orientations.push_back(ofQuaternion()); // zero rotation for no parents
		} else {
			int parent = jointParents[i];
			ofVec3f direction = positions[i] - positions[parent];
			ofQuaternion orientation;
			orientation.makeRotate(defaultDirection[i], direction);
			orientations.push_back(orientation);
		}
	}
	return orientations;
}

// openni is in millimeters, ram is in centimeters
const float ramScale = .1;

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
					if(openniIndex == 0 || positions[openniIndex].y < floorOffset) {
						floorOffset = positions[openniIndex].y;
					}
				}
				vector<ofQuaternion> orientations = getOrientation(positions);
				
				for(int ramIndex = 0; ramIndex < RAM_JOINT_COUNT; ramIndex++) {
					int openniIndex = jointMapping[ramIndex];
					ofVec3f position = positions[openniIndex]; // process a copy
					position -= openniCenter;
					position.y -= floorOffset;
					position.x *= -1; // openni is mirrored left/right
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
