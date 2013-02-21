#pragma once

#include "ofxXmlSettings.h"
#include "ramNodeLine.h"

class LineDrawing : public ramBaseScene
{
	
public:
	
	const string getName() { return "Line"; }
	
	struct LineContext
	{
		bool set_from;
		bool set_control0;
		bool set_control1;
		bool set_to;
		
		float curve;
		
		float spiral_radius;
		float spiral_num_rotate;
		
		float noise_scale;
		float noise_freq;
		
		float extend_from;
		float extend_to;
		
		float line_width;
		ofFloatColor color;
		
		ramNodeLine nodeLine;
		
		bool active;
		int id;

		void setupControlPanel()
		{
			ramControlPanel &gui = ramGetGUI();
			
#ifdef RAM_GUI_SYSTEM_OFXUI
			
			ofxUICanvas* panel = gui.getCurrentUIContext();
			
			line_width = 2;
			
			active = false;
			panel->addToggle("Line " + ofToString(id), &active, 15, 15, 0, 0);
			
			panel->addButton("From", &set_from, 10, 10, 0, 0);
			panel->addButton("Control0", &set_control0, 10, 10, 0, 80);
			panel->addButton("Control1", &set_control1, 10, 10, 0, 160);
			panel->addButton("To", &set_to, 10, 10, 0, 240);
			
			panel->addSlider("Line R", 0, 1, &color.r, 95, 10);
			panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
			panel->addSlider("Line G", 0, 1, &color.g, 95, 10);
			panel->addSlider("Line B", 0, 1, &color.b, 95, 10);
			panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
			
			panel->addSlider("line_width", 1, 10, &line_width, 150, 10);
			panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
			panel->addSlider("curve", -400, 400, &curve, 150, 10);
			panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
			
			panel->addSlider("spiral_radius", 0, 200, &spiral_radius, 150, 10);
			panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
			panel->addSlider("spiral_num_rotate", 0, 100, &spiral_num_rotate, 150, 10);
			panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
			
			panel->addSlider("noise_scale", 0, 200, &noise_scale, 150, 10);
			panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
			panel->addSlider("noise_freq", 0, 10, &noise_freq, 150, 10);
			panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
			
			panel->addSlider("extend from", 0, 1000, &extend_from, 150, 10);
			panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_RIGHT);
			panel->addSlider("extend to", 0, 1000, &extend_to, 150, 10);
			panel->setWidgetPosition(OFX_UI_WIDGET_POSITION_DOWN);
			
			panel->addSpacer(gui.kLength, 2);
			
#endif
		}
		
		void update()
		{
			if (!active) return;
			
			if (set_from) nodeLine.from = ramActorManager::instance().getLastSelectedNodeIdentifer();
			if (set_control0) nodeLine.control0 = ramActorManager::instance().getLastSelectedNodeIdentifer();
			if (set_control1) nodeLine.control1 = ramActorManager::instance().getLastSelectedNodeIdentifer();
			if (set_to) nodeLine.to = ramActorManager::instance().getLastSelectedNodeIdentifer();
		}
		
		void draw()
		{
			if (!active) return;
			
			nodeLine.curve(curve);
			nodeLine.resampling(0.3);
			
			if (extend_from > 0)
			{
				nodeLine.extendFrom(extend_from);
			}
			
			if (extend_to > 0)
			{
				nodeLine.extendTo(extend_to);
			}
			
			if (spiral_radius > 0)
			{
				nodeLine.spiral(spiral_radius, spiral_num_rotate);
			}
			
			if (noise_scale > 0)
			{
				nodeLine.noise(noise_scale, noise_freq, ofGetElapsedTimef());
			}
			
			ofSetColor(color);
			
			ofSetLineWidth(line_width);
			nodeLine.draw();
			
			ofSetColor(255, 127);
			
			ramNode node;
			if (nodeLine.from.get(node))
			{
				ofDrawBitmapString("FROM", node.getGlobalPosition() + ofVec3f(5, 5, 0));
			}
			
			if (nodeLine.control0.get(node))
			{
				ofDrawBitmapString("CP0", node.getGlobalPosition() + ofVec3f(5, 5, 0));
			}
			
			if (nodeLine.control1.get(node))
			{
				ofDrawBitmapString("CP1", node.getGlobalPosition() + ofVec3f(5, 5, 0));
			}
			
			if (nodeLine.to.get(node))
			{
				ofDrawBitmapString("TO", node.getGlobalPosition() + ofVec3f(5, 5, 0));
			}
		}
		
