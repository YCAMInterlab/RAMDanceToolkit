#include "ofApp.h"

namespace openni {
	enum Joint {
		JOINT_TORSO = 0,
		JOINT_NECK,
		JOINT_HEAD,
		
		JOINT_LEFT_SHOULDER,
		JOINT_LEFT_ELBOW,
		JOINT_LEFT_HAND,
		
		JOINT_RIGHT_SHOULDER,
		JOINT_RIGHT_ELBOW,
		JOINT_RIGHT_HAND,
		
		JOINT_LEFT_HIP,
		JOINT_LEFT_KNEE,
		JOINT_LEFT_FOOT,
		
		JOINT_RIGHT_HIP,
		JOINT_RIGHT_KNEE,
		JOINT_RIGHT_FOOT,
		
		NUM_JOINTS
	};
	
	ofVec3f up(0, -1, 0), down = -up, left(-1, 0, 0), right = -left;
	ofVec3f defaultDirection[] = {
		ofVec3f(0), //JOINT_TORSO = 0,
		up, //JOINT_NECK,
		up, //JOINT_HEAD,
		
		right, //JOINT_LEFT_SHOULDER,
		down, //JOINT_LEFT_ELBOW,
		down, //JOINT_LEFT_HAND,
		
		left, //JOINT_RIGHT_SHOULDER,
		down, //JOINT_RIGHT_ELBOW,
		down, //JOINT_RIGHT_HAND,
		
		right, //JOINT_LEFT_HIP,
		down, //JOINT_LEFT_KNEE,
		down, //JOINT_LEFT_FOOT,
		
		left, //JOINT_RIGHT_HIP,
		down, //JOINT_RIGHT_KNEE,
		down, //JOINT_RIGHT_FOOT,
	};
	
	openni::Joint jointParents[] = {
		-1,
		openni::JOINT_TORSO,
		openni::JOINT_NECK,
		
		openni::JOINT_NECK,
		openni::JOINT_LEFT_SHOULDER,
		openni::JOINT_LEFT_ELBOW,
		
		openni::JOINT_NECK,
		openni::JOINT_RIGHT_SHOULDER,
		openni::JOINT_RIGHT_ELBOW,
		
		openni::JOINT_TORSO,
		openni::JOINT_LEFT_HIP,
		openni::JOINT_LEFT_KNEE,
		
		openni::JOINT_TORSO,
		openni::JOINT_RIGHT_HIP,
		openni::JOINT_RIGHT_KNEE
	}
	
	openni::Joint jointMapping[] = {
		openni::JOINT_TORSO, //ram::JOINT_HIPS
		openni::JOINT_TORSO, //ram::JOINT_ABDOMEN,
		openni::JOINT_NECK, //ram::JOINT_CHEST,
		openni::JOINT_NECK, //ram::JOINT_NECK,
		openni::JOINT_HEAD, //ram::JOINT_HEAD,
		
		openni::JOINT_LEFT_HIP, //ram::JOINT_LEFT_HIP,
		openni::JOINT_LEFT_KNEE, //ram::JOINT_LEFT_KNEE,
		openni::JOINT_LEFT_FOOT, //ram::JOINT_LEFT_ANKLE,
		openni::JOINT_LEFT_FOOT, //ram::JOINT_LEFT_TOE,
		
		openni::JOINT_RIGHT_HIP, //ram::JOINT_RIGHT_HIP,
		openni::JOINT_RIGHT_KNEE, //ram::JOINT_RIGHT_KNEE,
		openni::JOINT_RIGHT_FOOT, //ram::JOINT_RIGHT_ANKLE,
		openni::JOINT_RIGHT_FOOT, //ram::JOINT_RIGHT_TOE,
		
		openni::JOINT_LEFT_SHOULDER, //ram::JOINT_LEFT_COLLAR,
		openni::JOINT_LEFT_SHOULDER, //ram::JOINT_LEFT_SHOULDER,
		openni::JOINT_LEFT_ELBOW, //ram::JOINT_LEFT_ELBOW,
		openni::JOINT_LEFT_HAND, //ram::JOINT_LEFT_WRIST,
		openni::JOINT_LEFT_HAND, //ram::JOINT_LEFT_HAND,
		
		openni::JOINT_RIGHT_SHOULDER, //ram::JOINT_RIGHT_COLLAR,
		openni::JOINT_RIGHT_SHOULDER, //ram::JOINT_RIGHT_SHOULDER,
		openni::JOINT_RIGHT_ELBOW, //ram::JOINT_RIGHT_ELBOW,
		openni::JOINT_RIGHT_HAND, //ram::JOINT_RIGHT_WRIST,
		openni::JOINT_RIGHT_HAND, //ram::JOINT_RIGHT_HAND,
	};
	
}

