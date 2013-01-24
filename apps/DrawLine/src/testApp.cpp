#include "testApp.h"


/*!
 Scenes
 */
#include "DrawLines.h"
DrawLines drawLines;


#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground( ramColor::WHITE );
	
	/*!
	 ramBaseApp setup
	 */
	ramEnableAllEvents();
	oscReceiver.setup(10000);
	
	const float lightPosition[] = { -100.0f, 500.0f, 200.0f };
	gl::calcShadowMatrix( gl::kGroundPlaneYUp, lightPosition, shadowMat.getPtr() );
	
	
	/*!
	 gui setup
	 */
	gui.setup();
	gui.loadFont("Fonts/din-webfont.ttf", 11);
	
	/* camera */
	camSettingXml.loadFile("settings.camera.xml");
	setting_cam = ramCameraSettings::getSettings(camSettingXml);
	gui.addMultiToggle("Camera Position", 0, ramCameraSettings::getCamNames(camSettingXml));
	
	
	/*!
	 scenes setup
	 */
	scenes.push_back( drawLines.getPtr() );
	
	gui.addPanel("All Scenes");
	gui.addToggle("Draw Actor", true);
	for (int i=0; i<scenes.size(); i++)
	{
		string key = scenes.at(i)->getSceneEnableKey();
		gui.addToggle(key, false);
	}
	for (int i=0; i<scenes.size(); i++)
	{
		scenes.at(i)->setup();
		scenes.at(i)->setMatrix(shadowMat);
		scenes.at(i)->refreshControlPanel(gui);
	}
	scenes.at(0)->enable();
}

//--------------------------------------------------------------
void testApp::update()
{
	
	/* Entities update */
	oscReceiver.update();
	
	
	/* Scenes update */
	for (int i=0; i<scenes.size(); i++)
		scenes.at(i)->update();
	
	
	/* GUI: camera */
	if (gui.hasValueChanged("Camera Position"))
	{
		/* camera */
		int posIndex = gui.getValueI("Camera Position");
		ofVec3f pos = setting_cam.at(posIndex).pos;
		getActiveCamera().setPosition(pos);
		getActiveCamera().lookAt(ofVec3f(0,170,0));
	}
	
	
	/* GUI: floor */
	if (gui.hasValueChanged("Background"))
	{
		float bgcolor = gui.getValueF("Background");
		ofBackground(bgcolor);
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
	ramBasicFloor(gui.getValueI("Floor pattern"),
				  gui.getValueF("Floor size"),
				  gui.getValueF("Grid size"),
				  ramColor::BLUE_LIGHT,
				  ramColor::BLUE_DEEP);
}

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	if ( gui.getValueB("Draw Actor") )
		ramBasicActor(actor, shadowMat.getPtr());
	
	for (int i=0; i<scenes.size(); i++) scenes.at(i)->drawActor(actor);
}

//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
{
	for (int i=0; i<scenes.size(); i++)
		scenes.at(i)->drawRigid(rigid);
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
