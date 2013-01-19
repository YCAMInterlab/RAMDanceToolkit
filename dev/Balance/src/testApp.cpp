#include "testApp.h"
#include "arrow.h"

static const string myActorName = "default";
ofLight light;

ramBalancer balancer;
bool bActor, bBalance, bArrow;

Arrow arrow;

#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(0);
	oscReceiver.setup(10001);
	
	// enable ramBaseApp::setup, update, draw, exit
	ramEnableAllEvents();
	
	arrow.setup();
	bActor = true;
	bBalance = true;
	bArrow = true;
}

//--------------------------------------------------------------
void testApp::update()
{
	oscReceiver.update();
	
	ramActor &actor = getActorManager().getActor(myActorName);
	balancer.update(actor);
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofBackgroundGradient( ofColor( 240 ), ofColor( 60 ) );
	ofPushStyle();
	
	if (getActorManager().getNumActor() > 0)
	{
		ramCameraBegin();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_DEPTH_TEST);
		
		ofVec3f balance = balancer.getAvarage().getNormalized();
		float actorHeight = 156.;
		
		ofVec3f chest = getActorManager().getActor(myActorName).getNode(ramActor::JOINT_CHEST).getPosition();
		chest.y = 0.;
		
		if (bBalance)
		{
			ofSetColor(255, 0, 0);
			ofLine(chest, balance * actorHeight);
		}
//		
//		if (bArrow)
//		{
//			float scale = balancer.getAvarage().length() * 2.5;
//			static const float max_scale = 20.;
//			static const float min_scale = 1.;
//			
//			scale = (scale > max_scale) ? max_scale :
//					(scale < min_scale) ? min_scale : scale;
//			
//			glTranslatef(balance.x, 200, balance.z);
//			glScalef(scale, 10.0, scale);
//			glRotatef(angle, 0.f, 1.f, 0.f);
//			arrow.drawElements(GL_TRIANGLES, kNumFaces);
//		}
		
		glDisable(GL_DEPTH_TEST);
		glPopAttrib();
		ramCameraEnd();
	}
	
	ofPopStyle();
}




#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawFloor()
{
	ramBasicFloor(600., 50.);
}

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	if (bActor) return;
	
	ofPushStyle();
	ofNoFill();
    ofSetColor(150);
	
	for (int i=0; i<actor.getNumNode(); i++)
    {
		const ramNode &node = actor.getNode(i);
		
		ofBox(node, (i==ramActor::JOINT_HEAD) ? 6 : 3);
        
        if (node.hasParent())
            ofLine(node, *node.getParent());
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
	switch (key)
	{
		case 'a': bActor ^= true; break;
		case 's': bBalance ^= true; break;
		case 'f': ofToggleFullscreen(); break;
	}
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

