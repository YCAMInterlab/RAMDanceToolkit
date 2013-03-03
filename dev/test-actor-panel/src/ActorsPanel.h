#pragma once


/*
 almost all things of this class depends on ofxUI
*/
class ActorsPanel : public ramBaseScene
{
	
public:
	
	ActorsPanel() : mNeedUpdatePanel(false) {}
	
	void setupControlPanel()
	{
		mLocalPanel = gui().getCurrentUIContext();
	}
	
	void setup()
	{
		
	}
	
	void update()
	{
		if (mNeedUpdatePanel)
			rebuildControlPanel();
	}
	
	void draw()
	{
		
		ramBeginCamera();
		
		for(int i=0; i<getNumNodeArray(); i++)
		{
			ramNodeArray &NA = getNodeArray(i);
			
			SegmentsIter it = mSegmentsMap.find(NA.getName());
			
			assert(it != mSegmentsMap.end());
			
			ControlSegment *seg = it->second;
			
			
			/// draw if "Show actor" toggle is anabled
			// note that ofxUIImageToggle shows hilighted image when it's false,
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
			
			
			/// capture the actor if "Start recording" toggle is anabled

		}
		
		ramEndCamera();
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
	
	const string getName() { return "ActorsPanel"; }
	
	
private:
	
	
	class ControlSegment
	{
		
	public:
		
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

			
			/// actor color
			panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
			panel->addSlider("R", 0, 1, &jointColor.r, 75, height);
			panel->addSlider("G", 0, 1, &jointColor.g, 75, height);
			panel->addSlider("B", 0, 1, &jointColor.b, 75, height);
			panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
			
			
			// recording
			panel->addLabelToggle("Start Recording", &bRecording, width);
			
			
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
	
};









