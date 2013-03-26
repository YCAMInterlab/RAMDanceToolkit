#include "ramPlaybackTab.h"
#include "ramControlPanel.h"



#pragma mark -
#pragma mark Constructor
ramPlaybackTab::ramPlaybackTab() : ofxUITab("Playback", false)
{
	ofAddListener(this->newGUIEvent, this, &ramPlaybackTab::onValueChanged);
	ofAddListener(ofEvents().fileDragEvent, this, &ramPlaybackTab::onFileDrop);
	ofAddListener(ofEvents().setup, this, &ramPlaybackTab::setup);
	ofAddListener(ofEvents().draw, this, &ramPlaybackTab::drawSessions);

}

ramPlaybackTab::~ramPlaybackTab()
{
	ofRemoveListener(this->newGUIEvent, this, &ramPlaybackTab::onValueChanged);
	ofRemoveListener(ofEvents().fileDragEvent, this, &ramPlaybackTab::onFileDrop);
	ofRemoveListener(ofEvents().setup, this, &ramPlaybackTab::setup);
	ofRemoveListener(ofEvents().draw, this, &ramPlaybackTab::drawSessions);
}



#pragma mark -
#pragma mark update and draw
void ramPlaybackTab::setup(ofEventArgs &e)
{
	removeWidgets();
	resetPlacer();
	createPanelHeader();
	autoSizeToFitWidgets();
}

void ramPlaybackTab::update()
{
	SegmentsIter it = mSegmentMap.begin();
	
	while (it != mSegmentMap.end())
	{
		PlaybackSegment *seg = it->second;
		seg->update();
		
		it++;
	}
	
	if (mNeedUpdatePanel)
		rebuildControlPanel();
}

void ramPlaybackTab::drawSessions(ofEventArgs &e)
{
	SegmentsIter it = mSegmentMap.begin();
	
	while (it != mSegmentMap.end())
	{
		PlaybackSegment *seg = it->second;
		
		ramSession &session = seg->session;
		const ramNodeArray &NA = session.getCurrentFrame();
		
		getActorManager().setNodeArray(NA);
		
		if (session.isPlaying())
		{
			getActorManager().setBus(NA.getName(), NA);
		}
		
		if (seg->bHideActor)
		{
			if (getActorManager().hasBus(NA.getName()))
			{
				getActorManager().eraseFromBus(NA.getName());
			}
		}
		
		it++;
	}
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
			coder.load(filePath);
			ramSession session = coder.get();
			
			SegmentsIter it = mSegmentMap.find(session.getNodeArrayName());
			if( it != mSegmentMap.end() ) return;
			
			addControlSegment(session);
		}
		catch (std::exception &e)
		{
			cout << e.what() << endl;
		}
	}
}

void ramPlaybackTab::onValueChanged(ofxUIEventArgs &e)
{
	ofxUILabelButton *button = (ofxUILabelButton *)e.widget;
	const string name = button->getName();
	
	if (button == btnLoadFile)
	{
		cout << "load" << endl;
	}
	
	if (button == btnPauseAll)
	{
		cout << "pause" << endl;
		
		SegmentsIter it = mSegmentMap.begin();
		
		while (it != mSegmentMap.end())
		{
			PlaybackSegment *seg = it->second;
//			seg.pause();
			it++;
		}
	}
	
	if (button == btnDeleteAll)
	{
		SegmentsIter it = mSegmentMap.begin();
		
		while (it != mSegmentMap.end())
		{
			getActorManager().eraseFromBus(it->first);
			it++;
		}
		
		mSegmentMap.clear();
		
		rebuildControlPanel();
	}
}



