#include "ramActorsScene.h"


#pragma mark -
#pragma mark constructor

ramActorsScene::ramActorsScene() {}



#pragma mark -
#pragma mark called from ramSceneManager

const string ramActorsScene::getName()
{
	return "Actors";
}

void ramActorsScene::setupControlPanel()
{
	mLocalPanel = gui().getCurrentUIContext();
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
				ofSetColor(seg->jointColor);
				ofTranslate(seg->position.x, 0, seg->position.y);
				
				if (bUseNewActor)
					drawNodes(NA);
				else
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
#pragma mark Events fired from SceneManager

void ramActorsScene::onEnabled()
{
	if (getNumNodeArray() != mSegmentsMap.size())
		rebuildControlPanel();
}
	


#pragma mark -
#pragma mark private methods

void ramActorsScene::addControlSegment(const ramNodeArray &NA)
{
	const string name = NA.getName();
	
	if (mSegmentsMap.find(name) != mSegmentsMap.end()) return;
	
	ControlSegment *seg = new ControlSegment();
	mSegmentsMap.insert( make_pair(name, seg) );
	
	ofxUICanvasPlus* childPanel = seg->setup(mLocalPanel, NA);
	childPanel->getRect()->y = (mSegmentsMap.size()-1) * 210;
	
	mLocalPanel->autoSizeToFitWidgets();
}

void ramActorsScene::removeControlSegment(const ramNodeArray &NA)
{
	const string key = NA.getName();
	
	if (mSegmentsMap.find(key) == mSegmentsMap.end()) return;
	
	mNeedUpdatePanel = true;
}

void ramActorsScene::rebuildControlPanel()
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