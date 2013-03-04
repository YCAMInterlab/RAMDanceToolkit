#include "ramSession.h"

#include "ramControlPanel.h"

void ramSession::setLoop(const bool l) { mLoop = l; };
void ramSession::setRate(const float r) { mRate = r; };
void ramSession::setPlayhead(const float t) { mPlayhead = t; };

const bool ramSession::isPlaying() const { return mPlaying; }
const bool ramSession::isRecording() const { return mRecording; }
const bool ramSession::isLoop() const { return mLoop; }

#pragma mark -
#pragma mark constructor

ramSession::ramSession() { clear(); }
ramSession::ramSession(const ramSession &copy) { clear(); *this = copy; }
ramSession::ramSession(const ramNodeArrayBuffer &buf) { clear(); mBuffer = buf; }

#pragma mark -
#pragma mark gui settings

void ramSession::setupControlPanel()
{

#ifdef RAM_GUI_SYSTEM_OFXUI

	ofxUICanvas* panel = ramGetGUI().getCurrentUIContext();

	panel->addToggle("Rec", false, 40, 40);
	panel->addToggle("Play", false, 40, 40);
	panel->addToggle("Loop", true, 40, 40);

	ofAddListener(panel->newGUIEvent, this, &ramSession::onPanelChanged);

#endif
}

void ramSession::onPanelChanged(ofxUIEventArgs& e)
{
	string name = e.widget->getName();
	
	if (name == "Rec")
	{
		ofxUIToggle *toggle = (ofxUIToggle*)e.widget;
		
		if (toggle->getValue())
			startRecording();
		
		else
			stopRecording();
	}
	
	if (name == "Play")
	{
		ofxUIToggle *toggle = (ofxUIToggle*)e.widget;
		
		if (toggle->getValue())
			play();
		
		else
			stop();
	}
	
	if (name == "Loop")
	{
		ofxUIToggle *toggle = (ofxUIToggle*)e.widget;
		setLoop(toggle->getValue());
	}
}



#pragma mark -
#pragma mark override ramBaseFilter

const ramNodeArray& ramSession::filter(const ramNodeArray &src)
{
	if (isRecording())
	{
		appendFrame(src);
	}
	
	if (isPlaying())
	{
		updatePlayhead();
		return mBuffer.get(getFrameIndex());
	}
	
	return src;
}

const string ramSession::getName()
{
	return "ramSession";
}



#pragma mark -
#pragma mark session handling

void ramSession::startRecording()
{
	if (isRecording()) return;

	clear();
	mPlaying = false;
	mRecording = true;
	mRecStartTime = ofGetElapsedTimef();
	
	cout << "recording start." << endl;
}

void ramSession::stopRecording()
{
	if (!isRecording() || mBuffer.getSize() <= 0) return;
	
	mRecording = false;
	mRecEndTime = ofGetElapsedTimef();
	
	cout << "Recording finished." << endl;
	cout << "Actor: " << getNodeArrayName() << endl;
	cout << "Duration: " << getDuration() << " sec"<< endl;
	cout << "Frames: " << getNumFrames() << endl;
}

void ramSession::play()
{
	if (getNumFrames() <= 0) return;

	mRecording = false;
	mPlaying = true;
	mPlayStartTime = ofGetElapsedTimef();
	
	cout << "start playing " << getNodeArrayName() << "." << endl;
}

void ramSession::stop()
{
	if (!isPlaying()) return;

	mPlaying = false;
	
	cout << "stop playing " << getNodeArrayName() << "." << endl;
}


// --


void ramSession::updatePlayhead()
{
	if (!isPlaying()) return;
	
	mPlayhead = (ofGetElapsedTimef() - mPlayStartTime) * mRate;
	
	if (getFrameIndex() <= 0)
	{
		if (isLoop())
		{
			mPlayStartTime = ofGetElapsedTimef();
		}
		else
		{
			stop();
		}
	}
}


// --

void ramSession::clear()
{
	mBuffer.clear();
	
	mPlayhead = 0;
	mPlayStartTime = 0;
	mRecStartTime = 0;
	mRecEndTime = 0;
	
	mRate = 1.0;
	mLoop = true;
	mRecording = false;
	mPlaying = false;
}

void ramSession::appendFrame(const ramNodeArray copy)
{
	mBuffer.add(copy);
}

const ramNodeArray& ramSession::getFrame(int index) const
{
	if (index > getNumFrames()) index = getNumFrames();
	return mBuffer.get(getNumFrames() - index);
}

const ramNodeArray& ramSession::getCurrentFrame() const
{
	return mBuffer.get(getFrameIndex());
}



#pragma mark -
#pragma mark getters, setters

const float ramSession::getPlayhead() const
{
	return mPlayhead;
}

const int ramSession::getFrameIndex() const
{
	return getNumFrames() - floor(mPlayhead / getFrameTime());
}

const int ramSession::getNumFrames() const
{
	return mBuffer.getSize();
}

const float ramSession::getFrameTime() const
{
	return getDuration() / getNumFrames();
}

const float ramSession::getDuration() const
{
	assert(getNumFrames() > 0);
	
	const ramNodeArray &frontFrame = mBuffer.get( 0 );
	const ramNodeArray &backFrame = mBuffer.get( getNumFrames() );
	
	return frontFrame.getTimestamp() - backFrame.getTimestamp();
}

const string ramSession::getNodeArrayName() const
{
	assert(getNumFrames() > 0);
	
	return getNumFrames() > 0 ? mBuffer.get(0).getName() : "no name";
}

void ramSession::setNodeArrayBuffer(ramNodeArrayBuffer &buffer)
{
	if (buffer.getSize() <= 0)
	{
		ofLogError("RAMDanceToolkit::ramSession") << "invalid buffer was passed to setNodeArrayBuffer";
		return;
	}
	
	clear();
	mBuffer = buffer;
}
