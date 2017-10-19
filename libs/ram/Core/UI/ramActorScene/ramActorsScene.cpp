// 
// ramActorsScene.cpp - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "ramActorsScene.h"


#pragma mark -
#pragma mark constructor, destructor

using namespace rdtk;

ActorsScene::ActorsScene() :
bShowAllActor(true),
bRecAllActor(false),
bUseShading(true),
bUseSimpleActor(true),
loadNewFile(false)
{
	ofAddListener(ofEvents().fileDragEvent, this, &ActorsScene::onFileDrop);
}

ActorsScene::~ActorsScene()
{
	ofRemoveListener(ofEvents().fileDragEvent, this, &ActorsScene::onFileDrop);
}


#pragma mark -
#pragma mark to be called from ramSceneManager

string ActorsScene::getName() const
{
	return "Actors";
}

void ActorsScene::setupControlPanel()
{
	mLocalPanel = GetGUI().getCurrentUIContext();
	rebuildControlPanel();
	
	ofAddListener(mLocalPanel->newGUIEvent, this, &ActorsScene::onValueChanged);
	
// ignore win32
#ifndef TARGET_WIN32
	/// load Ando_1.tsv if this is the first launch of this application
	string initial_file_path = "Settings/presets/preset.init.xml";
	if (!ofFile::doesFileExist( ToResourcePath(initial_file_path) ))
	{
		ofBuffer buf("hello ram!");
		ofBufferToFile(ToResourcePath(initial_file_path), buf);
		
		string fake_yoko = ToResourcePath("MotionData/Ando_1.tsv");
		
		if (ofFile::doesFileExist( fake_yoko ))
		{
			loadFile(fake_yoko);
		}
	}
#endif
}

void ActorsScene::setup()
{
	/// to get message from PlaybackSegment
	ofRegisterGetMessages(this);
	
	
	/// font setting to draw "RECORDING" on screen right top
	fontSize = 30;
	font.load(ToResourcePath(RAM_FONT_FILE), fontSize, true, true);
	font.setLineHeight(fontSize*1.4f);
	font.setLetterSpacing(1.0);
	
	
	/// light for drawActor
	light.setPosition(300, 600, 300);
	
	
	/// register events
	ofAddListener(ofEvents().keyPressed, this, &ActorsScene::onKeyPressed);
}

void ActorsScene::update()
{
    if (loadNewFile) {
        loadFile(newFilePath);
        loadNewFile = false;
    }
    
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
            if (static_cast<PlaybackSegment*>(seg)->isPlaying())
            {
                seg->session.updatePlayhead();
				
                NodeArray & NA = seg->session.getCurrentFrame();
				NA.setPlayback(true);
				NA.setTimestamp(ofGetElapsedTimef());
				getActorManager().instance().setNodeArray(NA);
            }
        }
		
		it++;
	}
}

void ActorsScene::draw()
{
    ///
	bRecording = false;
    
    ///
    BeginCamera();

	for(int i=0; i<getNumNodeArray(); i++)
	{
		NodeArray &NA = getNodeArray(i);
		
		const string& name = NA.getName();
		
		SegmentsIter it = mSegmentsMap.find(name);
		
		if (it == mSegmentsMap.end())
			continue;
		
		BaseSegment *seg = it->second;
		
		/// draw if "Show actor" toggle is anabled
		// note that ofxUIImageToggle shows hilighted image when it's false,
        if (seg->isVisible())
		{
			ofPushMatrix();
			ofPushStyle();
			{
				
				if (bUseShading) light.enable();
				
				ofSetColor(seg->jointColor);
				ofTranslate(seg->position.x, 0, seg->position.y);
				
				if (seg->isFixActor() &&
					NA.isTypeOf(RAM_NODEARRAY_TYPE_ACTOR))
					ofTranslate(-NA.getNode(Actor::JOINT_CHEST).getGlobalPosition().x,0,
								-NA.getNode(Actor::JOINT_CHEST).getGlobalPosition().z);
				
				
				if (NA.isRigid())
				{
					DrawBasicRigid((RigidBody&)NA);
				}
				else
				{
					if (bUseSimpleActor)
                        DrawBasicActor((Actor&)NA);
					else
                        drawNodes((Actor&)NA);
				}
				
				if (bUseShading) light.disable();
			}
			ofPopStyle();
			ofPopMatrix();
		}
		
		if (seg->getType() == RAM_UI_SEGMENT_TYPE_CONTROL)
        {
            if (static_cast<ControlSegment*>(seg)->isRecording())
            {
                bRecording = true;
            }
        }
	}
    EndCamera();
}

