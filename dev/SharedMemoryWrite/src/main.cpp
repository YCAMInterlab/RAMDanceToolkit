#include "ofAppGlutWindow.h"
#include "ofMain.h"

#include "Poco/SharedMemory.h"
using Poco::SharedMemory;

class SharedData {
public:
	char str[256];
	int x, y;
};
	
class ofApp : public ofBaseApp {
public:
	SharedMemory mem;
	SharedData sharedData;
	string str;

	void setup() {
		ofSetFrameRate(120);
		ofSetVerticalSync(true);
		mem = SharedMemory("sharedMemoryTest", sizeof(SharedData), SharedMemory::AM_WRITE, 0, true);
	}
	
	void uploadSharedData() {
		strcpy(sharedData.str, str.c_str());
		memcpy((void*) mem.begin(), (const void*) &sharedData, sizeof(SharedData));
	}
	
	void downloadSharedData() {
		str = sharedData.str;
		memcpy((void*) &sharedData, (const void*) mem.begin(), sizeof(SharedData));
	}
	
	void update() {
		downloadSharedData();
	}
	
	void draw() {
		ofBackground(0);
		ofSetColor(255);
		ofDrawBitmapString("watashi wa kaku>>", 10, 20);
		ofDrawBitmapString(ofToString(ofToHex(mem.begin())) + "-" + ofToString(ofToHex(mem.end())), 10, 40);
		ofDrawBitmapString(sharedData.str, 10, 60);
		ofEllipse(sharedData.x, sharedData.y, 4, 4);
		ofLine(0, sharedData.y, ofGetWidth(), sharedData.y);
		ofLine(sharedData.x, 0, sharedData.x, ofGetHeight());
	}
	
	void mouseMoved(int x, int y) {
		if(ofRectangle(0, 0, ofGetWidth(), ofGetHeight()).inside(x, y)) {
			sharedData.x = x;
			sharedData.y = y;
			uploadSharedData();
		}
	}
	
	void keyPressed(int key) {
		if(key == OF_KEY_BACKSPACE) {
			if(str.size() > 0) {
				str.resize(str.size() - 1);
			}
		} else {
			str += key;
		}
		uploadSharedData();
	}
};

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 200, 100, OF_WINDOW);
	ofRunApp(new ofApp());
}