namespace ram {
	enum Joint
	{
		JOINT_HIPS = 0,
		JOINT_ABDOMEN,
		JOINT_CHEST,
		JOINT_NECK,
		JOINT_HEAD,
		
		JOINT_LEFT_HIP,
		JOINT_LEFT_KNEE,
		JOINT_LEFT_ANKLE,
		JOINT_LEFT_TOE,
		
		JOINT_RIGHT_HIP,
		JOINT_RIGHT_KNEE,
		JOINT_RIGHT_ANKLE,
		JOINT_RIGHT_TOE,
		
		JOINT_LEFT_COLLAR,
		JOINT_LEFT_SHOULDER,
		JOINT_LEFT_ELBOW,
		JOINT_LEFT_WRIST,
		JOINT_LEFT_HAND,
		
		JOINT_RIGHT_COLLAR,
		JOINT_RIGHT_SHOULDER,
		JOINT_RIGHT_ELBOW,
		JOINT_RIGHT_WRIST,
		JOINT_RIGHT_HAND,
		
		NUM_JOINTS
	};
	
	string jointName[] =
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
}

vector<ofQuaternion> getOrientation(vector<ofVec3f>& positions) {
	vector<ofQuaternion> orientations;
	for(int i = 0; i < positions.size(); i++) {
		if(jointParents[i] == -1) {
			orientations[i] = ofQuaternion(); // zero rotation for no parents
		} else {
			int parent = openni::jointParents[i];
			ofVec3f direction = positions[i] - positions[parent];
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
		for(int i = 0; i < numTrackedUsers; i++) {
			ofxOpenNIUser& user = openNI.getTrackedUser(i);
			if(user.isTracking() && user.isSkeleton()) {
				ofxOscMessage msg;
				msg.setAddress("/ram/skeleton");
				int userId = user.getXnID();
				string actorName = "OpenNI " + ofToString(userId) + " @" + ofToString(deviceId);
				int numJoints = user.getNumJoints();
				msg.addStringArg(actorName);
				msg.addIntArg(ram::NUM_JOINTS);
				
				// should use accelerometer to right things
				// or a custom slider to position people
				float floorOffset = 0;
				vector<ofVec3f> positions(openni::NUM_JOINTS);
				for(int j = 0; j < openni::NUM_JOINTS; j++) {
					ofxOpenNIJoint& joint = user.getJoint((Joint) j);
					positions[i] = joint.getWorldPosition();
					if(j == 0 || positions[i].y < floorOffset) {
						floorOffset = positions[i].y;
					}
				}
				vector<ofQuaternion> orientations = getOrientation(positions);
				
				for(int j = 0; j < ram::NUM_JOINTS; j++) {
					int openniIndex = openni::jointMapping[j];
					ofxOpenNIJoint& joint = user.getJoint((Joint) openniIndex);
					ofVec3f& position = positions[i];
					position -= openniCenter;
					position.y -= floorOffset;
					position.x *= -1; // openni is mirrored left/right
					position *= ramScale;		
					msg.addStringArg(ram::jointName[j]);
					msg.addFloatArg(position.x);
					msg.addFloatArg(position.y);
					msg.addFloatArg(position.z);
					// send zero orientation
					msg.addFloatArg(0);
					msg.addFloatArg(1);
					msg.addFloatArg(0);
					msg.addFloatArg(0);				
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
