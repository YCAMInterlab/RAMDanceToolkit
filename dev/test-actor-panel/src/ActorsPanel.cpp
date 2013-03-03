#include "ActorsPanel.h"


#pragma mark -
#pragma mark constructor

ActorsPanel::ActorsPanel() {}



#pragma mark -
#pragma mark called from ramSceneManager

void ActorsPanel::setupControlPanel()
{
	mLocalPanel = gui().getCurrentUIContext();
}

void ActorsPanel::setup()
{
	fontSize = 30;
	font.loadFont(ramToResourcePath("Fonts/NewMedia Fett.ttf"), fontSize, true, true);
	font.setLineHeight(fontSize*1.4f);
	font.setLetterSpacing(1.0);
	
	
}

void ActorsPanel::update()
{
	if (mNeedUpdatePanel)
	{
		rebuildControlPanel();
	}
	
	SegmentsIter it = mSegmentsMap.begin();
	
	while (it != mSegmentsMap.end())
	{
		ControlSegment *seg = it->second;
		
		/// position reset
		if (seg->bNeedsResetPos)
		{
			seg->position = ofPoint::zero();
			seg->bNeedsResetPos = false;
		}
		
		/// recording
		if (seg->session.isRecording())
		{
			seg->session.update( getNodeArray(it->first) );
		}
		
		it++;
	}
}

void ActorsPanel::draw()
{
	bRecording = false;
	
	for(int i=0; i<getNumNodeArray(); i++)
	{
		ramNodeArray &NA = getNodeArray(i);
		
		const string name = NA.getName();
		
		SegmentsIter it = mSegmentsMap.find(name);
		
		assert(it != mSegmentsMap.end());
		
		ControlSegment *seg = it->second;
		
		/// draw if "Show actor" toggle is anabled
		// note that ofxUIImageToggle shows hilighted image when it's false,
		ramBeginCamera();
		
		if (!seg->bHideActor)
		{
			ofPushMatrix();
			ofPushStyle();
			{
				ofSetColor(seg->jointColor);
				ofTranslate(seg->position.x, 0, seg->position.y);
				ramDrawNodes(NA);
			}
			ofPopStyle();
			ofPopMatrix();
		}
		
		ramEndCamera();
		
		/// show/hide recording indicator
		if (seg->bRecording)
		{
			bRecording = true;
		}
	}
}

void ActorsPanel::drawHUD()
{
	if (bRecording)
	{
		ofPushStyle();
		{
			ofSetColor(255, 0, 0);
			font.drawString("RECORDING", ofGetWidth()-230, fontSize*1.3);
		}
		ofPopStyle();
	}
}

const string ActorsPanel::getName()
{
	return "ActorsPanel";
}



#pragma mark -
#pragma mark Events fired from ActorManager
	
void ActorsPanel::onActorSetup(const ramActor &actor)
{
	addControlSegment(actor);
}

void ActorsPanel::onRigidSetup(const ramRigidBody &rigid)
{
	addControlSegment(rigid);
}

void ActorsPanel::onActorExit(const ramActor &actor)
{
	removeControlSegment(actor);
}

void ActorsPanel::onRigidExit(const ramRigidBody &rigid)
{
	removeControlSegment(rigid);
}



#pragma mark -
#pragma mark Events fired from SceneManager

void ActorsPanel::onEnabled()
{
	if (getNumNodeArray() != mSegmentsMap.size())
		rebuildControlPanel();
}
	



#pragma mark -
#pragma mark private methods

void ActorsPanel::addControlSegment(const ramNodeArray &NA)
{
	const string name = NA.getName();
	
	if (mSegmentsMap.find(name) != mSegmentsMap.end()) return;
	
	ControlSegment *seg = new ControlSegment();
	mSegmentsMap.insert( make_pair(name, seg) );
	
	ofxUICanvasPlus* childPanel = seg->setup(mLocalPanel, NA);
	childPanel->getRect()->y = (mSegmentsMap.size()-1) * 210;
	
	mLocalPanel->autoSizeToFitWidgets();
}

void ActorsPanel::removeControlSegment(const ramNodeArray &NA)
{
	const string key = NA.getName();
	
	if (mSegmentsMap.find(key) == mSegmentsMap.end()) return;
	
	mNeedUpdatePanel = true;
}

void ActorsPanel::rebuildControlPanel()
{
	/// remove all widgets
	mLocalPanel->removeWidgets();
	mLocalPanel->resetPlacer();
	mSegmentsMap.clear();
	
	
	/// insert panels
	for(int i=0; i<getNumNodeArray(); i++)
	{
		const ramNodeArray &NA = getNodeArray(i);
		addControlSegment(NA);
	}
	
	mLocalPanel->autoSizeToFitWidgets();
	
	mNeedUpdatePanel = false;
}



