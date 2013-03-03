#include "testApp.h"

// encode ramSession to tsv, decode tsv to ramSession
ramTSVCoder coder;
ramSession session;

// managing sessions dynamically
ramFilterEach<ramSession> sessions;


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
	/// update sessions with all node arrays
	// ------------------
	sessions.update(getAllNodeArrays());
	
	
	/// update only playhead, it is used for session which is aimed at load tsv and play
	// ------------------
	session.updatePlayhead();
}

//--------------------------------------------------------------
void testApp::draw()
{
	
	/// draw recorded session if it's playing
	// ------------------
	for(int i=0; i<sessions.getNumFilters(); i++)
	{
		ramSession &sess = sessions.getFilter(i);
		
		if (sess.isPlaying())
		{
			ramBeginCamera();
			ramActor &actor = (ramActor &)sess.get();
			ramDrawBasicActor(actor);
			ramEndCamera();
		}
	}
	
	
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
			for(int i=0; i<sessions.getNumFilters(); i++)
				sessions.getFilter(i).startRecording();
			break;

		case ']':
			for(int i=0; i<sessions.getNumFilters(); i++)
				sessions.getFilter(i).stopRecording();
			break;

		case '-':
			session.play();
			for(int i=0; i<sessions.getNumFilters(); i++)
				sessions.getFilter(i).play();
			break;
			
		case '=':
			for(int i=0; i<sessions.getNumFilters(); i++)
				sessions.getFilter(i).stop();
			break;
			
		case 's':
			for(int i=0; i<sessions.getNumFilters(); i++)
				coder.save(sessions.getFilter(i), "session-"+ofToString(i+1)+".tsv");
//				coder.save(sessions.getFilter(i));
			break;
			
		case 'l':
			session = coder.load(ramToResourcePath("MotionData/Cyril.tsv"));
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

