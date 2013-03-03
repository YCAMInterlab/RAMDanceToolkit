#pragma once

#include "ramSession.h"
#include "ramTSVCoder.h"

/*
 almost all things of this class depends on ofxUI
*/
class ramActorsScene : public ramBaseScene
{
	
public:
	
	ramActorsScene() : mNeedUpdatePanel(false) {}
	
	void setupControlPanel()
	{
		mLocalPanel = gui().getCurrentUIContext();
	}
	
	void setup()
	{
		fontSize = 30;
		font.loadFont(ramToResourcePath("Fonts/NewMedia Fett.ttf"), fontSize, true, true);
		font.setLineHeight(fontSize*1.4f);
		font.setLetterSpacing(1.0);
	}
	
	void update()
	{
		if (mNeedUpdatePanel)
		{
			rebuildControlPanel();
		}
		
		SegmentsIter it = mSegmentsMap.begin();
		
		while (it != mSegmentsMap.end())
		{
			ControlSegment *seg = it->second;
			
			if (seg->bNeedsResetPos)
			{
				seg->position = ofPoint::zero();
				seg->bNeedsResetPos = false;
			}
			
			it++;
		}
	}
	
	void draw()
	{
		bool bRecording = false;
		
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
			
			
			// recording indicator
			if (seg->bRecording)
			{
				bRecording = true;
			}
		}
		
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
	
	void onValueChanged(ofxUIEventArgs& e)
	{
		string name = e.widget->getName();
	}
	
	void addControlSegment(const ramNodeArray &NA)
	{
		const string name = NA.getName();
		
		if (mSegmentsMap.find(name) != mSegmentsMap.end()) return;
		
		ControlSegment *seg = new ControlSegment();
		seg->setup(mLocalPanel, NA);
		
		mLocalPanel->autoSizeToFitWidgets();
		
		mSegmentsMap.insert( make_pair(name, seg) );
	}
	
	void removeControlSegment(const ramNodeArray &NA)
	{
		const string key = NA.getName();
		
		if (mSegmentsMap.find(key) == mSegmentsMap.end()) return;
		
		mNeedUpdatePanel = true;
	}
	
	void rebuildControlPanel()
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
	
	void onActorSetup(const ramActor &actor)
	{
		addControlSegment(actor);
	}
	
	void onRigidSetup(const ramRigidBody &rigid)
	{
		addControlSegment(rigid);
	}
	
	void onActorExit(const ramActor &actor)
	{
		removeControlSegment(actor);
	}
	
	void onRigidExit(const ramRigidBody &rigid)
	{
		removeControlSegment(rigid);
	}
	
	void onEnabled()
	{
		cout << "[Scene enabled] " <<  getName() << endl;
		
		if (getNumNodeArray() != mSegmentsMap.size())
			rebuildControlPanel();
	}
	
	const string getName() { return "Actors"; }
	
	
private:
	
	
	class ControlSegment
	{
		
	public:
		
		ramSession session;
		ofFloatColor jointColor;
		ofPoint position;
		bool bHideActor;
		bool bNeedsResetPos;
		bool bRecording;
		
		
		ControlSegment() { reset(); }
		

		void setup(ofxUICanvasPlus* panel, const ramNodeArray &NA)
		{
//			ofxUICanvas *panel = new ofxUICanvas(0.0f, 0.0f, 200, 200);
//			parent->addWidget( panel );
//			panel->disableAppDrawCallback();
//			panel->disableAppEventCallbacks();
			panel->disableKeyEventCallbacks();
			panel->disableMouseEventCallbacks();
			
			
			const float margin = ramGetGUI().kXInit;
			const float width = ramGetGUI().kLength;
			const float height = ramGetGUI().kDim+3;
			
			
			/// section title
			panel->addWidgetDown(new ofxUILabel(NA.getName(), OFX_UI_FONT_MEDIUM));
			panel->addSpacer(width, 2);

			
			// Icons
			panel->addWidgetDown(new ofxUIImageToggle(32, 32, &bHideActor, ramToResourcePath("Images/show.png"),"show"));
			panel->addWidgetRight(new ofxUIImageButton(32, 32, &bNeedsResetPos, ramToResourcePath("Images/reset.png"),"reset"));
			panel->addWidgetRight(new ofxUIImageToggle(32, 32, &bRecording, ramToResourcePath("Images/record.png"),"record"));

			
			/// actor color
			panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
			panel->addSlider("R", 0, 1, &jointColor.r, 60, height);
			panel->addSlider("G", 0, 1, &jointColor.g, 60, height);
			panel->addSlider("B", 0, 1, &jointColor.b, 60, height);
			panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
			
			
			/// actor position
			panel->add2DPad("Position",
							ofPoint(-500, 500),
							ofPoint(-500, 500),
							&position,
							width, 100);
		}
		
		void exit(ofxUICanvas* panel, const ramNodeArray &NA)
		{
			ofxUIWidget *widget = panel->getWidget(NA.getName());
			panel->removeWidget(widget);
		}
		
		void reset()
		{
			bHideActor = false;
			bNeedsResetPos = false;
			bRecording = false;
			
			jointColor = ofFloatColor(1.0, 0.15, 0.4);
			position = ofPoint(0, 0);
		}
	};
	
	
	ofxUICanvasPlus *mLocalPanel;
	
	bool mNeedUpdatePanel;
	
	map<string, ControlSegment*> mSegmentsMap;
	typedef map<string, ControlSegment*>::iterator SegmentsIter;
	
	ramTSVCoder coder;
	
	ofTrueTypeFont font;
	float fontSize;
};