void ActorsScene::drawHUD()
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
	
void ActorsScene::onActorSetup(const Actor &actor)
{
    addSegment(new ControlSegment(actor.getName()));
}

void ActorsScene::onRigidSetup(const RigidBody &rigid)
{
    addSegment(new ControlSegment(rigid.getName()));
}

void ActorsScene::onActorExit(const Actor &actor)
{
	removeControlSegment(actor.getName());
}

void ActorsScene::onRigidExit(const RigidBody &rigid)
{
	removeControlSegment(rigid.getName());
}



#pragma mark -
#pragma mark Events
void ActorsScene::gotMessage(ofMessage &msg)
{
	vector<string> keys = ofSplitString(msg.message, "/");
	
    string route = ofSplitString(msg.message, " ")[0];
	string value = msg.message;
	ofStringReplace(value, route+" ", "");
	
	if (route == "/PlaybackSegment/remove")
	{
		removeControlSegment(value);
	}
}

void ActorsScene::onKeyPressed(ofKeyEventArgs &e)
{
	if (e.key == ' ')
	{
        const bool newState = !getActorManager().isFreezed();
        btnPause->setValue(newState);
        btnPause->triggerSelf();
    }
}

void ActorsScene::onValueChanged(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	
    if (name == "Load Recorded File")
    {
        ofxUIButton *button = (ofxUIButton *)e.widget;
        
        if (button->getValue())
        {
            ofFileDialogResult result = ofSystemLoadDialog("Load recorded *.tsv file.", false, "");
            if (result.bSuccess) {
                loadNewFile = true;
                newFilePath = result.getPath();
            }
        }
    }
    
	if (name == "Show All Actors")
	{
        ofxUILabelToggle *t = (ofxUILabelToggle *)e.widget;
		showAll(t->getValue());
	}
	
	if (name == "Reset Positions")
	{
        ofxUILabelToggle *t = (ofxUILabelToggle *)e.widget;
        resetPosAll(t->getValue());
	}
	
	if (name == "Pause (Space key)")
	{
        ofxUILabelToggle *t = (ofxUILabelToggle *)e.widget;
		pauseAll(t->getValue());
	}
	
	if (name == "Recording All Actors")
	{
        ofxUILabelToggle *t = (ofxUILabelToggle *)e.widget;
		recAll(t->getValue());
	}
}

void ActorsScene::onFileDrop(ofDragInfo &e)
{
// ignore win32
#ifndef TARGET_WIN32
    for(int i=0; i<e.files.size(); i++)
	{
		const string filePath = e.files.at(i);
		loadFile(filePath);
	}
#endif
}

void ActorsScene::loadFile(const string& filePath)
{
	if (mSegmentsMap.size() >= MAX_ACTORS)
		return;

   try
   {
		coder.load(filePath);
		Session session = coder.get();

        string name = session.getNodeArrayName();

		SegmentsIter it = mSegmentsMap.find(name);
		if( it != mSegmentsMap.end() ) return;

		PlaybackSegment *seg = new PlaybackSegment(name);
		seg->session = session;
		seg->session.prepareForPlay();
		seg->session.play();
		addSegment(seg);
   }
   catch (std::exception &e)
   {
		cout << e.what() << endl;
   }
}



#pragma mark -
#pragma mark control methods

void ActorsScene::showAll(bool bShow)
{
	// note that ofxUIImageToggle shows hilighted image when it's false,
	const bool bHide = !bShow;
	
	bShowAllActor = bHide;
	btnShowAll->setValue(bShow);
	btnShowAll->stateChange();
	
    SegmentsIter it = mSegmentsMap.begin();
    
    while (it != mSegmentsMap.end())
    {
        BaseSegment *seg = it->second;
        
        seg->bHideActor = bHide;
        seg->btnHideActor->setValue(bHide);
        seg->btnHideActor->stateChange();
        seg->saveCache();
        
        it++;
    }
}

