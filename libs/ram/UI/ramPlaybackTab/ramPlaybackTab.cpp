#include "ramPlaybackTab.h"
#include "ramControlPanel.h"



#pragma mark -
#pragma mark Constructor
ramPlaybackTab::ramPlaybackTab() : ofxUITab("Playback", false)
{
	ofAddListener(ofEvents().fileDragEvent, this, &ramPlaybackTab::onFileDrop);
	ofAddListener(ofEvents().draw, this, &ramPlaybackTab::drawSessions);
}

ramPlaybackTab::~ramPlaybackTab()
{
	ofRemoveListener(ofEvents().fileDragEvent, this, &ramPlaybackTab::onFileDrop);
	ofRemoveListener(ofEvents().draw, this, &ramPlaybackTab::drawSessions);
}



#pragma mark -
#pragma mark update and draw
void ramPlaybackTab::update()
{
	SessionsIter it = mSessionsMap.begin();
	
	while (it != mSessionsMap.end())
	{
		ramSession &session = it->second;
		if (session.isPlaying())
		{
			session.updatePlayhead();
		}
		
		it++;
	}
}

void ramPlaybackTab::drawSessions(ofEventArgs &e)
{
	SessionsIter it = mSessionsMap.begin();
	
	ramBeginCamera();
	
	while (it != mSessionsMap.end())
	{
		ramSession &session = it->second;
		if (session.isPlaying())
		{
			const ramNodeArray &NA = session.getCurrentFrame();
			ramDrawNodes(NA);
		}
		
		it++;
	}
	
	ramEndCamera();
}



#pragma mark -
#pragma mark Events
void ramPlaybackTab::onFileDrop(ofDragInfo &e)
{
	for(int i=0; i<e.files.size(); i++)
	{
		const string filePath = e.files.at(i);
		
		try
		{
			ramSession session = coder.load(filePath);
			addControlSegment(session);
			session.play();
			
			mSessionsMap.insert( make_pair(session.getNodeArrayName(), session) );
		}
		catch (std::exception &e)
		{
			
			cout << e.what() << endl;
			
		}
	}
}



#pragma mark -
#pragma mark Private methods
void ramPlaybackTab::addControlSegment(const ramSession &session)
{
	const string name = session.getNodeArrayName();
//	const SegmentsIter it = mSegmentMap.find(name);

//	assert( it == mSegmentMap.end() );
	
	
	/// create control segment date internally
	PlaybackSegment *seg = new PlaybackSegment();
	mSegmentMap.insert( make_pair(name, seg) );

	
	/// create and add child panel
	const int panelIndex = mSegmentMap.size()-1;
	const int panelHeight = 210;
	const int panelHeaderHeight = 155;

	ofxUICanvasPlus* childPanel = seg->createPanel(session);
	childPanel->getRect()->y = panelIndex * panelHeight + panelHeaderHeight;

	
	/// append widget, resize parent panel, load default settings
	addWidget(childPanel);
	autoSizeToFitWidgets();
//
	
//	seg->loadCache();
}

void ramPlaybackTab::removeControlSegment(const ramSession &session)
{
//	const string name = NA.getName();
//	const SegmentsIter it = mSegmentMap.find(name);
//	
//	assert( it != mSegmentMap.end() );
//	
//	mNeedUpdatePanel = true;
}

void ramPlaybackTab::rebuildControlPanel()
{
	/// remove all widgets
	removeWidgets();
	resetPlacer();
	mSegmentMap.clear();
	
	
	/// adding panel header
	createPanelHeader();
	
	
//	/// insert panels
//	for(int i=0; i<getNumNodeArray(); i++)
//	{
//		const ramNodeArray &NA = getNodeArray(i);
//		addControlSegment(NA);
//	}
	
	autoSizeToFitWidgets();
	
	mNeedUpdatePanel = false;
}

void ramPlaybackTab::createPanelHeader()
{
	
	const int width = gui().kLength/2 - 5;
	const int height = gui().kDim * 1.3;
	
//	addLabel(getName(), OFX_UI_FONT_LARGE);
//	addSpacer(gui().kLength, 2);

	
	/// 2x2 matrix
//	addWidgetDown( new ofxUILabelToggle("Play All", &bPlayAll, width, height) );
//	addWidgetDown( new ofxUILabelButton("Cue All", false, width, height) );
//	addWidgetDown( new ofxUILabelButton("Delete All", false, width, height) );

	
//	/// buttons which are controlled programatically
//	//  all of the child widgets of mLocalPanel are deleted when rebuildControlPanel is executed
//	//  so it needs to make new pointer
//	btnPause = new ofxUILabelToggle("Pause (Space key)", false, gui().kLength, height);
//	btnRecAll = new ofxUILabelToggle("Recording All Actors", false, gui().kLength, height);
//	addWidgetDown( btnPause );
//	addWidgetDown( btnRecAll );
}


