#include "testApp.h"


static const string myActorName = "Ando_2012-09-01_18-49-10";


#include "ofxAutoControlPanel.h"
ofxAutoControlPanel gui;
vector<ramSceneBase*> scenes;


#include "BigBox.h"
string
kPoints = "points",
kSpeed = "speed",
kReconstruct = "reconstruct";
int points;

vector<Vector> vertices;
set<Triangle> triangles;
ofMatrix4x4 shadowMat;




void refreshVertices()
{
	vertices.clear();
	vertices.resize(points);
	
	for(int i = 0; i < points; ++i)
	{
		Vector v;
		v.x = ofRandom(-gui.getValueF("x"), gui.getValueF("x"));
		v.y = ofRandom(-gui.getValueF("y"), gui.getValueF("y"));
		v.z = ofRandom(-gui.getValueF("z"), gui.getValueF("z"));
		
		float speed = gui.getValueF(kSpeed);
		v.speed = ofRandom(-speed, speed);
		
		v.color.setHsb( ofRandom(255), ofRandom(255), 125);
		vertices.at(i) = v;
	}
	
	Delaunay3d::getDelaunayTriangles(vertices, &triangles);
}



#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(255);
	oscReceiver.setup(10000);

	// enable ramBaseApp::setup, update, draw, exit
	ramEnableAllEvents();
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	
	
	// gui setup
	ofxControlPanel::setTextColor(simpleColor(255,0,0,100));
	ofxControlPanel::setBackgroundColor(simpleColor(0,0,0,20));
	gui.setup(ofGetWidth()-100, ofGetHeight());
	
	gui.addPanel("Delaunay");
	gui.addToggle(kReconstruct);
	gui.addSlider(kPoints, 4, 4, 20);
	gui.addSlider(kSpeed, 2, -10, 10);
	gui.addSlider("x", 300, 10, 1000);
	gui.addSlider("y", 300, 10, 1000);
	gui.addSlider("z", 300, 10, 1000);
	
	
	const float lightPosition[] = { -100.0f, 500.0f, 200.0f };
	gl::calcShadowMatrix( gl::kGroundPlaneYUp, lightPosition, shadowMat.getPtr() );
	ramCameraManager::instance().getActiveCamera().setPosition(0, 300, 500);
	ramCameraManager::instance().getActiveCamera().lookAt(ofVec3f(0,0,0));
	
	
	// triangulation...
	// ---------------
	points = 4;
	refreshVertices();
}

//--------------------------------------------------------------
void testApp::update()
{
	oscReceiver.update();
	
	if ( gui.hasValueChanged(kReconstruct) )
		refreshVertices();
	
	if ( gui.hasValueChanged(kPoints) )
		points = gui.getValueI(kPoints);
	
	gui.clearAllChanged();
	
	
	ramActor &actor = getActor(myActorName);
	
	for (int i=0; i<vertices.size(); i++)
	{
		if (i<1)
		{
			ofVec3f v = actor.getNode(ramActor::JOINT_CHEST).getPosition();
			vertices.at(i).x = v.x;
			vertices.at(i).y = v.y;
			vertices.at(i).z = v.z;
		}
		else
		{
			float delta = ofGetElapsedTimef();
			
			Vector& v = vertices.at(i);
			v.x = cos(delta * v.speed) * gui.getValueF("x");
			v.z = sin(delta * v.speed) * gui.getValueF("z");
		}
	}
	
}

//--------------------------------------------------------------
void testApp::draw()
{
	ramCameraBegin();
	
	int i=0;
	for(std::set<Triangle>::iterator it = triangles.begin(); it != triangles.end(); ++it)
    {
		
		Triangle t = *it;
		
		for(int i = 0; i < 3; ++i)
		{
			ofVec3f v1, v2;
			v1 = ofVec3f(t.p[i]->x,
						 t.p[i]->y,
						 t.p[i]->z);
			
			v2 = ofVec3f(t.p[(i+1)%3]->x,
						 t.p[(i+1)%3]->y,
						 t.p[(i+1)%3]->z);
			
			ofSetColor(t.p[i]->color);
			
			if(v1.distance(v2) < 2000)
			{
				
				glEnable(GL_DEPTH_TEST);
				{
					ofLine(v1, v2);
					ofSetColor(0);
					ofBox(v1, 2);
				}
				glDisable(GL_DEPTH_TEST);
				
//				glPushMatrix();
//				{
//					glMultMatrixf(shadowMat.getPtr());
//					ofEnableAlphaBlending();
//					ofSetColor(ofColor(100,40));
//					ofLine(v1, v2);
//				}
//				glPopMatrix();
			}
		}
		
		i++;
    }
	
	ramCameraEnd();
}



#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawFloor()
{
	ramBasicFloor(600., 50., ofColor(255, 50), ofColor(200, 50));
}

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	
	glEnable(GL_DEPTH_TEST);
	for (int i=0; i<actor.getNumNode(); i++)
	{
		ramNode &node = actor.getNode(i);
		
		ofSetHexColor(0xa7f8a9);
		node.transformBegin();
		ofBox( i==(ramActor::JOINT_HEAD) ? 6 : 3 );
		node.transformEnd();
		
		if (node.hasParent())
		{
			ofSetHexColor(0xffdede);
			ofLine(node, *node.getParent());
		}
	}
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