void ActorsScene::resetPosAll(bool bReset)
{
    SegmentsIter it = mSegmentsMap.begin();
    
    while (it != mSegmentsMap.end())
    {
        BaseSegment *seg = it->second;
        seg->position = ofPoint::zero();
        seg->saveCache();
        
        it++;
    }
}

void ActorsScene::pauseAll(bool bPause)
{
    /// realtime osc data
    getActorManager().setFreezed(bPause);
    
    
    /// playback data
    SegmentsIter it = mSegmentsMap.begin();
    
    while (it != mSegmentsMap.end())
    {
        BaseSegment *seg = it->second;
        if (seg->getType() == RAM_UI_SEGMENT_TYPE_PLAYBACK)
        {
            static_cast<PlaybackSegment*>(seg)->pause(bPause);
        }
        it++;
    }
}

void ActorsScene::recAll(bool bStartRec)
{
    SegmentsIter it = mSegmentsMap.begin();
    
    while (it != mSegmentsMap.end())
    {
        
        if (it->second->getType() == RAM_UI_SEGMENT_TYPE_CONTROL)
        {
            ControlSegment *seg = (ControlSegment *)it->second;
            seg->toggleRecording(bStartRec);
        }
        
        // no need to saveCache on click this recording button
        it++;
    }
}

bool ActorsScene::getShowAll()
{
	return bShowAllActor;
}

#pragma mark -
#pragma mark private methods

void ActorsScene::addSegment(BaseSegment *newSegment)
{
    const string name = newSegment->getName();
    
    if (mSegmentsMap.find(name) != mSegmentsMap.end())
		return;
	
    mSegmentsMap.insert( make_pair(name, newSegment) );

	/// create and add child panel
	const int panelIndex = mSegmentsMap.size()-1;
	const int panelHeight = 192;
	const int panelHeaderHeight = 164;
	
	ofxUICanvasPlus* childPanel = newSegment->createPanel(name);
	childPanel->getRect()->x = GetGUI().kXInit;
	childPanel->getRect()->y = panelHeaderHeight + panelIndex * panelHeight;
	childPanel->getRect()->width = GetGUI().kLength;
	
    
	/// append widget, resize parent panel, load default settings
	mLocalPanel->addWidget(childPanel);
	mLocalPanel->autoSizeToFitWidgets();
	newSegment->loadCache();
}

void ActorsScene::removeControlSegment(const string& name)
{
    ActorManager::instance().removeNodeArray(name);
	
	SegmentsIter it = mSegmentsMap.find(name);
	
	if (it == mSegmentsMap.end())
		return;
	
	mSegmentsMap.erase(it);
	
    setNeedsUpdatePanel(true);
}

void ActorsScene::rebuildControlPanel()
{
	/// remove all widgets
	mLocalPanel->removeWidgets();
	mLocalPanel->resetPlacer();
	
	
	/// adding panel header
	createPanelHeader();
	
	
	/// insert panels
    map<string, BaseSegment*> tmpMap = mSegmentsMap;
    mSegmentsMap.clear();
    SegmentsIter it = tmpMap.begin();
    
    while (it != tmpMap.end())
    {
        BaseSegment *seg = it->second;
        
        if (seg->getType() == RAM_UI_SEGMENT_TYPE_CONTROL)
        {
            ControlSegment *s = new ControlSegment(seg->getName());
            addSegment(s);
        }
        else if (seg->getType() == RAM_UI_SEGMENT_TYPE_PLAYBACK)
        {
            PlaybackSegment *s = new PlaybackSegment(seg->getName());
            s->session = seg->session;
			s->session.prepareForPlay();
            s->session.play();
            addSegment(s);
        }
        
        it++;
    }
    
	mLocalPanel->autoSizeToFitWidgets();
	
	setNeedsUpdatePanel(false);
}

