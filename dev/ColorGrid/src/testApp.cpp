#include "testApp.h"

static const ofColor cyanPrint = ofColor::fromHex(0x00abec);
static const ofColor magentaPrint = ofColor::fromHex(0xec008c);
static const ofColor yellowPrint = ofColor::fromHex(0xffee00);

#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(120);
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	ofSetVerticalSync(true);
	//ofBackground(0);
	ofSetBackgroundAuto(false);
	oscReceiver.setup(10000);
	
	// enable ramBaseApp::setup, update, draw, exit
	ramEnableAllEvents();
	
	img.allocate(500, ramActor::NUM_JOINTS, OF_IMAGE_COLOR);
	mode = true;
}

//--------------------------------------------------------------
void testApp::update()
{
	oscReceiver.update();
}

//--------------------------------------------------------------
void testApp::draw()
{	
	img.update();
	ofSetColor(255);
	ofSetMinMagFilters(GL_NEAREST, GL_NEAREST);
	img.draw(0, 0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	for(int y = 0; y < img.getHeight(); y++) {
		for(int x = 0; x < img.getWidth() - 1; x++) {
			img.setColor(x, y, img.getColor(x + 1, y));
		}
	}
	
	for (int i=0; i<actor.getNumNode(); i++)
	{
		if(i < ramActor::NUM_JOINTS) {
			ramNode &node = actor.getNode(i);
			ofColor cur;
			if(mode) {
				ofVec3f base = ofVec3f(1, 0, 0) * node.getOrientationQuat();
				cur = ofColor(ofMap(base.x, -1, 1, 0, 255, true),
											ofMap(base.y, -1, 1, 0, 255, true),
											ofMap(base.z, -1, 1, 0, 255, true));
			} else {
				ofVec3f euler = node.getOrientationEuler();
				ofVec3f base = ofVec3f(1, 0, 0) * node.getOrientationQuat();				
				cur = ofColor::fromHsb(ofMap(euler.z, -180, 180, 0, 255, true),
															 ofMap(base.x, -1, 1, 0, 255, true),
															 ofMap(base.y, -1, 1, 0, 255, true));
			}
			img.setColor(img.getWidth() - 1, i, cur);
			
			ofPushMatrix();
			node.beginTransform();
			ofFill();
			ofBox((i==ramActor::JOINT_HEAD) ? 6 : 3);
			ofSetColor(cur);
			node.endTransform();
			ofPopMatrix();
			
			if (node.hasParent())
			{
				ramNode* parent = node.getParent();
				ofLine(node, *parent);
			}
		}
	}
}

//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
{
}


#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	if(key == 'f') ofToggleFullscreen();
	if(key == ' ') mode = !mode;
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