		void randomize()
		{
			ramActorManager &AM = ramActorManager::instance();
			const vector<string>& names = AM.getNodeArrayNames();
			
			nodeLine.from.index = ofRandom(23);
			nodeLine.from.name = names[rand() % names.size()];
			
			nodeLine.control0.index = ofRandom(23);
			nodeLine.control0.name = names[rand() % names.size()];
			
			nodeLine.to.index = ofRandom(23);
			nodeLine.to.name = names[rand() % names.size()];
			
			nodeLine.control1.index = ofRandom(23);
			nodeLine.control1.name = names[rand() % names.size()];
		}
	};
	
	enum
	{
		NUM_LINE = 3
	};
	
	LineContext lines[NUM_LINE];
	float random_change_time;
	float last_changed_time;
	
	LineDrawing()
	{
	}
	
	void setupControlPanel()
	{
		ramControlPanel &gui = ramGetGUI();
		
		random_change_time = 60;
		gui.addSlider("random_change_time", 0.1, 60, &random_change_time);
		last_changed_time = ofGetElapsedTimef();
		
		for (int i = 0; i < NUM_LINE; i++)
		{
			lines[i].id = i;
			lines[i].setupControlPanel();
		}
		
		lines[0].active = true;
	}
	
	void setup()
	{
		ofAddListener(ofEvents().keyPressed, this, &LineDrawing::onKeyPressed);
	}
	
	void onKeyPressed(ofKeyEventArgs &e)
	{
		if (e.key == 'r')
		{
			for (int i = 0; i < NUM_LINE; i++)
			{
				lines[i].randomize();
			}
		}
		
		if (e.key == 'l')
		{
			loadXML();
		}
	}
	
	void update()
	{
		if (random_change_time < 60
			&& ofGetElapsedTimef() - last_changed_time > random_change_time)
		{
			last_changed_time = ofGetElapsedTimef();
			
			for (int i = 0; i < NUM_LINE; i++)
			{
				lines[i].randomize();
			}
		}
		
		for (int i = 0; i < NUM_LINE; i++)
		{
			lines[i].update();
		}
	}
	
	void draw()
	{
		ramBeginCamera();
		
		ofPushStyle();
		for (int i = 0; i < NUM_LINE; i++)
		{
			lines[i].draw();
		}
		ofPopStyle();
		
		ramEndCamera();
	}

	void loadXML()
	{
		string fileName = "Lines.xml";
		

		if (!ofFile::doesFileExist(fileName))
		{
			#define _S(src) #src
			string default_xml = _S(<line>
							<from><name>Yoko</name><id>1</id></from>
							<control1><name>Yoko</name><id>2</id></control1>
							<control2><name>Yoko</name><id>13</id></control2>
							<to><name>Yoko</name><id>20</id></to>
							</line>
							<line>
							<from><name>Yoko</name><id>11</id></from>
							<control1><name>Yoko</name><id>2</id></control1>
							<control2><name>Yoko</name><id>22</id></control2>
							<to><name>Yoko</name><id>3</id></to>
							</line>
							<line>
							<from><name>Yoko</name><id>5</id></from>
							<control1><name>Yoko</name><id>18</id></control1>
							<control2><name>Yoko</name><id>15</id></control2>
							<to><name>Yoko</name><id>1</id></to>
							</line>
			);
	
			#undef _S

			ofBuffer buf(default_xml);
			ofBufferToFile(fileName, buf);
		}
		
		ofxXmlSettings XML;
		XML.loadFile(fileName);
		
		int n = XML.getNumTags("line");
		for (int i = 0; i < n; i++)
		{
			if (i >= NUM_LINE) return;
			
			XML.pushTag("line", i);
			
			/// nodes
			const string from_name	= XML.getValue("from:name", "Yoko");
			const int	 from_id	= XML.getValue("from:id", ramActor::JOINT_RIGHT_HAND);
			
			const string cp0_name	= XML.getValue("control1:name", "Yoko");
			const int	 cp0_id		= XML.getValue("control1:id", ramActor::JOINT_RIGHT_TOE);
			
			const string cp1_name	= XML.getValue("control2:name", "Yoko");
			const int	 cp1_id		= XML.getValue("control2:id", ramActor::JOINT_LEFT_TOE);
			
			const string to_name	= XML.getValue("to:name", "Yoko");
			const int	 to_id		= XML.getValue("to:id", ramActor::JOINT_LEFT_HAND);
			
			/// curve
			const float curve		= XML.getValue("param:curve", 10);
			
			/// spiral
			const float radius		= XML.getValue("param:radius", 10);
			const float num_rotate	= XML.getValue("param:num_rotate", 10);
			const float noise		= XML.getValue("param:scale", 1);
			const float freq		= XML.getValue("param:freq", 10);
			
			/// extend length
			const float ex_from		= XML.getValue("param:extend_from", 10);
			const float ex_to		= XML.getValue("param:extend_to", 10);
			
			/// line styling
			const float line_width = XML.getValue("param:line_width", 2);
			const float color		= XML.getValue("param:color", 1.0);
			
			
			LineContext &line = lines[i];
			line.active = true;
			line.nodeLine.from = ramNodeIdentifer(from_name, from_id);
			line.nodeLine.control0 = ramNodeIdentifer(cp0_name, cp0_id);
			line.nodeLine.control1 = ramNodeIdentifer(cp1_name, cp1_id);
			line.nodeLine.to = ramNodeIdentifer(to_name, to_id);
			line.curve = curve;
			line.spiral_radius = radius;
			line.spiral_num_rotate = num_rotate;
			line.noise_scale = noise;
			line.noise_freq = freq;
			line.extend_from = ex_from;
			line.extend_to = ex_to;
			line.line_width = line_width;
			line.color = color;
			
			XML.popTag();
		}
	}
};

