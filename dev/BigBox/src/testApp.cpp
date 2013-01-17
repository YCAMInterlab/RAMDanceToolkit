#include "testApp.h"


#include <numeric>
ofMatrix4x4 shadowMat;

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
	
	const float lightPosition[] = { -100.0f, 500.0f, 200.0f };
    gl::calcShadowMatrix( gl::kGroundPlaneYUp, lightPosition, shadowMat.getPtr() );
	
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
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofBackgroundGradient( ofColor( 240 ), ofColor( 60 ) );
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
	ofColor currSklColor(110, 20, 20);
	ofColor recSklColor(20, 20, 110);
	ofColor shadowColor(0, 30);
	float boxSize1 = 500.0;
	float boxSize2 = 5.0;
	
	
	for (int i=0; i<actor.getNumNode(); i++)
	{
		const ramNode &node = actor.getNode(i);
		
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushMatrix();
		ofPushStyle();
		ofNoFill();
		
		glEnable(GL_DEPTH_TEST);
		
		// big box
		node.transformBegin();
		ofSetColor(recSklColor);
		ofBox(node, boxSize1);
		node.transformEnd();
		
		// actor
		ofSetColor(currSklColor);
		ofBox(node, boxSize2);
		if (node.hasParent())
			ofLine(node, *node.getParent());

		
		// shadows
		glDisable(GL_DEPTH_TEST);
		glMultMatrixf(shadowMat.getPtr());
		ofEnableAlphaBlending();
		ofSetColor(shadowColor);
		
		node.transformBegin();
		ofBox(node, boxSize1);
		node.transformEnd();

		ofBox(node, boxSize2);
		if (node.hasParent())
			ofLine(node, *node.getParent());
		
		ofPopStyle();
		glPopMatrix();
		glPopAttrib();
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

