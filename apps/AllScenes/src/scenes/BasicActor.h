#pragma once

class BasicActor : public ramBaseScene
{
	
	struct ControlSegment
	{
		string name;
		ofVec3f translate;
		float scale;
		
		void reset()
		{
			translate = ofVec3f::zero();
			scale = 1.0;
		}
	};
	
	map<const string, ControlSegment> mControlSegments;
	ofxUICanvas *panel;
	
public:
	
	BasicActor() {}
	
	void setupControlPanel(ofxUICanvas* panel_)
	{
		panel = panel_;
		ofAddListener(panel->newGUIEvent, this, &BasicActor::onPanelChanged);
	}
	
	void setup()
	{
		mControlSegments.clear();
	}
	
	void addEntityControl(const ramActor &actor)
	{
		ramControlPanel &gui = ramGetGUI();
		
		const string &name = actor.getName();
		
		mControlSegments.insert( make_pair(name, ControlSegment()) );
		ControlSegment &segment = mControlSegments[name];
		
		segment.reset();
		segment.name = actor.getName();
		
		panel->addSpacer(gui.kLength, 2);
		panel->addWidgetDown(new ofxUILabel(segment.name, OFX_UI_FONT_LARGE));
		panel->addSpacer(gui.kLength, 2);
		panel->addButton("Reset: " + segment.name, false, 40, 40);
		panel->addSlider(segment.name + " Translate: X", -600.0, 600.0, &segment.translate.x, gui.kLength, gui.kDim);
		panel->addSlider(segment.name + " Translate: Y", -600.0, 600.0, &segment.translate.y, gui.kLength, gui.kDim);
		panel->addSlider(segment.name + " Translate: Z", -600.0, 600.0, &segment.translate.z, gui.kLength, gui.kDim);
		panel->addSlider(segment.name + " Scale", 0.01, 5.0, &segment.scale, gui.kLength, gui.kDim);
	}
	
	void update()
	{
		int numSegments = mControlSegments.size();
		int numActor = getNumNodeArray();
		
		if (numSegments < numActor)
		{
			for (int i=numSegments; i<numActor; i++)
			{
				const ramNodeArray copy = getNodeArray(i);
				addEntityControl( copy );
			}
		}
	}
	
	void draw()
	{
		
	}
	
	void drawActor( ramActor& actor )
	{
		ControlSegment &segment = mControlSegments[actor.getName()];
		
		glPushMatrix();
		{
			glTranslatef(segment.translate.x, segment.translate.y, segment.translate.z);
			glScalef(segment.scale, segment.scale, segment.scale);
			ramDrawBasicActor(actor);
		}
		glPopMatrix();
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
		
	}
	
	void onPanelChanged(ofxUIEventArgs& e)
	{
		string name = e.widget->getName();
		
		
		map <const string, ControlSegment>::iterator it = mControlSegments.begin();
		while( it != mControlSegments.end() )
		{
			ControlSegment &segment = (*it).second;
			
			if (name == ("Reset: "+segment.name)) segment.reset();
			
			++it;
		}
		
	}
	
	const string getName() { return "Basic Actor"; }
};