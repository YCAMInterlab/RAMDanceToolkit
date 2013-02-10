#include "testApp.h"
#include "ofxBt.h"


static const string DATAFOLDER_PATH = "../../../AllScenes/bin/data/";


/*!
 Scenes
 */
#include "BigBox.h"
BigBox bigbox;


#pragma mark - oF methods
ramBoxPrimitive *cube;
ramSpherePrimitive *sphere;

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(ramColor::WHITE);
	
	ramGlobal().init();
	ramGlobal().setLightPosition(ofVec3f(-100.0f, 500.0f, 200.0f));
	
	/*!
	 ramBaseApp setup
	 */

	// enable ramBaseApp::setup, update, draw, exit
	ramEnableAllEvents();
	oscReceiver.setup(10000);
	
	/*!
	 scenes setup
	 */
	scenes.push_back( bigbox.getPtr() );
	ramGlobal().getGUI().addScenePanels(scenes);
	cube = new ramBoxPrimitive(ofVec3f(0, 300, 0), 100);
	sphere = new ramSpherePrimitive(ofVec3f(0, 0, 0), 50);
}

//--------------------------------------------------------------
void testApp::update()
{
	/// Entities update
	// ------------------
	oscReceiver.update();

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
}

//--------------------------------------------------------------
void testApp::draw()
{
	/* Scenes draw */
	for (int i=0; i<scenes.size(); i++)
	{
		ramSceneBase* scene = scenes.at(i);
		if (!scene->isEnabled()) continue;
		scene->draw();
	}
   ofEnableSmoothing();
	
	ramNodeFinder nf(ramActor::JOINT_ABDOMEN);
	
	ramBeginCamera();
    
	ofNoFill();
	
	cube->draw();
	sphere->draw();
    
//	ramPhysics::instance().debugDraw();
	
    ofDisableSmoothing();
	
	ramEndCamera();
}



#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawFloor()
{
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

// ram methods
//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
{
	
}

#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	cube = new ramBoxPrimitive(ofVec3f(0, 300, 0), 100);
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

