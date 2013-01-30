#include "testApp.h"


#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(ramColor::WHITE);
	
	
	/// ram setup
	// ------------------
	ramInit();
	oscReceiver.setup(10000);
	
	
	/// scenes setup
	// ------------------
	vector<ramSceneBase*> scenes;
	sceneManager.setup(scenes);	
}

//--------------------------------------------------------------
void testApp::update()
{
	/* Entities update */
	oscReceiver.update();
	sceneManager.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	
}



#pragma mark - ram methods

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	ramBasicActor(actor, NULL);
	
	for (int i=0; i<actor.getNumNode(); i++)
	{
		ramNode &node = actor.getNode(i);
		ofVec3f v;
		
		
		v = node.getPosition();
		
		{
//			if(i==0) cout << node.getPosition() << endl;
			ofBox(v, 10);
		}
		{
			
			float *dst = ofMatrix4x4().getPtr();
			GLfloat src1[16];
			glGetFloatv(GL_MODELVIEW_MATRIX, src1);
			float *src2 = node.ofNode::getGlobalTransformMatrix().getPtr();
			
			
			for(int y=0;y<4;y++)
			{
				for(int x=0;x<4;x++)
				{
					
					dst[y*4+x] = src2[y*4]   * src1[x]   +
								 src2[y*4+1] * src1[x+4] +
								 src2[y*4+2] * src1[x+8] +
								 src2[y*4+3] * src1[x+12];
				}
			}
			
			if(i==0)
			{
				cout << node.getPosition() << endl;
				cout << dst[1] << " " << dst[5] << " "<< dst[9] << " "<< dst[13] << " " << endl;
				cout << dst[2] << " " << dst[6] << " "<< dst[10] << " "<< dst[14] << " " << endl;
				cout << dst[3] << " " << dst[7] << " "<< dst[11] << " "<< dst[15] << " " << endl;
				cout << dst[4] << " " << dst[8] << " "<< dst[12] << " "<< dst[16] << " " << endl << endl;
			}
			
//			node.transformBegin();
//			ofBox(10);
//			node.transformEnd();
//			ofPushMatrix();
//			ofMultMatrix( dst );
//			ofBox(v*gui.getValueF("test"), 10);
//			ofPopMatrix();
		}
		ofPopMatrix();
		
	}
	cout << endl << "---" << endl;
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

