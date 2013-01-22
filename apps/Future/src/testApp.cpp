#include "testApp.h"


static const string DATAFOLDER_PATH = "../../../AllScenes/bin/data/";



/*!
 Scenes
 */
#include "Future.h"
Future future;


#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground( getRamColor(ramColor::WHITE)-20 );
	
	
	/*!
	 ramBaseApp setup
	 */
	ramEnableAllEvents();
	oscReceiver.setup(10000);
	
	const float lightPosition[] = { -100.0f, 500.0f, 200.0f };
	gl::calcShadowMatrix( gl::kGroundPlaneYUp, lightPosition, shadowMat.getPtr() );
	
	
	/*!
	 GUI setup
	 */
	gui.setup();
	gui.loadFont(DATAFOLDER_PATH + "Fonts/din-webfont.ttf", 11);
	
	/* camera */
	setting_cam = ramCameraSettings::getSettings(camSettingXml);
	gui.addMultiToggle("Camera Position", 0, ramCameraSettings::getCamNames(camSettingXml));
	
	
	/*!
	 scenes setup
	 */
	scenes.push_back( future.getPtr() );
	
	/* adding GUI Panel for each scene */
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
		scenes.at(i)->refreshControlPanel(gui);
		scenes.at(i)->setMatrix(shadowMat);
	}
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
		cout << 1 << endl;
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
				  getRamColor(ramColor::BLUE_LIGHT),
				  getRamColor(ramColor::BLUE_LIGHT)-20);
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