#pragma mark -
#pragma mark Private methods
void ramPlaybackTab::addControlSegment(ramSession &src)
{
	const string name = src.getNodeArrayName();
	
	
	/// create control segment date internally
	PlaybackSegment *seg = new PlaybackSegment();
	seg->parent = this;
	mSegmentMap.insert( make_pair(name, seg) );
	src.play();
	
	cout << name << "is added to segment map." << endl;
	
	/// create and add child panel
	const int panelIndex = mSegmentMap.size()-1;
	const int panelHeight = 113;
	const int panelHeaderHeight = 138;
	
	ofxUICanvasPlus* childPanel = seg->createPanel(src);
	childPanel->getRect()->y = panelIndex * panelHeight + panelHeaderHeight;

	
	/// append widget, resize parent panel, load default settings
	addWidget(childPanel);
	autoSizeToFitWidgets();

	
//	seg->loadCache();
}

void ramPlaybackTab::removeControlSegment(const string segmentName)
{
	mSegmentMap.erase(segmentName);
	
	if (getActorManager().hasBus(segmentName))
	{
		getActorManager().eraseFromBus(segmentName);
	}
	
	mNeedUpdatePanel = true;
}

void ramPlaybackTab::rebuildControlPanel()
{
	
	/// remove all widgets
	removeWidgets();
	resetPlacer();
	
	
	/// adding panel header
	createPanelHeader();
	
	
	/// insert panels
	SegmentsIter it = mSegmentMap.begin();
	
	while (it != mSegmentMap.end())
	{
		cout << "adding..." << endl;
		
		ramSession &session = it->second->session;
		addControlSegment(session);
		
		it++;
	}
	
	autoSizeToFitWidgets();
	
	mNeedUpdatePanel = false;
}

void ramPlaybackTab::createPanelHeader()
{
	const int width = ramGetGUI().kLength;
	const int height = ramGetGUI().kDim * 1.3;
	
	addLabel("Playback", OFX_UI_FONT_LARGE);
	addSpacer(ramGetGUI().kLength, 2);

	btnLoadFile = new ofxUILabelButton("Load File", false, width, height);
	btnHideAll = new ofxUILabelButton("Hide All Sessions", false, width, height);
	btnPauseAll = new ofxUILabelButton("Pause All Sessions", false, width, height);
	btnDeleteAll = new ofxUILabelButton("Delete All Sessions", false, width, height);
	addWidgetDown( btnLoadFile );
	addWidgetDown( btnHideAll );
	addWidgetDown( btnPauseAll );
	addWidgetDown( btnDeleteAll );
}




// ------------------------------------------------------------------



#pragma mark -
#pragma mark constructor, destructor

PlaybackSegment::PlaybackSegment()
{
	reset();
	
	btnHideActor = new ofxUIImageToggle(32, 32, &bHideActor, ramToResourcePath("Images/show.png"),"show");
	btnPlayActor = new ofxUIImageToggle(32, 32, &bPlaying, ramToResourcePath("Images/play.png"),"play");
	btnCueActor = new ofxUIImageButton(32, 32, &bNeedsResetPos, ramToResourcePath("Images/reset.png"),"cue");
	btnDeleteActor = new ofxUIImageButton(32, 32, &bDelete, ramToResourcePath("Images/delete.png"),"delete");
	sliderProgress = new ofxUISlider("Progress", 0, 1, &progress, 100, 20);
}

PlaybackSegment::~PlaybackSegment()
{
	delete btnHideActor;
	delete btnPlayActor;
	delete btnCueActor;
	delete btnDeleteActor;
}




#pragma mark -
#pragma mark public methods

void PlaybackSegment::update()
{
	session.updatePlayhead();
	progress = session.getPlayhead();
}

