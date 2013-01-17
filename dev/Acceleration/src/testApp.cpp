#include "testApp.h"


#include <numeric>

static const string myActorName = "Ando_2012-09-01_18-49-10";

ofMatrix4x4 shadowMat;

typedef struct
{
	ramActor actor;
	
	vector<ramAccelerometer> accelerometers;
	
	void setup(ramActor &a)
	{
		actor = a;
		accelerometers.clear();
		accelerometers.resize(a.getNumNode());
	}
	
} AcceRecord;

deque<AcceRecord> acceRecords;
float currRec;

static const int N_ACCE = 300;
static const int SPEED = 2;

namespace gl
{
    
    //ax + by + cz + d = 0;
    static const float kGroundPlaneXUp[] = { 1.0, 0.0, 0.0, 1.0 };
    static const float kGroundPlaneYUp[] = { 0.0, 1.0, 0.0, 1.0 };
    static const float kGroundPlaneZUp[] = { 0.0, 0.0, 1.0, 1.0 };
    
    inline void calcShadowMatrix(const float groundplane[4],
                                 const float lightpos[3],
                                 float shadowMatrix[16]);
    
    void calcShadowMatrix(const float groundplane[],
                          const float lightpos[],
                          float shadowMatrix[])
    {
        float dot = inner_product(groundplane, groundplane+4, lightpos, 0.f);
        
        for(int y = 0; y < 4;++y) {
            for(int x = 0; x < 4; ++x) {
                
                shadowMatrix[y*4+x] = - groundplane[y]*lightpos[x];
                
                if (x == y) shadowMatrix[y*4+x] += dot;
            }
        }
    }
}


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
	
	// scene setup
	const float lightPosition[] = { -100.0f, 500.0f, 200.0f };
    gl::calcShadowMatrix( gl::kGroundPlaneYUp, lightPosition, shadowMat.getPtr() );
	
	currRec = 0;
	acceRecords.clear();
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
	
	if (getActorManager().getNumActor() > 0)
	{
		ramActor &actor = getActorManager().getActor(myActorName);
		
		AcceRecord record;
		record.setup(actor);
		
		for (int i=0; i<actor.getNumNode(); i++)
		{
			ramNode &node = record.actor.getNode(i);
			ramAccelerometer &acce = record.accelerometers.at(i);
			acce.update(node);
		}
		
		acceRecords.push_back(record);
	}

    while (acceRecords.size() > N_ACCE) acceRecords.pop_front();

    if (acceRecords.size() > SPEED*5)
    {
        currRec += SPEED;
        if (currRec >= acceRecords.size())
            currRec -= acceRecords.size();
    }
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofBackgroundGradient( ofColor( 240 ), ofColor( 60 ) );
	ofNoFill();
	
	
    ramCameraBegin();
	glEnable(GL_DEPTH_TEST);
	
	// actor: recorded
    if ( currRec < acceRecords.size() )
    {
        ramActor &recordedActor = acceRecords.at(currRec).actor;
		vector<ramAccelerometer> &accerelometers = acceRecords.at(currRec).accelerometers;
		
        for ( int i=0; i<recordedActor.getNumNode(); i++ )
        {
			if ( i==0 )
			{
				for ( int j=0; j<acceRecords.size()-1; j++)
				{
					ramNode &n0 = acceRecords.at( j ).actor.getNode( i );
					ramNode &n1 = acceRecords.at( j + 1 ).actor.getNode( i );
					ofLine( n0.getPosition(), n1.getPosition() );
				}
			}
			
			float size = accerelometers.at(i).getAcceleration().length() / 20;
			cout << recordedActor.getNode(i).getName() << " diff: " << size << endl;
			
            const int MAX_SIZE = 20.0;
            if ( size > MAX_SIZE ) size = MAX_SIZE;
			
			ramNode &recordedNode = recordedActor.getNode(i);
            ofBox(recordedNode, size);
			
            if ( recordedNode.getParent() )
            {
				ofLine(recordedNode, *recordedNode.getParent());
            }
        }
    }
	
	glDisable(GL_DEPTH_TEST);
    ramCameraEnd();
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

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	ofPushStyle();
	ofNoFill();
    
    ofColor currSklColor( 110, 20, 20 );
    ofColor recSklColor( 20, 20, 110 );
    ofColor locusColor( 20, 110, 20 );
    
	// actor: realtime
    for (int i=0; i<actor.getNumNode(); i++)
    {
		const ramNode &node = actor.getNode(i);
		
        ofSetColor(currSklColor);
        ofBox(node, 1.0);
        
        if (node.hasParent())
            ofLine(node, *node.getParent());
    }
	
    ofSetDrawBitmapMode( OF_BITMAPMODE_MODEL_BILLBOARD );
    ofVec3f namePos = actor.getNode(ramActor::JOINT_HEAD).getPosition();
    namePos.y += 20.0;
    ofDrawBitmapString(actor.getName(), namePos);
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