void ActorsScene::drawImGui()
{
#ifndef TARGET_WIN32
	if (ImGui::Button("Load Recorded File"))
	{
		ofFileDialogResult result = ofSystemLoadDialog("Load recorded *.tsv file.", false, "");
		if (result.bSuccess) {
			loadNewFile = true;
			newFilePath = result.getPath();
		}
	}
#endif
	ImGui::SameLine();
	if (ImGui::Button("Reset Positions")) resetPosAll(true);
	if (ImGui::Checkbox("Show All Actors", &bShowAllActor)) showAll(bShowAllActor);
	
	ImGui::Checkbox("Use Shading", &bUseShading);ImGui::SameLine();
	ImGui::Checkbox("Use SimpleActor", &bUseSimpleActor);

	if (ImGui::Checkbox("Pause (Space key)", &bPausing)) pauseAll(bPausing);
	ImGui::SameLine();
	if (ImGui::Checkbox("Recording All Actors", &bRec)) recAll(bRec);
	
	/// insert panels
	map<string, BaseSegment*> tmpMap = mSegmentsMap;
	SegmentsIter it = tmpMap.begin();
	
	while (it != tmpMap.end())
	{
		BaseSegment *seg = it->second;

		if (seg->getType() == RAM_UI_SEGMENT_TYPE_CONTROL)
		{
			seg->drawImGui();
		}
		else if (seg->getType() == RAM_UI_SEGMENT_TYPE_PLAYBACK)
		{
			seg->drawImGui();
		}
		
		it++;
	}
}


void ActorsScene::createPanelHeader()
{
	const int width = GetGUI().kLength/2 - 3;
	const int height = GetGUI().kDim * 1.3;
	
	mLocalPanel->addLabel(getName(), OFX_UI_FONT_LARGE);
	mLocalPanel->addSpacer(GetGUI().kLength, 2);
	
// ignore win32
#ifndef TARGET_WIN32
	mLocalPanel->addWidgetDown( new ofxUILabelButton("Load Recorded File", false, GetGUI().kLength, height) );
#endif
	
	/// 2x2 matrix
	btnShowAll = new ofxUILabelToggle("Show All Actors", &bShowAllActor, width, height);
	mLocalPanel->addWidgetDown( btnShowAll );
	mLocalPanel->addWidgetRight( new ofxUILabelButton("Reset Positions", &bRecAllActor, width, height) );
	mLocalPanel->addWidgetDown( new ofxUILabelToggle("Use Shading", &bUseShading, width, height) );
	mLocalPanel->addWidgetRight( new ofxUILabelToggle("Use SimpleActor", &bUseSimpleActor, width, height) );
	
	
	/// buttons which are controlled programatically
	//  all of the child widgets of mLocalPanel are deleted when rebuildControlPanel is executed
	//  so it needs to make new pointer
	btnPause = new ofxUILabelToggle("Pause (Space key)", false, GetGUI().kLength, height);
	btnRecAll = new ofxUILabelToggle("Recording All Actors", false, GetGUI().kLength, height);
	mLocalPanel->addWidgetDown(btnPause);
	mLocalPanel->addWidgetDown(btnRecAll);
}

void ActorsScene::setNeedsUpdatePanel(const bool needsUpdate)
{
    bNeedUpdatePanel = needsUpdate;
}

bool ActorsScene::needsUpdatePanel()
{
    return bNeedUpdatePanel;
}



#pragma mark -
#pragma mark experimental
void ActorsScene::drawNodes(const NodeArray &NA)
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
		
		const Node &node = NA.getNode(i);
		const Node *parent = node.getParent();
		if (parent == NULL) continue;
		
		Box(node, 2);
		
		parent->beginTransform();
		
		ofVec3f axis(0, 0, 1);
		ofVec3f c = node.getPosition().getNormalized().getCrossed(axis);
		
		ofRotate(90, c.x, c.y, c.z);
		
		ofVec3f p0 = node.getGlobalPosition();
		ofVec3f p1 = parent->getGlobalPosition();
		
		float dist = p0.distance(p1);
		float offset = 0.2;
		
		glNormal3f(0, 0, 0);
		ofDrawLine(ofVec3f(0), ofVec3f(0, 0, -dist));
		
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