ofxUICanvasPlus* PlaybackSegment::createPanel(const ramSession &src)
{
	session = src;
	
	name = session.getNodeArrayName();
	const float width = ramGetGUI().kLength;
	const float height = ramGetGUI().kDim+3;
	
	
	ofxUICanvasPlus *child = new ofxUICanvasPlus();
	child->disableAppDrawCallback();
	child->disableAppEventCallbacks();
	child->disableKeyEventCallbacks();
	child->disableMouseEventCallbacks();
	
	
	/// section title
	child->addWidgetDown(new ofxUILabel(name, OFX_UI_FONT_MEDIUM));
	child->addSpacer(width, 2);
	
	
	/// Icons
	child->addWidgetDown(btnHideActor);
	child->addWidgetRight(btnPlayActor);
	child->addWidgetRight(btnCueActor);
	child->addWidgetRight(btnDeleteActor);
	
	
	/// actor color
	child->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
	child->addSlider("R", 0, 1, &jointColor.r, 43, height);
	child->addSlider("G", 0, 1, &jointColor.g, 43, height);
	child->addSlider("B", 0, 1, &jointColor.b, 43, height);
	child->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
	
//	child->addWidgetDown(sliderProgress);
//	child->addSlider("Progress", 0, src.getDuration(), &progress, width, height);
	
	child->autoSizeToFitWidgets();
	
	ofAddListener(child->newGUIEvent, this, &PlaybackSegment::onValueChanged);
	
	return child;
}

void PlaybackSegment::loadCache()
{
	//	if ( !ofFile::doesFileExist(getXMLFilePath()) ) return;
	//
	//	XML.clear();
	//	XML.loadFile(getXMLFilePath());
	//
	//	/// color
	//	XML.pushTag("color");
	//	jointColor.r = XML.getValue("r", 0.8);
	//	jointColor.g = XML.getValue("g", 0.8);
	//	jointColor.b = XML.getValue("b", 0.8);
	//	XML.popTag();
	//
	//	/// position
	//	XML.pushTag("position");
	//	position.x = XML.getValue("x", 0.0);
	//	position.y = XML.getValue("y", 0.0);
	//	XML.popTag();
	//
	//	/// boolean state
	//	XML.pushTag("state");
	//	bHideActor = XML.getValue("hideActor", 0);
	//	btnHideActor->setValue(bHideActor);
	//	btnHideActor->stateChange();
	//	XML.popTag();
}

void PlaybackSegment::saveCache()
{
	//	XML.clear();
	//
	//	/// color
	//	XML.addTag("color");
	//	XML.pushTag("color");
	//	XML.addValue("r", jointColor.r);
	//	XML.addValue("g", jointColor.g);
	//	XML.addValue("b", jointColor.b);
	//	XML.popTag();
	//
	//	/// position
	//	XML.addTag("position");
	//	XML.pushTag("position");
	//	XML.addValue("x", position.x);
	//	XML.addValue("y", position.y);
	//	XML.popTag();
	//
	//	/// boolean states
	//	XML.addTag("state");
	//	XML.pushTag("state");
	//	XML.addValue("hideActor", bHideActor);
	//	XML.popTag();
	//
	//	XML.saveFile(getXMLFilePath());
}

void PlaybackSegment::setVisible(const bool value)
{
	bHideActor = value;
}

void PlaybackSegment::setPlay(const bool value)
{
	if (!value)
		session.play();
	else
		session.stop();
}

void PlaybackSegment::cue()
{
	session.setPlayhead(0);
}


#pragma mark -
#pragma mark Events

void PlaybackSegment::onValueChanged(ofxUIEventArgs& e)
{
	const string widgetName = e.widget->getName();
	
	
	if (widgetName == "show")
	{
		ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
		const bool value = toggle->getValue();
		setVisible(value);
	}

	if (widgetName == "play")
	{
		ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
		const bool value = toggle->getValue();
		setPlay(value);
	}

	if (widgetName == "cue")
	{
		cue();
	}

	if (widgetName == "delete")
	{
		parent->removeControlSegment(session.getNodeArrayName());
	}
	
	
	progress = ofRandom(0,3);
	
	saveCache();
}




#pragma mark -
#pragma mark private methods

void PlaybackSegment::reset()
{
	bHideActor = false;
	bNeedsResetPos = false;
	bDelete = false;
	
	jointColor = ofFloatColor(1.0, 0.15, 0.4);
	position = ofPoint(0, 0);
}

const string PlaybackSegment::getXMLFilePath() const
{
	return ramToResourcePath("Settings/Actors/color."+name+".xml");
}

