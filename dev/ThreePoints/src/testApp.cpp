#include "testApp.h"


static const ofColor cyanPrint = ofColor::fromHex(0x00abec);
static const ofColor magentaPrint = ofColor::fromHex(0xec008c);
static const ofColor yellowPrint = ofColor::fromHex(0xffee00);

//--------------------------------------------------------------
void rotateToNormal(ofVec3f normal) {
	normal.normalize();
	
	float rotationAmount;
	ofVec3f rotationAngle;
	ofQuaternion rotation;
	
	ofVec3f axis(0, 0, 1);
	rotation.makeRotate(axis, normal);
	rotation.getRotate(rotationAmount, rotationAngle);
	ofRotate(rotationAmount, rotationAngle.x, rotationAngle.y, rotationAngle.z);
}

// from the bourkster.
// https://gist.github.com/2724338
//--------------------------------------------------------------
bool lineLineIntersectSegment(ofVec3f p1, ofVec3f p2, ofVec3f p3, ofVec3f p4, ofVec3f &pa, ofVec3f &pb) {
	float mua = 0;
	float mub = 0;
	
	ofVec3f p13,p43,p21;
	
	float d1343,d4321,d1321,d4343,d2121;
	float numer,denom;
	p13 = p1 - p3;
	p43 = p4 - p3;
	
	
	if (ABS(p43.x) < FLT_EPSILON && ABS(p43.y) < FLT_EPSILON && ABS(p43.z) < FLT_EPSILON)
		return false;
	
	p21 = p2 - p1;
	
	if (ABS(p21.x) < FLT_EPSILON && ABS(p21.y) < FLT_EPSILON && ABS(p21.z) < FLT_EPSILON)
		return false;
	
	d1343 = p13.x * p43.x + p13.y * p43.y + p13.z * p43.z;
	d4321 = p43.x * p21.x + p43.y * p21.y + p43.z * p21.z;
	d1321 = p13.x * p21.x + p13.y * p21.y + p13.z * p21.z;
	d4343 = p43.x * p43.x + p43.y * p43.y + p43.z * p43.z;
	d2121 = p21.x * p21.x + p21.y * p21.y + p21.z * p21.z;
	
	denom = d2121 * d4343 - d4321 * d4321;
	if (ABS(denom) < FLT_EPSILON)
		return false;
	numer = d1343 * d4321 - d1321 * d4343;
	
	mua = numer / denom;
	mub = (d1343 + d4321 * mua) / d4343;
	
	pa = p1 + p21 * mua;
	pb = p3 + p43 * mub;
	
	return true;
}

// https://gist.github.com/2724338
//--------------------------------------------------------------
ofVec3f lineLineIntersection(ofVec3f p1, ofVec3f p2, ofVec3f p3, ofVec3f p4) {
	ofVec3f pa, pb;
	lineLineIntersectSegment(p1,p2,p3,p4,pa,pb);
	return (pa+pb)/2.f;
}

//--------------------------------------------------------------
void findCircle(const ofVec3f& a, const ofVec3f& b, const ofVec3f& c, ofVec3f& center, ofVec3f& normal, float& radius) {
	ofVec3f ab = a - b, cb = c - b;
	normal = ab.getCrossed(cb);
	ofVec3f abm = ab / 2, cbm = cb / 2;
	ofVec3f abo = abm.getRotated(90, normal), cbo = cbm.getRotated(90, normal);
	abm += b, cbm += b;
	center = lineLineIntersection(abm, abm + abo, cbm, cbm + cbo);
	radius = center.distance(b);
	normal.normalize();
}

