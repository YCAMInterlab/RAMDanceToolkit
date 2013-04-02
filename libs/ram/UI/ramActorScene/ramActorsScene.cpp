#include "ramActorsScene.h"


#pragma mark -
#pragma mark constructor, destructor

ramActorsScene::ramActorsScene() :
bShowAllActor(true),
bRecAllActor(false),
bUseShading(true),
bUseSimpleActor(false)
{
	ofAddListener(ofEvents().fileDragEvent, this, &ramActorsScene::onFileDrop);
}

ramActorsScene::~ramActorsScene()
{
	ofRemoveListener(ofEvents().fileDragEvent, this, &ramActorsScene::onFileDrop);
}


#pragma mark -
#pragma mark to be called from ramSceneManager

string ramActorsScene::getName() const
{
	return "Actors";
}

void ramActorsScene::setupControlPanel()
{
	mLocalPanel = ramGetGUI().getCurrentUIContext();
	rebuildControlPanel();
	
	ofAddListener(mLocalPanel->newGUIEvent, this, &ramActorsScene::onValueChanged);
}

void ramActorsScene::setup()
{
	/// font setting to draw "RECORDING" on screen right top
	fontSize = 30;
	font.loadFont(ramToResourcePath("Fonts/FreeUniversal-Regular.ttf"), fontSize, true, true);
	font.setLineHeight(fontSize*1.4f);
	font.setLetterSpacing(1.0);
	
	
	/// light for drawActor
	light.setPosition(300, 600, 300);
	
	
	/// register events
	ofAddListener(ofEvents().keyPressed, this, &ramActorsScene::onKeyPressed);
}

void ramActorsScene::update()
{
    
    /// refresh control panel if it's needed
	if (needsUpdatePanel())
		rebuildControlPanel();
	
    
	SegmentsIter it = mSegmentsMap.begin();
	
	while (it != mSegmentsMap.end())
	{
		BaseSegment *seg = it->second;
		
		/// position reset
		if (seg->bNeedsResetPos)
		{
			seg->position = ofPoint::zero();
			seg->bNeedsResetPos = false;
		}
        
        /// realtime osc data
        if (seg->getType() == RAM_UI_SEGMENT_TYPE_CONTROL)
        {
            seg->session.filter( getNodeArray(it->first) );
        }
        /// recording data playback
        else if (seg->getType() == RAM_UI_SEGMENT_TYPE_PLAYBACK)
        {
            seg->session.updatePlayhead();
            
            ramNodeArray NA = seg->session.getCurrentFrame();
            NA.setTimestamp(ofGetElapsedTimef());
            getActorManager().instance().setNodeArray(NA);
        }
		
		it++;
	}
    
    cout << "--- total size:" << mSegmentsMap.size() << endl;
}

void ramActorsScene::draw()
{
    ///
	bRecording = false;
	
    
    ///
    ramBeginCamera();

    SegmentsIter it = mSegmentsMap.begin();
    
    while (it != mSegmentsMap.end())
    {
        const string name = it->first;

        assert( getActorManager().hasNodeArray(name) );
        
        ramNodeArray &NA = getNodeArray(name);
        ramDrawBasicActor(NA);
        
        it++;
        
    }
    ramEndCamera();
    
    
    
//
//	for(int i=0; i<getNumNodeArray(); i++)
//	{
//		ramNodeArray &NA = getNodeArray(i);
//		
//		const string name = NA.getName();
//		
//		SegmentsIter it = mSegmentsMap.find(name);
//		
////		assert(it != mSegmentsMap.end());
//		
//		BaseSegment *seg = it->second;
//		
//		/// draw if "Show actor" toggle is anabled
//		// note that ofxUIImageToggle shows hilighted image when it's false,
//
//		if (!seg->bHideActor)
//		{
//			ofPushMatrix();
//			ofPushStyle();
//			{
//				
//				if (bUseShading) light.enable();
//				
//				ofSetColor(seg->jointColor);
//				ofTranslate(seg->position.x, 0, seg->position.y);
//				
//				if (NA.isRigid())
//				{
//					ramDrawBasicRigid((ramRigidBody&)NA);
//				}
//				else
//				{
//					if (bUseSimpleActor) ramDrawBasicActor((ramActor&)NA);
//					else drawNodes(NA);
//				}
//				
//				if (bUseShading) light.disable();
//			}
//			ofPopStyle();
//			ofPopMatrix();
//		}
		
		
//		if (seg->bRecording)
//		{
//			bRecording = true;
//		}
//	}
}

void ramActorsScene::drawHUD()
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



#pragma mark -
#pragma mark Events fired from ActorManager
	
void ramActorsScene::onActorSetup(const ramActor &actor)
{
    cout << __FUNCTION__ << endl;
    addSegment(new ControlSegment(actor.getName()));
}

void ramActorsScene::onRigidSetup(const ramRigidBody &rigid)
{
    cout << __FUNCTION__ << endl;
    addSegment(new ControlSegment(rigid.getName()));
}

void ramActorsScene::onActorExit(const ramActor &actor)
{
    cout << __FUNCTION__ << endl;
    cout << actor.getName() << endl;
	removeControlSegment(actor.getName());
}

