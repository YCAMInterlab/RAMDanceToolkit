#include "testApp.h"

//ramFilterEach<ramSession> sessions;

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

	
	/// session
	// ------------------
//	ramGetGUI().addPanel( &session );
}

//--------------------------------------------------------------
void testApp::update()
{
//	sessions.update(getAllNodeArrays());
	if (getNumNodeArray() > 0)
	{
		session.update(getNodeArray(0));
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
	/// draw recorded session if it's playing
//	for(int i=0; i<sessions.getNumFilters(); i++)
//	{
//		ramSession &session = sessions.getFilter(i);
//		
		if (session.isPlaying())
		{
			ramBeginCamera();
			ramActor &actor = (ramActor &)session.get();
			ramDrawBasicActor(actor);
			ramEndCamera();
		}
//	}
}



#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawActor(const ramActor &actor)
{
	/// draw realtime data
	ofSetColor(ramColor::RED_DEEP);
	ramDrawBasicActor(actor);
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
		case '[':
//			for(int i=0; i<sessions.getNumFilters(); i++)
//				sessions.getFilter(i).startRecording();
			
			session.startRecording();
			break;

		case ']':
//			for(int i=0; i<sessions.getNumFilters(); i++)
//				sessions.getFilter(i).stopRecording();
			
			session.stopRecording();
			break;

		case '-':
//			for(int i=0; i<sessions.getNumFilters(); i++)
//				sessions.getFilter(i).play();
			
			session.play();
			break;
			
		case '=':
//			for(int i=0; i<sessions.getNumFilters(); i++)
//				sessions.getFilter(i).stop();
			
			session.stop();
			break;
			
		case 's':
//			for(int i=0; i<sessions.getNumFilters(); i++)
//				coder.save(sessions.getFilter(i), ofToString(i)+".tsv");
			
			coder.save(session);
			break;
			
		case 'l':
			session = coder.load("tst.tsv");
			
//			cout << "Duration: " << session.getDuration() << "sec"<< endl;
//			cout << "Frames: " << session.getNumFrames() << endl;
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

