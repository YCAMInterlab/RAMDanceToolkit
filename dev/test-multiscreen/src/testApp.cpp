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
	
	/// Scene Manager setup
	// ------------------
	vector<ramBaseScene*> scenes;
	scenes.push_back( scene1.getPtr() );
	scenes.push_back( scene2.getPtr() );
	sceneManager.setup(scenes);
	
	for (int i = 0; i < 5; i++)
	{
		ofEasyCam *cam = ramCameraManager::instance().createCamera<ofEasyCam>();
		cam->disableMouseInput();
	}
	
	for (int i = 0; i < 5; i++)
	{
		ramCameraManager::instance().setActiveCamera(i + 1);
		ramCameraManager::instance().rollbackDefaultCameraSetting(i);
	}
	
	ramCameraManager::instance().setActiveCamera(0);
}

//--------------------------------------------------------------
void testApp::update()
{
	/// Entities update
	// ------------------
	oscReceiver.update();
	
	
	/// Scene Manager update
	// ------------------
	sceneManager.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	/// Scene Manager draw
	// ------------------
	
	setDrawFloorAuto(true);
	
	int main_screen_width = 1920;
	int main_screen_height = 1200;
	
	main_screen_width = min(main_screen_width, ofGetWidth());
	main_screen_height = min(main_screen_height, ofGetHeight());
	
	ramCameraManager::instance().setActiveCamera(0);
	
	ofPushView();
	ofViewport(0, 0, 1920, 1200);
	
	ramBeginCamera();
	drawFloor();
	ramEndCamera();
	
	sceneManager.draw();
	ofPopView();
	
	int screen_w = 1280, screen_h = 720;
	
	int inv_screen_height = ofGetHeight() - screen_h;
	
	for (int i = 0; i < 5; i++)
	{
		ofPushView();
		
		ofCamera *screen_camera = ramCameraManager::instance().getCamera(i + 1);
		
		ofViewport(ofRectangle(main_screen_width + i * screen_w, inv_screen_height, screen_w, screen_w));
		ramCameraManager::instance().setActiveCamera(i + 1);
		
		screen_camera->begin();
		drawFloor();
		screen_camera->end();
		
		sceneManager.draw();
		
		ofPopView();
	}

	ramCameraManager::instance().setActiveCamera(0);
	
	setDrawFloorAuto(false);
}



#pragma mark - ram methods

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	
}

//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
{
	
}




#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	if (key == 'f')
	{
		ofToggleFullscreen();
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