void ramActorsScene::onRigidExit(const ramRigidBody &rigid)
{
    cout << __FUNCTION__ << endl;
	removeControlSegment(rigid.getName());
}



#pragma mark -
#pragma mark Events
void ramActorsScene::onKeyPressed(ofKeyEventArgs &e)
{
	if (e.key == ' ')
	{
        bool newState = !getActorManager().isFreezed();
        cout << "new state is " << (newState ? "stop" : "play") << endl;
        
        /// realtime osc data
		getActorManager().setFreezed(newState);
		
        
        /// playback data
        SegmentsIter it = mSegmentsMap.begin();
        
        while (it != mSegmentsMap.end())
        {
            BaseSegment *seg = it->second;
            if (seg->getType() == RAM_UI_SEGMENT_TYPE_PLAYBACK)
            {
                seg->session.setFreeze(newState);
            }
            it++;
        }
        
        
        /// UI Button state
		btnPause->setValue(newState);
		btnPause->stateChange();
	}
}

void ramActorsScene::onValueChanged(ofxUIEventArgs &e)
{
	const string name = e.widget->getName();
	
	if (name == "Show All Actors")
	{
		SegmentsIter it = mSegmentsMap.begin();
		
		while (it != mSegmentsMap.end())
		{
			ControlSegment *seg = (ControlSegment *)it->second;
			ofxUILabelToggle *toggle = (ofxUILabelToggle *)e.widget;
			
			// note that ofxUIImageToggle shows hilighted image when it's false,
			const bool value = !toggle->getValue();
			
			seg->bHideActor = toggle->getValue();
			seg->btnHideActor->setValue(value);
			seg->btnHideActor->stateChange();
			seg->saveCache();
			
			it++;
		}
	}
	
	if (name == "Reset Positions")
	{
		SegmentsIter it = mSegmentsMap.begin();
		
		while (it != mSegmentsMap.end())
		{
			ControlSegment *seg = (ControlSegment *)it->second;
			seg->position = ofPoint::zero();
			seg->saveCache();
			
			it++;
		}
	}
	
	if (name == "Pause (Space key)")
	{
		getActorManager().toggleFreeze();
		
		btnPause->setValue( getActorManager().isFreezed() );
		btnPause->stateChange();
	}
	
	if (name == "Recording All Actors")
	{
		SegmentsIter it = mSegmentsMap.begin();
		
		while (it != mSegmentsMap.end())
		{
			ControlSegment *seg = (ControlSegment *)it->second;
			ofxUILabelToggle *toggle = (ofxUILabelToggle *)e.widget;
			
			const bool value = toggle->getValue();
			seg->toggleRecording(value);
			
			// no need to saveCache on click this recording button
			it++;
		}
	}
}



#pragma mark -
#pragma mark experimental
void ramActorsScene::drawNodes(const ramNodeArray &NA)
{
	ofPushStyle();
	
	ofNoFill();
	ofSetRectMode(OF_RECTMODE_CENTER);
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_CULL_FACE);
	
	ofColor front_color(ofGetStyle().color, 200);
	ofColor back_color(ofGetStyle().color, 150);
	
	for (int i = 0; i < NA.getNumNode(); i++)
	{
		ofSetColor(front_color);
		
		const ramNode &node = NA.getNode(i);
		const ramNode *parent = node.getParent();
		if (parent == NULL) continue;
		
		ramBox(node, 2);
		
		parent->beginTransform();
		
		ofVec3f axis(0, 0, 1);
		ofVec3f c = node.getPosition().normalized().crossed(axis);
		
		ofRotate(90, c.x, c.y, c.z);
		
		ofVec3f p0 = node.getGlobalPosition();
		ofVec3f p1 = parent->getGlobalPosition();
		
		float dist = p0.distance(p1);
		float offset = 0.2;
		
		glNormal3f(0, 0, 0);
		ofLine(ofVec3f(0), ofVec3f(0, 0, -dist));
		
		if (i < 4)
			glScalef(1., 1.8, 1);
		else if (i == 4)
			glScalef(1, 1, 3);
		else
			glScalef(1., 0.8, 1);
		
		glBegin(GL_TRIANGLE_STRIP);
		for (int n = 0; n < 6; n++)
		{
			float d = ofMap(n, 0, 5, 0, 1);
			float dd = ofMap(d, 0, 1, offset, 1 - offset);
			
			float xx = sin(d * PI) * 2 + 4;
			float zz = dd * -dist;
			float w = 5;
			
			glNormal3f(1, 0.5, 0);
			glVertex3f(xx, w, zz);
			glNormal3f(1, -0.5, 0);
			glVertex3f(xx, -w, zz);
		}
		glEnd();
		
		ofSetColor(back_color);
		
		glBegin(GL_TRIANGLE_STRIP);
		for (int n = 0; n < 6; n++)
		{
			float d = ofMap(n, 0, 5, 0, 1);
			float dd = ofMap(d, 0, 1, offset, 1 - offset);
			
			float xx = -sin(d * PI) * 1 - 6;
			float zz = dd * -dist;
			float w = 3;
			
			glNormal3f(-1, 0.5, 0);
			glVertex3f(xx, -w, zz);
			glNormal3f(-1, -0.5, 0);
			glVertex3f(xx, w, zz);
		}
		glEnd();
		
		parent->endTransform();
	}
	
	glPopAttrib();
	
	ofPopStyle();
}


