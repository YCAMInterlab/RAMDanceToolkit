#include "testApp.h"


static const string myActorName = "Ando_2012-09-01_18-49-10";
ofLight light;

// arrow
// ------------------------
const ofIndexType faces[] =
{
	// bottom
	2,1,0,
	3,5,6,
	3,4,5,

	// top
	7,8,9,
	10,12,13,
	10,11,12,

	// z minus side
	0,7,9,
	0,2,9,

	3,4,10,
	4,10,11,

	2,3,9,
	3,9,10,

	// z plus side
	0,7,8,
	0,1,8,

	1,6,8,
	6,8,13,

	12,13,5,
	5,6,13,

	// hip
	4,5,11,
	5,11,12,
};

const float verts[] =
{
	4.0f, 0.0f, 0.0f,
	2.0f, 0.0f, -2.0f,
	2.0f, 0.0f, 2.0f,
	2.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, -1.0f,
	2.0f, 0.0f, -1.0f,

	4.0f, 1.0f, 0.0f,
	2.0f, 1.0f, -2.0f,
	2.0f, 1.0f, 2.0f,
	2.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, -1.0f,
	2.0f, 1.0f, -1.0f,
};

const int kNumFaces = sizeof(faces) / sizeof(ofIndexType);
const int kNumVerts = sizeof(verts) / sizeof(float);

ofVec3f v[kNumVerts];
ofVec3f n[kNumVerts];
ofFloatColor c[kNumVerts];
ofVbo vbo;

float calcAngle2D(ofVec3f v1, ofVec3f v2)
{
	float angle = ofRadToDeg( atan((v2.z - v1.z)/(v2.x - v1.x)) );
	
	if (v2.z < v1.z && v2.x > v1.x)
		return angle;
	
	else if((v2.z < v1.z && v2.x < v1.x) || (v2.z > v1.z && v2.x < v1.x))
		return angle + 180.;
	
	else
		return angle + 360.;
}


// balance
// cite: http://goo.gl/QcylU
// ------------------------
float weightBalance[] =
{
	5,	//ramActor::JOINT_HIPS
	5,	//ramActor::JOINT_ADBOMEN // 10
	5, //ramActor::JOINT_CHEST  // 10
	5,	//ramActor::JOINT_NECK
	10, //ramActor::JOINT_HEAD
	
	5,	//ramActor::JOINT_LEFT_HIP
	4.5,//ramActor::JOINT_LEFT_KNEE
	5,	//ramActor::JOINT_LEFT_ANKLE
	1.5,//ramActor::JOINT_LEFT_TOE
	
	5, //ramActor::JOINT_RIGHT_HIP
	4.5, //ramActor::JOINT_RIGHT_KNEE
	5, //ramActor::JOINT_RIGHT_ANKLE
	1.5, //ramActor::JOINT_RIGHT_TOE
	
	4, //ramActor::JOINT_LEFT_COLLAR
	4, //ramActor::JOINT_LEFT_SHOULDER
	3, //ramActor::JOINT_LEFT_ELBOW
	2, //ramActor::JOINT_LEFT_WRIST
	1, //ramActor::JOINT_LEFT_HAND
	
	4, //ramActor::JOINT_RIGHT_COLLAR
	4, //ramActor::JOINT_RIGHT_SHOULDER
	3, //ramActor::JOINT_RIGHT_ELBOW
	2, //ramActor::JOINT_RIGHT_WRIST
	1, //ramActor::JOINT_RIGHT_HAND
};

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
	
	ofEnableAlphaBlending();
    glEnable(GL_DEPTH_TEST);
	
	int i, j = 0;
    for ( i = 0; i < kNumVerts; i++ )
    {
        c[i].r = 3*i;
        c[i].g = 3*i;
        c[i].b = 3*i;

        v[i][0] = verts[j];
        j++;
        v[i][1] = verts[j];
        j++;
        v[i][2] = verts[j];
        j++;
    }

    vbo.setVertexData( &v[0], kNumVerts, GL_STATIC_DRAW );
    vbo.setColorData( &c[0], kNumVerts, GL_STATIC_DRAW );
    vbo.setIndexData( &faces[0], kNumFaces, GL_STATIC_DRAW );
}

//--------------------------------------------------------------
void testApp::update()
{
	oscReceiver.update();
//	ramActor &actor = getActorManager().getActor(myActorName);
//	
//	balancer.update(actor);
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofBackgroundGradient( ofColor( 240 ), ofColor( 60 ) );
	
	ofPushStyle();
	
	ofVec3f avarage;
	float scale;
	int angle;
	if (getActorManager().getNumActor() > 0)
	{
		ramCameraBegin();
		glPushMatrix();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_DEPTH_TEST);
		
		ramActor &nodeArray = getActorManager().getActor(myActorName);
		ofVec3f axis = nodeArray.getNode(ramActor::JOINT_CHEST).getPosition();
		
		const int size = nodeArray.getNumNode();
		
		axis.y = 0.;
		
		for (int i=0; i<size; i++)
		{
			ramNode &node = nodeArray.getNode(i);
			
			ofVec3f pos = nodeArray.getNode(i).getPosition();
			pos.y = 0.;
			
			ofVec3f dist(pos - axis);
			dist *= weightBalance[i];
			
			avarage.x += dist.x;
			avarage.z += dist.z;
		}
		avarage.x /= size;
		avarage.z /= size;
		
		scale = sqrt(avarage.x*avarage.x+avarage.z*avarage.z);
		angle = calcAngle2D(axis, avarage);
		
		glTranslatef(axis.x, 200, axis.z);
		glScalef(scale, 10.0, scale);
		glRotatef(angle, 0.f, 1.f, 0.f);
		
		vbo.drawElements(GL_TRIANGLES, kNumFaces);
		
		glDisable(GL_DEPTH_TEST);
		glPopAttrib();
		glPopMatrix();
		ofPopStyle();
		ramCameraEnd();
	}
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
	glEnable(GL_DEPTH_TEST);
	ofPushStyle();
	ofNoFill();
    ofSetColor(20);
	
	for (int i=0; i<actor.getNumNode(); i++)
    {
		const ramNode &node = actor.getNode(i);
		
		ofBox(node, 10);
        
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




#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	switch (key)
	{
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

