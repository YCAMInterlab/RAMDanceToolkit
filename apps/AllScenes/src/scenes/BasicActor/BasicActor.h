#pragma once

class BasicActor : public ramBaseScene
{
	
	struct ControlSegment
	{
		string name;
		ofVec3f translate;
		float scale;
		float r,g,b;
		
		void reset()
		{
			translate = ofVec3f::zero();
			scale = 1.0;
			r = ofRandom(0, 255);
			g = ofRandom(0, 255);
			b = ofRandom(0, 255);
		}
	};
	
	map<const string, ControlSegment> mControlSegments;
	ofxUICanvas *panel;
	
	ofLight light;
	bool enableLight;
	
public:
	
	BasicActor() {}
	
	void setupControlPanel(ofxUICanvas* panel_)
	{
		panel = panel_;
		
		ramControlPanel &gui = ramGetGUI();
		gui.addToggle("Enable light", &enableLight);
		
		ofAddListener(panel->newGUIEvent, this, &BasicActor::onPanelChanged);
	}
	
	void setup()
	{
		enableLight = true;
		light.setPosition(300, 600, 300);
		
		mControlSegments.clear();
	}
	
	void addEntityControl(const ramNodeArray &nodeArray)
	{
		ramControlPanel &gui = ramGetGUI();
		
		const string &name = nodeArray.getName();
		
		mControlSegments.insert( make_pair(name, ControlSegment()) );
		ControlSegment &segment = mControlSegments[name];
		
		segment.reset();
		segment.name = nodeArray.getName();
		
		panel->addSpacer(gui.kLength, 2);
		panel->addWidgetDown(new ofxUILabel(segment.name, OFX_UI_FONT_LARGE));
		panel->addSpacer(gui.kLength, 2);
		panel->addButton("Reset: " + segment.name, false, 20, 20);
		
		
		panel->addSlider(nodeArray.getName() + " Joint R", 0, 255, &segment.r, 95, gui.kDim);
		panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
		panel->addSlider(nodeArray.getName() + " Joint G", 0, 255, &segment.g, 95, gui.kDim);
		panel->addSlider(nodeArray.getName() + " Joint B", 0, 255, &segment.b, 95, gui.kDim);
		panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
		
		panel->addSlider(segment.name + " X", -600.0, 600.0, &segment.translate.x, 95, gui.kDim);
		panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
		panel->addSlider(segment.name + " Y", -600.0, 600.0, &segment.translate.y, 95, gui.kDim);
		panel->addSlider(segment.name + " Z", -600.0, 600.0, &segment.translate.z, 95, gui.kDim);
		panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
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
		if (enableLight) light.enable();
		
		ramBeginCamera();
		for (int i=0; i<getNumNodeArray(); i++)
		{
			ramNodeArray &array = getNodeArray(i);
			
			ControlSegment &segment = mControlSegments[array.getName()];
			
			glPushMatrix();
			{
				glTranslatef(segment.translate.x, segment.translate.y, segment.translate.z);
				glScalef(segment.scale, segment.scale, segment.scale);
				ramDrawNodes(array, ofColor(segment.r, segment.g, segment.b));
			}
			glPopMatrix();
		}
		ramEndCamera();
		
		if (enableLight) light.disable();
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