#pragma mark -
#pragma mark private methods

void ramActorsScene::onFileDrop(ofDragInfo &e)
{
    for(int i=0; i<e.files.size(); i++)
	{
		const string filePath = e.files.at(i);
		
		try
		{
			coder.load(filePath);
			ramSession session = coder.get();
			
			SegmentsIter it = mSegmentsMap.find(session.getNodeArrayName());
            
			if( it != mSegmentsMap.end() ) return;
			
            const string name = session.getNodeArrayName();
            
            PlaybackSegment *seg = new PlaybackSegment(name);
            seg->session = session;
            seg->session.play();
			addSegment(seg);
		}
		catch (std::exception &e)
		{
			cout << e.what() << endl;
		}
	}
}

void ramActorsScene::loadFile(const string filePath)
{
    
}

void ramActorsScene::addSegment(BaseSegment *newSegment)
{
    const string name = newSegment->getName();
    
    if (mSegmentsMap.find(name) == mSegmentsMap.end())
        mSegmentsMap.insert( make_pair(name, newSegment) );
    
    
	/// create and add child panel
	const int panelIndex = mSegmentsMap.size()-1;
	const int panelHeight = 210;
	const int panelHeaderHeight = 155;
	
	ofxUICanvasPlus* childPanel = newSegment->createPanel(name);
	childPanel->getRect()->y = panelIndex * panelHeight + panelHeaderHeight;
	
    
	/// append widget, resize parent panel, load default settings
	mLocalPanel->addWidget(childPanel);
	mLocalPanel->autoSizeToFitWidgets();
	newSegment->loadCache();
}


void ramActorsScene::removeControlSegment(const string name)
{
	SegmentsIter it = mSegmentsMap.find(name);
	
	assert( it != mSegmentsMap.end() );
    
    mSegmentsMap.erase( it );

    setNeedsUpdatePanel(true);
}

void ramActorsScene::rebuildControlPanel()
{
	/// remove all widgets
	mLocalPanel->removeWidgets();
	mLocalPanel->resetPlacer();
	
	
	/// adding panel header
	createPanelHeader();
	
	
	/// insert panels
    
    SegmentsIter it = mSegmentsMap.begin();
    
    while (it != mSegmentsMap.end())
    {
        BaseSegment *seg = it->second;
        addSegment(seg);
//        if (seg->getType() == RAM_UI_SEGMENT_TYPE_CONTROL)
//        {
//        }
//        else if (seg->getType() == RAM_UI_SEGMENT_TYPE_PLAYBACK)
//        {
//        }
        
        it++;
    }
    
	mLocalPanel->autoSizeToFitWidgets();
	
	setNeedsUpdatePanel(false);
}


void ramActorsScene::createPanelHeader()
{
	const int width = ramGetGUI().kLength/2 - 5;
	const int height = ramGetGUI().kDim * 1.3;
	
	mLocalPanel->addLabel(getName(), OFX_UI_FONT_LARGE);
	mLocalPanel->addSpacer(ramGetGUI().kLength, 2);
	
    
	mLocalPanel->addWidgetDown( new ofxUILabelButton("Load Recorded File", false, ramGetGUI().kLength, height) );
	
	/// 2x2 matrix
	btnShowAll = new ofxUILabelToggle("Show All Actors", &bShowAllActor, width, height);
	mLocalPanel->addWidgetDown( btnShowAll );
	mLocalPanel->addWidgetRight( new ofxUILabelButton("Reset Positions", &bRecAllActor, width, height) );
	mLocalPanel->addWidgetDown( new ofxUILabelToggle("Use Shading", &bUseShading, width, height) );
	mLocalPanel->addWidgetRight( new ofxUILabelToggle("Use SimpleActor", &bUseSimpleActor, width, height) );
	
	
	/// buttons which are controlled programatically
	//  all of the child widgets of mLocalPanel are deleted when rebuildControlPanel is executed
	//  so it needs to make new pointer
	btnPause = new ofxUILabelToggle("Pause (Space key)", false, ramGetGUI().kLength, height);
	btnRecAll = new ofxUILabelToggle("Recording All Actors", false, ramGetGUI().kLength, height);
	mLocalPanel->addWidgetDown(btnPause);
	mLocalPanel->addWidgetDown(btnRecAll);
}

void ramActorsScene::setShowAll(bool showAll)
{
	btnShowAll->setValue(showAll);
	btnShowAll->triggerSelf();
}

bool ramActorsScene::getShowAll() const
{
	return bShowAllActor;
}

void ramActorsScene::setNeedsUpdatePanel(const bool needsUpdate)
{
    bNeedUpdatePanel = needsUpdate;
}

bool ramActorsScene::needsUpdatePanel()
{
    return bNeedUpdatePanel;
}