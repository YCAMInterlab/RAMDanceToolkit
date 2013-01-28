#include "testApp.h"


static const string DATAFOLDER_PATH = "../../../AllScenes/bin/data/";


/*!
 Scenes
 */
#include "BigBox.h"
BigBox bigbox;


#pragma mark - oF methods
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground( ramColor::WHITE );
	
	ramGlobal().init();
	ramGlobal().setLightPosition(ofVec3f(-100.0f, 500.0f, 200.0f));
	
	/*!
	 ramBaseApp setup
	 */
	ramEnableAllEvents();
	oscReceiver.setup(10000);
	
	/*!
	 scenes setup
	 */
	scenes.push_back( bigbox.getPtr() );
	
	ramGlobal().getGUI().addScenePanels(scenes);
	
//	for (int i=0; i<scenes.size(); i++)
//		scenes.at(i)->setMatrix(shadowMat);
}

//--------------------------------------------------------------
void testApp::update()
{
	/* Entities update */
	oscReceiver.update();

	/* Scenes update */
	for (int i=0; i<scenes.size(); i++)
	{
		ramSceneBase* scene = scenes.at(i);
		scene->update();
		
		/* Enable / Disable scenes */
		string key = scene->getSceneEnableKey();
		
		if ( ramGlobal().getGUI().hasValueChanged(key) )
		{
			scene->setEnabled( ramGlobal().getGUI().getValueB(key) );
		}
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
	/* Scenes draw */
	for (int i=0; i<scenes.size(); i++)
		scenes.at(i)->draw();
}



#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawFloor()
{
//	ramBasicFloor(ramGlobal().getGUI().getValueI("Floor pattern"),
//				  ramGlobal().getGUI().getValueF("Floor size"),
//				  ramGlobal().getGUI().getValueF("Grid size"),
//				  ramColor::BLUE_LIGHT,
//				  ramColor::BLUE_LIGHT-20);
}

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
//	if ( ramGlobal().getGUI().getValueB("Draw Actor") )
//		ramBasicActor(actor, shadowMat.getPtr());

	ramBasicActor(actor, NULL);
	
	glPushMatrix();
	glMultMatrixf(shadowMat.getPtr());
	ramBasicActor(actor, NULL);
	glPopMatrix();
	
	for (int i=0; i<scenes.size(); i++) scenes.at(i)->drawActor(actor);
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

