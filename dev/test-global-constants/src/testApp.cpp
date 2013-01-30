#include "testApp.h"


<<<<<<< HEAD:dev/test-global-constants/src/testApp.cpp
static const string DATAFOLDER_PATH = "../../../AllScenes/bin/data/";
=======
const string myActorName = "Ando_2012-08-29_13-48-10";
>>>>>>> 813dde751d2de68bf727a5490f2a218d1d42fc27:dev/ram_api_rewrite/src/testApp.cpp

#include "ofxBt.h"

<<<<<<< HEAD:dev/test-global-constants/src/testApp.cpp
/*!
 Scenes
 */
#include "BigBox.h"
BigBox bigbox;


#pragma mark - oF methods
=======
ramBoxPrimitive *cube;
ramSpherePrimitive *sphere;

//--------------------------------------------------------------
>>>>>>> 813dde751d2de68bf727a5490f2a218d1d42fc27:dev/ram_api_rewrite/src/testApp.cpp
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
<<<<<<< HEAD:dev/test-global-constants/src/testApp.cpp
	ofBackground( ramColor::WHITE );
	
	ramGlobal().init();
	ramGlobal().setLightPosition(ofVec3f(-100.0f, 500.0f, 200.0f));
	
	/*!
	 ramBaseApp setup
	 */
=======
	ofBackground(0);
	oscReceiver.setup(10000);

	// enable ramBaseApp::setup, update, draw, exit
>>>>>>> 813dde751d2de68bf727a5490f2a218d1d42fc27:dev/ram_api_rewrite/src/testApp.cpp
	ramEnableAllEvents();
	oscReceiver.setup(10000);
	
<<<<<<< HEAD:dev/test-global-constants/src/testApp.cpp
	/*!
	 scenes setup
	 */
	scenes.push_back( bigbox.getPtr() );
	
	ramGlobal().getGUI().addScenePanels(scenes);
=======
	cube = new ramBoxPrimitive(ofVec3f(0, 300, 0), 100);
	sphere = new ramSpherePrimitive(ofVec3f(0, 0, 0), 50);
>>>>>>> 813dde751d2de68bf727a5490f2a218d1d42fc27:dev/ram_api_rewrite/src/testApp.cpp
}

//--------------------------------------------------------------
void testApp::update()
{
	/* Entities update */
	oscReceiver.update();
<<<<<<< HEAD:dev/test-global-constants/src/testApp.cpp

	/* Scenes update */
	for (int i=0; i<scenes.size(); i++)
	{
		ramSceneBase* scene = scenes.at(i);
		
		/* Enable / Disable scenes */
		string key = scene->getSceneEnableKey();
		
		if ( ramGlobal().getGUI().hasValueChanged(key) )
		{
			scene->setEnabled( ramGlobal().getGUI().getValueB(key) );
		}
	}
	
	for (int i=0; i<scenes.size(); i++)
	{
		ramSceneBase* scene = scenes.at(i);
		if (!scene->isEnabled()) continue;
		scene->update();
	}
=======
>>>>>>> 813dde751d2de68bf727a5490f2a218d1d42fc27:dev/ram_api_rewrite/src/testApp.cpp
}

//--------------------------------------------------------------
void testApp::draw()
{
<<<<<<< HEAD:dev/test-global-constants/src/testApp.cpp
	/* Scenes draw */
	for (int i=0; i<scenes.size(); i++)
	{
		ramSceneBase* scene = scenes.at(i);
		if (!scene->isEnabled()) continue;
		scene->draw();
	}
=======
    ofEnableSmoothing();
	
	ramNodeFinder nf(ramActor::JOINT_ABDOMEN);
	
	ramCameraBegin();
    
	ofNoFill();
	
	cube->draw();
	sphere->draw();
    
//	ramPhysics::instance().debugDraw();
	
    ofDisableSmoothing();
	
	ramCameraEnd();
>>>>>>> 813dde751d2de68bf727a5490f2a218d1d42fc27:dev/ram_api_rewrite/src/testApp.cpp
}



#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawFloor()
{
<<<<<<< HEAD:dev/test-global-constants/src/testApp.cpp
	ramBasicFloor(ramGlobal().getGUI().getValueI("Floor pattern"),
				  ramGlobal().getGUI().getValueF("Floor size"),
				  ramGlobal().getGUI().getValueF("Grid size"),
				  ramColor::BLUE_LIGHT,
				  ramColor::BLUE_LIGHT-20);
}

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	if ( ramGlobal().getGUI().getValueB("Draw Actor") )
		ramBasicActor(actor);

	ramGlobal().beginShadowMatrix();
	ramBasicActor(actor, ramColor::GRAY, ramColor::GRAY);
	ramGlobal().endShadowMatrix();
	
	for (int i=0; i<scenes.size(); i++)
	{
		ramSceneBase* scene = scenes.at(i);
		if (!scene->isEnabled()) continue;
		scene->drawActor(actor);
	}
}

=======
    
}

// ram methods
//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
    for (int i=0; i<actor.getNumNode(); i++)
    {
        ramNode &node = actor.getNode(i);
        
        {
            glPushAttrib(GL_ALL_ATTRIB_BITS);
            glPushMatrix();
            ofPushStyle();

            ofSetColor(255, 0, 0);
            ofNoFill();
			
            ofSetColor(0, 255, 0);
            ramBox(node, 10);
            
            ofPopStyle();
            glPopMatrix();
            glPopAttrib();
        }
    }
}

// ram methods
>>>>>>> 813dde751d2de68bf727a5490f2a218d1d42fc27:dev/ram_api_rewrite/src/testApp.cpp
//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
{
	
}




<<<<<<< HEAD:dev/test-global-constants/src/testApp.cpp
#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	
=======
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	cube = new ramBoxPrimitive(ofVec3f(0, 300, 0), 100);
>>>>>>> 813dde751d2de68bf727a5490f2a218d1d42fc27:dev/ram_api_rewrite/src/testApp.cpp
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

