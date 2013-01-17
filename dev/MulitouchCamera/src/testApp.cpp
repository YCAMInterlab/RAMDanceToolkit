#include "testApp.h"

#import "Trackpad.h"

ofEasyCam cam, camStart;

float scrollScale = 5;
float magnifyScale = 500;

// rotation is in degrees, counter-clockwise is positive
float rotationVelocity, rotationPosition;
ofVec2f swipeVelocity, swipePosition;
float magnifyVelocity, magnifyPosition;
ofVec2f scrollVelocity, scrollPosition;
ofVec2f mousePosition;

enum GestureName {
	TRACKPAD_NONE = 0,
	TRACKPAD_ROTATE,
	TRACKPAD_SWIPE,
	TRACKPAD_MAGNIFY,
	TRACKPAD_SCROLL
};
int prevGesture;

void trackpad_begin_gesture() {
	rotationVelocity = 0, rotationPosition = 0;
	swipeVelocity = ofVec2f(), swipePosition = ofVec2f();
	magnifyVelocity = 0, magnifyPosition = 0;
	scrollVelocity = ofVec2f(), scrollPosition = ofVec2f();
	prevGesture = TRACKPAD_NONE;
	
	camStart = cam;
}
void trackpad_end_gesture() {
}

void checkGesture(int curGesture) {
	if(prevGesture != TRACKPAD_NONE && prevGesture != curGesture) {
		trackpad_end_gesture();
		trackpad_begin_gesture();
	}
	prevGesture = curGesture;
}

void trackpad_rotate(float rotation) {
	checkGesture(TRACKPAD_ROTATE);
	
	rotationVelocity = rotation;
	rotationPosition += rotationVelocity;
	
	cam = camStart;
	cam.pan(-rotationPosition); // should be orbit instead of pan
}
void trackpad_swipe(float delta_x, float delta_y) {
	checkGesture(TRACKPAD_SWIPE);
	
	swipeVelocity.set(delta_x, delta_y);
	swipePosition += swipeVelocity;
}
void trackpad_magnify(float magnification) {
	checkGesture(TRACKPAD_MAGNIFY);
	
	magnifyVelocity = magnification;
	magnifyPosition += magnifyVelocity;
	
	cam = camStart;
	cam.dolly(magnifyScale * -magnifyPosition);
}
void trackpad_mouse_move(float x, float y) {
	mousePosition.set(x, y);
}
void trackpad_mouse_scroll(float delta_x, float delta_y) {
	checkGesture(TRACKPAD_SCROLL);
	
	scrollVelocity.set(delta_x, delta_y);
	scrollPosition += scrollVelocity;
	
	cam = camStart;
	cam.truck(scrollScale * scrollPosition.x);
	cam.boom(scrollScale * -scrollPosition.y);
}
void trackpad_left_mouse_down(float x, float y) {}
void trackpad_left_mouse_up(float x, float y) {}
void trackpad_left_mouse_dragged(float x, float y, float delta_x, float delta_y) {}
void trackpad_right_mouse_down(float x, float y) {}
void trackpad_right_mouse_up(float x, float y) {}
void trackpad_right_mouse_dragged(float x, float y, float delta_x, float delta_y) {}

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	mesh.load("lofi-bunny.ply");
	
	trackpad_set_begin_gesture_callback(trackpad_begin_gesture);
	trackpad_set_end_gesture_callback(trackpad_end_gesture);
	
	trackpad_set_rotate_callback(trackpad_rotate);
	trackpad_set_swipe_callback(trackpad_swipe);
	trackpad_set_magnify_callback(trackpad_magnify);
	trackpad_set_mouse_move_callback(trackpad_mouse_move);
	trackpad_set_mouse_scroll_callback(trackpad_mouse_scroll);
	
	trackpad_set_left_mouse_down_callback(trackpad_left_mouse_down);
	trackpad_set_left_mouse_up_callback(trackpad_left_mouse_up);
	trackpad_set_left_mouse_dragged_callback(trackpad_left_mouse_dragged);
	
	trackpad_set_right_mouse_down_callback(trackpad_right_mouse_down);
	trackpad_set_right_mouse_up_callback(trackpad_right_mouse_up);
	trackpad_set_right_mouse_dragged_callback(trackpad_right_mouse_dragged);
	
	trackpad_init();
}

//--------------------------------------------------------------
void testApp::update(){
	
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackgroundGradient(ofColor(64), ofColor(0));
	
	ofSetColor(255);
	ofDrawBitmapString("rotate: " + ofToString(rotationPosition), 10, 20);
	ofDrawBitmapString("swipe: " + ofToString(swipePosition), 10, 40);
	ofDrawBitmapString("magnify: " + ofToString(magnifyPosition), 10, 60);
	ofDrawBitmapString("scroll: " + ofToString(scrollPosition), 10, 80);
	ofDrawBitmapString("position: " + ofToString(mousePosition), 10, 100);
	cam.begin();
	
	ofPushMatrix();
	ofTranslate(0, -180, 0);
	ofDrawGrid(1000, 40, false, false, true, false);
	ofPopMatrix();
	
	ofSetColor(ofColor::gray);	
	mesh.drawWireframe();
	
	glPointSize(2);
	ofSetColor(ofColor::white);
	mesh.drawVertices();
	cam.end();
	
	int n = mesh.getNumVertices();
	float nearestDistance = 0;
	ofVec2f nearestVertex;
	int nearestIndex;
	ofVec2f mouse(mouseX, mouseY);
	for(int i = 0; i < n; i++) {
		ofVec3f cur = cam.worldToScreen(mesh.getVertex(i));
		float distance = cur.distance(mouse);
		if(i == 0 || distance < nearestDistance) {
			nearestDistance = distance;
			nearestVertex = cur;
			nearestIndex = i;
		}
	}
	
	ofSetColor(ofColor::gray);
	ofLine(nearestVertex, mouse);
	
	ofNoFill();
	ofSetColor(ofColor::yellow);
	ofSetLineWidth(2);
	ofCircle(nearestVertex, 4);
	ofSetLineWidth(1);
	
	ofVec2f offset(10, -10);
	ofDrawBitmapStringHighlight(ofToString(nearestIndex), mouse + offset);
}
