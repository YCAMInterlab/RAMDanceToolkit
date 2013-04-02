//#include "ramPlaybackTab.h"
//#include "ramControlPanel.h"
//
//
//
//#pragma mark -
//#pragma mark Constructor
//ramPlaybackTab::ramPlaybackTab() : ofxUITab("Playback", false)
//{
//	ofAddListener(this->newGUIEvent, this, &ramPlaybackTab::onValueChanged);
//	ofAddListener(ofEvents().fileDragEvent, this, &ramPlaybackTab::onFileDrop);
//	ofAddListener(ofEvents().setup, this, &ramPlaybackTab::setup);
//	ofAddListener(ofEvents().draw, this, &ramPlaybackTab::drawSessions);
//
//}
//
//ramPlaybackTab::~ramPlaybackTab()
//{
//	ofRemoveListener(this->newGUIEvent, this, &ramPlaybackTab::onValueChanged);
//	ofRemoveListener(ofEvents().fileDragEvent, this, &ramPlaybackTab::onFileDrop);
//	ofRemoveListener(ofEvents().setup, this, &ramPlaybackTab::setup);
//	ofRemoveListener(ofEvents().draw, this, &ramPlaybackTab::drawSessions);
//}
//
//
//
//#pragma mark -
//#pragma mark update and draw
//void ramPlaybackTab::setup(ofEventArgs &e)
//{
//	removeWidgets();
//	resetPlacer();
//	createPanelHeader();
//	autoSizeToFitWidgets();
//}
//
//void ramPlaybackTab::update()
//{
//	SegmentsIter it = mSegmentMap.begin();
//	
//	while (it != mSegmentMap.end())
//	{
//		PlaybackSegment *seg = it->second;
//		seg->update();
//		
//		it++;
//	}
//	
//	if (mNeedUpdatePanel)
//		rebuildControlPanel();
//}
//
//void ramPlaybackTab::drawSessions(ofEventArgs &e)
//{
//	SegmentsIter it = mSegmentMap.begin();
//	
//	while (it != mSegmentMap.end())
//	{
//		PlaybackSegment *seg = it->second;
//		
//		ramSession &session = seg->session;
//		const ramNodeArray &NA = session.getCurrentFrame();
//		
//		getActorManager().setNodeArray(NA);
//		
//		if (session.isPlaying())
//		{
//			getActorManager().setBus(NA.getName(), NA);
//		}
//		
//		if (seg->bHideActor)
//		{
//			if (getActorManager().hasBus(NA.getName()))
//			{
//				getActorManager().eraseFromBus(NA.getName());
//			}
//		}
//		
//		it++;
//	}
//}
//
//
//
//#pragma mark -
//#pragma mark Events
//void ramPlaybackTab::onFileDrop(ofDragInfo &e)
//{
//	for(int i=0; i<e.files.size(); i++)
//	{
//		const string filePath = e.files.at(i);
//		
//		try
//		{
//			coder.load(filePath);
//			ramSession session = coder.get();
//			
//			SegmentsIter it = mSegmentMap.find(session.getNodeArrayName());
//			if( it != mSegmentMap.end() ) return;
//			
//			addControlSegment(session);
//		}
//		catch (std::exception &e)
//		{
//			cout << e.what() << endl;
//		}
//	}
//}
//
//void ramPlaybackTab::onValueChanged(ofxUIEventArgs &e)
//{
//	ofxUILabelButton *button = (ofxUILabelButton *)e.widget;
//	const string name = button->getName();
//	
//	if (button == btnLoadFile)
//	{
//		cout << "load" << endl;
//	}
//	
//	if (button == btnPauseAll)
//	{
//		cout << "pause" << endl;
//		
//		SegmentsIter it = mSegmentMap.begin();
//		
//		while (it != mSegmentMap.end())
//		{
//			PlaybackSegment *seg = it->second;
////			seg.pause();
//			it++;
//		}
//	}
//	
//	if (button == btnDeleteAll)
//	{
//		SegmentsIter it = mSegmentMap.begin();
//		
//		while (it != mSegmentMap.end())
//		{
//			getActorManager().eraseFromBus(it->first);
//			it++;
//		}
//		
//		mSegmentMap.clear();
//		
//		rebuildControlPanel();
//	}
//}
//
//
//
//#pragma mark -
//#pragma mark Private methods
//void ramPlaybackTab::addControlSegment(ramSession &src)
//{
//	const string name = src.getNodeArrayName();
//	
//	
//	/// create control segment date internally
//	PlaybackSegment *seg = new PlaybackSegment();
//	seg->parent = this;
//	mSegmentMap.insert( make_pair(name, seg) );
//	src.play();
//	
//	cout << name << "is added to segment map." << endl;
//	
//	/// create and add child panel
//	const int panelIndex = mSegmentMap.size()-1;
//	const int panelHeight = 113;
//	const int panelHeaderHeight = 138;
//	
//	ofxUICanvasPlus* childPanel = seg->createPanel(src);
//	childPanel->getRect()->y = panelIndex * panelHeight + panelHeaderHeight;
//
//	
//	/// append widget, resize parent panel, load default settings
//	addWidget(childPanel);
//	autoSizeToFitWidgets();
//
//	
////	seg->loadCache();
//}
//
//void ramPlaybackTab::removeControlSegment(const string segmentName)
//{
//	mSegmentMap.erase(segmentName);
//	
//	if (getActorManager().hasBus(segmentName))
//	{
//		getActorManager().eraseFromBus(segmentName);
//	}
//	
//	mNeedUpdatePanel = true;
//}
//
//void ramPlaybackTab::rebuildControlPanel()
//{
//	
//	/// remove all widgets
//	removeWidgets();
//	resetPlacer();
//	
//	
//	/// adding panel header
//	createPanelHeader();
//	
//	
//	/// insert panels
//	SegmentsIter it = mSegmentMap.begin();
//	
//	while (it != mSegmentMap.end())
//	{
//		cout << "adding..." << endl;
//		
//		ramSession &session = it->second->session;
//		addControlSegment(session);
//		
//		it++;
//	}
//	
//	autoSizeToFitWidgets();
//	
//	mNeedUpdatePanel = false;
//}
//
//void ramPlaybackTab::createPanelHeader()
//{
//	const int width = ramGetGUI().kLength;
//	const int height = ramGetGUI().kDim * 1.3;
//	
//	addLabel("Playback", OFX_UI_FONT_LARGE);
//	addSpacer(ramGetGUI().kLength, 2);
//
//	btnLoadFile = new ofxUILabelButton("Load File", false, width, height);
//	btnHideAll = new ofxUILabelButton("Hide All Sessions", false, width, height);
//	btnPauseAll = new ofxUILabelButton("Pause All Sessions", false, width, height);
//	btnDeleteAll = new ofxUILabelButton("Delete All Sessions", false, width, height);
//	addWidgetDown( btnLoadFile );
//	addWidgetDown( btnHideAll );
//	addWidgetDown( btnPauseAll );
//	addWidgetDown( btnDeleteAll );
//}
//
//
//