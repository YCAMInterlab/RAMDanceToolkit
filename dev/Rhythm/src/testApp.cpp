#include "testApp.h"
#include "ofxSimpleParticleEngine.h"


static const string myActorName = "Ando_2012-09-01_18-49-10";

ofxSimpleParticleEngine pe;
Noise *noise;

const int kMaxRecords = 90;
vector<ramA> playingActor;
vector<ofVec3f> recordingActor;

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
	oscReceiver.setup(10000);
	
	// enable ramBaseApp::setup, update, draw, exit
	ramEnableAllEvents();
	
	//
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	
	//
	curPlayingIndex = 0;
	recordingPoints.clear();
	playingPoints.clear();
	
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
	while (oscReceiver.hasWaitingMessages())
	{
		ofxOscMessage m;
		oscReceiver.getNextMessage(&m);
		updateWithOscMessage(m);
	}
	
	ramNode &node = getActor(myActorName).getNode(targetJoint);
	recordingPoints.push_back(node.getPosition());
	curPlayingIndex++;
	
	if (curPlayingIndex >= kMaxRecords-1)
	{
		// data
		playingPoints = recordingPoints;
		recordingPoints.clear(); cout << "cleared" << endl;
		curPlayingIndex = 0;
		
		// line
//		for (int i=0; i<kNumDuplicates; i++) [i][i].clear();
		duplicatedActors.clear();
	}
	
	
	if ( !playingPoints.empty() )
	{
		for (int i=0; i<curPlayingIndex; i++)
		{
			
			ofVec3f &v = playingPoints.at(curPlayingIndex);
			
			// pe
			pe.emit(v);

			
			// line
			for (int i=0; i<kNumDuplicates; i++)
			{
				ac.addVertex(v);
			}
		}
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
		if ( !playingPoints.empty() && bLine )
		{
			line[0].draw();
		}
		
		
		// particle
		if (bParticle)
			pe.draw();
		
		
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
	int division = 600/50.0f;
	float size = 50.0f;
	const ofColor& c1(200);
	const ofColor& c2(230);
	
	ofPushStyle();
	ofFill();
	
	ofPushMatrix();
    ofRotate( 90.0f, 1.0f, 0.0f, 0.0f );
	
	if ( ofGetRectMode() != OF_RECTMODE_CENTER )
	{
		float w = division*size;
		ofTranslate( -w/2.0f+size/2.0f, -w/2.0f+size/2.0f );
	}
	
	glNormal3f( 0.0f, 1.0f, 0.0f );
	
	glEnable(GL_DEPTH_TEST);
	for (int i=0; i<division; i++)
	{
		for (int j=0; j<division; j++)
		{
			if ( ( i%2==0 && j%2== 0 ) || ( i%2==1 && j%2== 1 ) ) ofSetColor( c1 );
			else ofSetColor( c2 );
			ofRect( i*size, j*size, size, size );
		}
	}
	glDisable(GL_DEPTH_TEST);
	
	ofPopMatrix();
	ofPopStyle();
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
			
		default:
			break;
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

