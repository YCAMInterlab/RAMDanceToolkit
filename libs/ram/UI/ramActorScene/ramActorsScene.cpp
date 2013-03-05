#include "ramActorsScene.h"


#pragma mark -
#pragma mark constructor, destructor

ramActorsScene::ramActorsScene() :
bShowAllActor(true),
bRecAllActor(false),
bUseShading(true),
bUseSimpleActor(false	)
{

}

ramActorsScene::~ramActorsScene()
{
	delete btnPause;
	delete btnRecAll;
}


#pragma mark -
#pragma mark called from ramSceneManager

const string ramActorsScene::getName()
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
	font.loadFont(ramToResourcePath("Fonts/NewMedia Fett.ttf"), fontSize, true, true);
	font.setLineHeight(fontSize*1.4f);
	font.setLetterSpacing(1.0);
	
	
	/// light for drawActor
	light.setPosition(300, 600, 300);
	
	
	/// register events
	ofAddListener(ofEvents().keyPressed, this, &ramActorsScene::onKeyPressed);
}

void ramActorsScene::update()
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

void ramActorsScene::draw()
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
				
				if (bUseShading) light.enable();
				
				ofSetColor(seg->jointColor);
				ofTranslate(seg->position.x, 0, seg->position.y);
				
				if (NA.isRigid())
				{
					ramDrawBasicRigid((ramRigidBody&)NA);
				}
				else
				{
					if (bUseSimpleActor) ramDrawBasicActor((ramActor&)NA);
					else drawNodes(NA);
				}
				
				if (bUseShading) light.disable();
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
	addControlSegment(actor);
}

void ramActorsScene::onRigidSetup(const ramRigidBody &rigid)
{
	addControlSegment(rigid);
}

void ramActorsScene::onActorExit(const ramActor &actor)
{
	removeControlSegment(actor);
}

void ramActorsScene::onRigidExit(const ramRigidBody &rigid)
{
	removeControlSegment(rigid);
}



#pragma mark -
#pragma mark Events
void ramActorsScene::onKeyPressed(ofKeyEventArgs &e)
{
	if (e.key == ' ')
	{
		getActorManager().toggleFreeze();
		
		btnPause->setValue( getActorManager().isFreezed() );
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
			ControlSegment *seg = it->second;
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
			ControlSegment *seg = it->second;
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
			ControlSegment *seg = it->second;
			ofxUILabelToggle *toggle = (ofxUILabelToggle *)e.widget;
			
			const bool value = toggle->getValue();
			seg->toggleRecording(value);
			
			// no need to saveCache on click this recording button
			it++;
		}
	}
}



#pragma mark -
#pragma mark experimental actor
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

void ramActorsScene::addControlSegment(const ramNodeArray &NA)
{
	const string name = NA.getName();
	const SegmentsIter it = mSegmentsMap.find(name);
	
	assert( it == mSegmentsMap.end() );
	
	
	/// create control segment date internally
	ControlSegment *seg = new ControlSegment();
	mSegmentsMap.insert( make_pair(name, seg) );
	
	
	/// create and add child panel
	const int panelIndex = mSegmentsMap.size()-1;
	const int panelHeight = 210;
	const int panelHeaderHeight = 155;
	
	ofxUICanvasPlus* childPanel = seg->createPanel(NA);
	childPanel->getRect()->y = panelIndex * panelHeight + panelHeaderHeight;
	
	
	/// append widget, resize parent panel, load default settings
	mLocalPanel->addWidget(childPanel);
	mLocalPanel->autoSizeToFitWidgets();
	seg->loadCache();
}

void ramActorsScene::removeControlSegment(const ramNodeArray &NA)
{
	const string name = NA.getName();
	const SegmentsIter it = mSegmentsMap.find(name);
	
	assert( it != mSegmentsMap.end() );
	
	mNeedUpdatePanel = true;
}

void ramActorsScene::rebuildControlPanel()
{
	/// remove all widgets
	mLocalPanel->removeWidgets();
	mLocalPanel->resetPlacer();
	mSegmentsMap.clear();
	
	
	/// adding panel header
	createPanelHeader();
	
	
	/// insert panels
	for(int i=0; i<getNumNodeArray(); i++)
	{
		const ramNodeArray &NA = getNodeArray(i);
		addControlSegment(NA);
	}
	
	mLocalPanel->autoSizeToFitWidgets();
	
	mNeedUpdatePanel = false;
}


void ramActorsScene::createPanelHeader()
{
	const int width = ramGetGUI().kLength/2 - 5;
	const int height = ramGetGUI().kDim * 1.3;
	
	mLocalPanel->addLabel(getName(), OFX_UI_FONT_LARGE);
	mLocalPanel->addSpacer(ramGetGUI().kLength, 2);
	
	
	/// 2x2 matrix
	mLocalPanel->addWidgetDown( new ofxUILabelToggle("Show All Actors", &bShowAllActor, width, height) );
	mLocalPanel->addWidgetRight( new ofxUILabelButton("Reset Positions", &bRecAllActor, width, height) );
	mLocalPanel->addWidgetDown( new ofxUILabelToggle("Use Shading", &bUseShading, width, height) );
	mLocalPanel->addWidgetRight( new ofxUILabelToggle("Use Simple Actor", &bUseSimpleActor, width, height) );
	
	
	/// buttons which are controlled programatically
	//  all of the child widgets of mLocalPanel are deleted when rebuildControlPanel is executed
	//  so it needs to make new pointer
	btnPause = new ofxUILabelToggle("Pause (Space key)", false, ramGetGUI().kLength, height);
	btnRecAll = new ofxUILabelToggle("Recording All Actors", false, ramGetGUI().kLength, height);
	mLocalPanel->addWidgetDown( btnPause );
	mLocalPanel->addWidgetDown( btnRecAll );
}

