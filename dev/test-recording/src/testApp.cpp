#include "testApp.h"

// encode ramSession to tsv, decode tsv to ramSession
ramTSVCoder coder;
ramSession session;


#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(ramColor::WHITE);

	/// ram setup
	// ------------------
	ramInitialize(10000);
}

//--------------------------------------------------------------
void testApp::update()
{
	/// update only playhead, it is used for session which is aimed at load tsv and play
	// ------------------
	session.updatePlayhead();
}

//--------------------------------------------------------------
void testApp::draw()
{
	/// draw loaded session
	// ------------------
	if (session.isPlaying())
	{
		ramBeginCamera();
		ramActor &actor = (ramActor &)session.getCurrentFrame();
		ramDrawBasicActor(actor);
		ramEndCamera();
	}
}



#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawActor(const ramActor &actor)
{
	
}

//--------------------------------------------------------------
void testApp::drawRigid(const ramRigidBody &rigid)
{

}




#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	switch (key)
	{
		case '.':
			session.play();
			break;
			
		case '/':
			session.stop();
			break;
			
		case 'l':
			try
			{
				coder.load("test.tsv");
				session = coder.getSession();
				cout << "loaded from " << __FUNCTION__ << endl;
			}
			catch (std::exception &e)
			{
				cout << e.what() << endl;
			}
			
			break;

		default:
			break;
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

