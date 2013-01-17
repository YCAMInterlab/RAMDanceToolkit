#include "testApp.h"
#include "ofxSimpleParticleEngine.h"


static const string myActorName = "Ando_2012-09-01_18-49-10";

ofxSimpleParticleEngine pe;
Noise *noise;

const int kMaxRecords = 90;
vector<ramActor> playingActors;
vector<ramActor> recordingActors;

int curPlayingIndex;
int targetJoint;

const int kNumDuplicates = 9;
vector<ramActor> duplicatedActors;

bool bActor, bParticle, bLine;


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
	
	//
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	
	//
	curPlayingIndex = 0;
	recordingActors.clear();
	playingActors.clear();
	
	//
	for (int i=0; i<kNumDuplicates; i++) duplicatedActors.clear();
	targetJoint = ramActor::JOINT_RIGHT_HAND;
	
	//
	pe.setup(10000);
	pe.addForceFactor(new Gravity);
	pe.addForceFactor(new Floor);
	
	bActor = false;
	bParticle = false;
	bLine = false;
}

//--------------------------------------------------------------
void testApp::update()
{
	oscReceiver.update();
	
	recordingActors.push_back( getActor(myActorName) );
	curPlayingIndex++;
	
	if (curPlayingIndex >= kMaxRecords-1)
	{
		// data
		playingActors = recordingActors;
		recordingActors.clear(); cout << "cleared" << endl;
		curPlayingIndex = 0;
		
		// line
		duplicatedActors.clear();
	}
	
	
	if ( !playingActors.empty() )
	{
		const ofVec3f &v = playingActors.at(curPlayingIndex).getNode(targetJoint).getPosition();
		for(int i=0; i<100; i++) pe.emit(v);
	}
	
	pe.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofBackgroundGradient( ofColor( 240 ), ofColor( 60 ) );
	ofSetColor(255);
	
	ramCameraBegin();
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_DEPTH_TEST);
		
		// line
		ofPolyline line;
		if ( !playingActors.empty() )
		{
			for (int i=0; i<curPlayingIndex; i++)
			{
				ramNode &node = playingActors.at(i).getNode(targetJoint);
				line.addVertex(node.getPosition());
			}
		}
		
		if (bLine)
		{
			ofSetColor(255, 255, 0);
			line.draw();
		}
		
		// particle
		if (bParticle)
		{
			ofSetColor(255);
			pe.draw();
		}
		
		
		glDisable(GL_DEPTH_TEST);
		glPopAttrib();
	}
	ramCameraEnd();
}


//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	
	if (!bActor) return;
	
	glEnable(GL_DEPTH_TEST);
	ofPushStyle();
	ofNoFill();
    ofSetColor(80);
	
	for (int i=0; i<actor.getNumNode(); i++)
    {
		const ramNode &node = actor.getNode(i);
		const int size = (i==ramActor::JOINT_HEAD) ? 6 : 3;
		
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushMatrix();
		node.transformBegin();
		ofBox(size);
		node.transformEnd();
		glPopAttrib();
		glPopMatrix();
		
        if (node.hasParent())
            ofLine(node, *node.getParent());
    }
	
    ofPopStyle();
	glDisable(GL_DEPTH_TEST);
}

//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
{
	
}


#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawFloor()
{
	ramBasicFloor(600., 50.);
}



#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	switch (key)
	{
		case 'a': bActor ^= true; break;
		case 's': bLine ^= true; break;
		case 'd': bParticle ^= true; break;
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