void testApp::setupUI() {
	float dim = 20;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING; 
	float length = 320 - xInit;
	gui = new ofxUICanvas(0, 0, length + xInit * 2, ofGetHeight());
//	gui->setFont("/Users/motoishmz/Library/Fonts/din-webfont.ttf");
	ofColor cb(64, 192),
	co(192, 192),
	coh(128, 192),
	cf(240, 255),
	cfh(128, 255),
	cp(96, 192),
	cpo(255, 192);
	gui->setUIColors(cb, co, coh, cf, cfh, cp, cpo);
	
	gui->addLabel("Joint Selection", OFX_UI_FONT_LARGE);
	gui->addLabelToggle("Show spheres", &showSpheres, length, dim);
	gui->addLabelToggle("Show circles", &showCircles, length, dim);
	gui->addLabelToggle("Show rects", &showRects, length, dim);
	gui->addLabelToggle("Inverted spheres", &invertSpheres, length, dim);
	gui->addLabelToggle("Show circle bisector", &showCircleBisector, length, dim);
	gui->addLabelToggle("Show center circles", &showCenterCircles, length, dim);
	gui->addSlider("Point size", 1, 10, &pointSize, length, dim);
	gui->addSlider("Cross length", 1, 1000, &crossLength, length, dim);
	gui->addSlider("Rect radius", 1, 1000, &rectRadius, length, dim);
	gui->addSlider("Max invert radius", 1, 10000, &maxInvertRadius, length, dim);
	gui->addSlider("Circle resolution", 3, 30, &circleResolution, length, dim);
	int matrixSize = 10;
	// plug these in to draw actor check
	gui->addWidgetDown(new ofxUIToggleMatrix(matrixSize, matrixSize, 5, 5, "Joint 1"));
	gui->addWidgetDown(new ofxUIToggleMatrix(matrixSize, matrixSize, 5, 5, "Joint 2"));
	gui->addWidgetDown(new ofxUIToggleMatrix(matrixSize, matrixSize, 5, 5, "Joint 3"));
}

#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(120);
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	ofSetLineWidth(2);
	ofSetVerticalSync(true);
	ofBackground(0);
	oscReceiver.setup(10000);
	
	// enable ramBaseApp::setup, update, draw, exit
	ramEnableAllEvents();
	
	showRects = false;
	showSpheres = false;
	invertSpheres = true;
	crossLength = 100;
	rectRadius = 100;
	pointSize = 5;
	maxInvertRadius = 2000;
	showCircleBisector = true;
	circleResolution = 30;
	showCenterCircles = true;
	
	setupUI();
}

//--------------------------------------------------------------
void testApp::update()
{
	oscReceiver.update();
	
	glPointSize(pointSize);
	ofSetCircleResolution(circleResolution);
}

//--------------------------------------------------------------
void testApp::draw()
{
	
}


#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawFloor()
{
//	ramBasicFloor(600., 50., ofColor(255, 64), ofColor(255, 96));
	ramBasicFloor(ramFloor::FLOOR_NONE);
}

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	ofPushStyle();
	ofNoFill();
	ofSetColor(0);
	
	for (int i=0; i<actor.getNumNode(); i++)
	{
		const ramNode &node = actor.getNode(i);
		
		ofPushMatrix();
		node.transformBegin();
		ofSetColor(yellowPrint);
		ofBox((i==ramActor::JOINT_HEAD) ? 6 : 3);
		if(showRects) {
			ofPushStyle();
			ofFill();
			ofSetColor(255, 128);
			ofRect(0, 0, 100, 30);
			ofPopStyle();
		}
		
		ofSetColor(255);
		node.transformEnd();
		ofPopMatrix();
		
		if (node.hasParent())
		{
			ramNode* parent = node.getParent();
			ofLine(node, *parent);
			if(parent->hasParent() && ((i==ramActor::JOINT_RIGHT_WRIST) ||
																 (i==ramActor::JOINT_RIGHT_ANKLE) ||
																 (i==ramActor::JOINT_LEFT_WRIST) ||
																 (i==ramActor::JOINT_LEFT_ANKLE)))
			{
				ramNode* grandparent = parent->getParent();
				ofVec3f a = node, b = *parent, c = *grandparent;
				ofVec3f normal = (a - b).cross(c - b);
				normal.normalize();
				ofSetColor(magentaPrint);
				ofLine(b - normal * crossLength, b + normal * crossLength);
				
				ofVec3f center;
				float radius;
				findCircle(a, b, c, center, normal, radius);
				ofPushMatrix();
				ofTranslate(center);
				rotateToNormal(normal);
				if(showSpheres) {
					ofSetColor(255, 32);
					ofSphere(0, 0, radius);
				}
				ofSetColor(yellowPrint);
				if(showCircles) {
					ofCircle(0, 0, radius);
				}
				ofPopMatrix();
				if(showCircleBisector) {
					ofLine(b, b + (center - b) * 2);
				}
				
				ofPushMatrix();
				ofTranslate(b);
				rotateToNormal(normal);
				ofSetColor(cyanPrint);
				ofRect(-rectRadius, -rectRadius, 2*rectRadius, 2*rectRadius);
				if(invertSpheres) {
					ofSetColor(255, 32);
					ofSphere(0, 0, maxInvertRadius / radius);
				}
				if(showCenterCircles) {
					ofSetColor(yellowPrint);
					ofCircle(0, 0, radius);
				}
				ofPopMatrix();
			}
		}
	}
	
	ofPopStyle();
}

//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
{
}


#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{
	
}

