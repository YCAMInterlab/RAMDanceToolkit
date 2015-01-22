// 
// LineDrawing.h - RAMDanceToolkit
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

#pragma once

#include "ofxXmlSettings.h"
#include "ramNodeLine.h"

class LineDrawing : public ramBaseScene
{
	
public:
	
	string getName() const { return "Line"; }
	
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
        ofxUIToggle *toggle;
		
		bool active;
		int id;
        
		void setupControlPanel()
		{
			
#ifdef RAM_GUI_SYSTEM_OFXUI
			
			ofxUICanvas* panel = ramGetGUI().getCurrentUIContext();
			
			line_width = 2;
			
			active = true;
            toggle = new ofxUIToggle("Line " + ofToString(id), &active, 30, 30, 0, 0);
			panel->addWidgetDown(toggle);
			
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
			
			panel->addSpacer(ramGetGUI().kLength, 2);
			
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
			if (nodeLine.from.findOne(node))
			{
				ofDrawBitmapString("FROM", node.getGlobalPosition() + ofVec3f(5, 5, 0));
			}
			
			if (nodeLine.control0.findOne(node))
			{
				ofDrawBitmapString("CP0", node.getGlobalPosition() + ofVec3f(5, 5, 0));
			}
			
			if (nodeLine.control1.findOne(node))
			{
				ofDrawBitmapString("CP1", node.getGlobalPosition() + ofVec3f(5, 5, 0));
			}
			
			if (nodeLine.to.findOne(node))
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
		ramGetGUI().getCurrentUIContext()->addLabelButton("Load Line Settings", false, ramGetGUI().kLength);
		ramGetGUI().getCurrentUIContext()->addLabelButton("Save Line Settings", false, ramGetGUI().kLength);
        ramGetGUI().getCurrentUIContext()->addSpacer(ramGetGUI().kLength, 2);
        
		random_change_time = 60;
        ramGetGUI().addButton("Randomize");
		ramGetGUI().addSlider("Auto Random change time", 0.0, 60, &random_change_time);
        ramGetGUI().getCurrentUIContext()->addSpacer();
        
		last_changed_time = ofGetElapsedTimef();
		
		for (int i = 0; i < NUM_LINE; i++)
		{
			lines[i].id = i;
			lines[i].setupControlPanel();
		}
        
        ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &LineDrawing::onValueChanged);
	}
	
	void setup()
	{
        loadXML();
	}
    
    void onValueChanged(ofxUIEventArgs &e)
    {
        const string name = e.widget->getName();
        
        if (name == "Randomize")
        {
            ofxUIButton *button = (ofxUIButton *)e.widget;
            if (button->getValue())
            {
                for (int i = 0; i < NUM_LINE; i++) lines[i].randomize();
            }
        }
        
        if (name == "Load Line Settings")
        {
            ofxUIButton *button = (ofxUIButton *)e.widget;
            if (button->getValue())
            {
                ofFileDialogResult result = ofSystemLoadDialog("Load Line Settings.", false, "Lines.xml");
                if (result.bSuccess)
                    loadXML(result.getPath());
                
                for (int i=0; i<NUM_LINE; i++)
                {
                    bool active;
                    
                    LineContext &line = lines[i];
                    XML.pushTag("line", i);
                    active = XML.getValue("active", true);
                    XML.popTag();
                    
                    line.toggle->setValue(active);
                    line.toggle->stateChange();
                }
            }
        }
        
        if (name == "Save Line Settings")
        {
            ofxUIButton *button = (ofxUIButton *)e.widget;
            if (button->getValue())
            {
                ofFileDialogResult result = ofSystemSaveDialog("Lines.xml", "Save Line Settings.");
                if (result.bSuccess)
                    saveXML(result.getPath());
            }
        }
        
        saveXML();
    }
	
	void update()
	{
        
		if (random_change_time < 60
			&& ofGetElapsedTimef() - last_changed_time > random_change_time
            && random_change_time != 0.0)
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
    
    void exit()
    {
        
    }
    
	void saveXML(string fileName = "Lines.xml")
    {
        
        XML.clear();
        
        for (int i=0; i<NUM_LINE; i++)
        {
            LineContext &line = lines[i];
            
            XML.addTag("line");
            XML.pushTag("line", i);
            {
                
                // targets
                XML.setValue("from:name", line.nodeLine.from.name);
                XML.setValue("from:id", line.nodeLine.from.index);
                
                XML.setValue("control0:name", line.nodeLine.control0.name);
                XML.setValue("control0:id", line.nodeLine.control0.index);
                
                XML.setValue("control1:name", line.nodeLine.control1.name);
                XML.setValue("control1:id", line.nodeLine.control1.index);
                
                XML.setValue("to:name", line.nodeLine.to.name);
                XML.setValue("to:id", line.nodeLine.to.index);
                
                // styling
                XML.setValue("param:curve", line.curve);
                
                XML.setValue("param:radius", line.spiral_radius);
                XML.setValue("param:num_rotate", line.spiral_num_rotate);
                XML.setValue("param:scale", line.noise_scale);
                XML.setValue("param:freq", line.noise_freq);
                
                XML.setValue("param:extend_from", line.extend_from);
                XML.setValue("param:extend_to", line.extend_to);
                
                XML.setValue("param:line_width", line.line_width);
                XML.setValue("param:color:r", line.color.r);
                XML.setValue("param:color:g", line.color.g);
                XML.setValue("param:color:b", line.color.b);
                
                XML.setValue("param:active", line.active);
            }
            XML.popTag();
            XML.saveFile(fileName);
        }
    }
    
	void loadXML(string fileName = ofToDataPath("Lines.xml", true))
	{
		if (!ofFile::doesFileExist(fileName))
		{
			fileName = ramToResourcePath("Settings/presets/preset.lines.xml");
		}
		
		XML.loadFile(fileName);
		
		int n = XML.getNumTags("line");
		for (int i = 0; i < n; i++)
		{
			if (i >= NUM_LINE) return;
			
			XML.pushTag("line", i);
			
			/// nodes
			const string from_name	= XML.getValue("from:name", "Yoko");
			const int	 from_id	= XML.getValue("from:id", ramActor::JOINT_RIGHT_HAND);
			
			const string cp0_name	= XML.getValue("control0:name", "Yoko");
			const int	 cp0_id		= XML.getValue("control0:id", ramActor::JOINT_RIGHT_TOE);
			
			const string cp1_name	= XML.getValue("control1:name", "Yoko");
			const int	 cp1_id		= XML.getValue("control1:id", ramActor::JOINT_LEFT_TOE);
			
			const string to_name	= XML.getValue("to:name", "Yoko");
			const int	 to_id		= XML.getValue("to:id", ramActor::JOINT_LEFT_HAND);
			
			/// curve
			const float curve		= XML.getValue("param:curve", 10);
			
			/// spiral
			const float radius		= XML.getValue("param:radius", 10);
			const float num_rotate	= XML.getValue("param:num_rotate", 10);
			const float noise		= XML.getValue("param:scale", 0);
			const float freq		= XML.getValue("param:freq", 0);
			
			/// extend length
			const float ex_from		= XML.getValue("param:extend_from", 10);
			const float ex_to		= XML.getValue("param:extend_to", 10);
			
			/// line styling
			const float line_width = XML.getValue("param:line_width", 2);
			const float colorR		= XML.getValue("param:color:r", 0.5);
			const float colorG		= XML.getValue("param:color:g", 0.5);
			const float colorB		= XML.getValue("param:color:b", 0.5);
            
			const bool active		= XML.getValue("param:active", 1);
			
			
			LineContext &line = lines[i];
			line.active = active;
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
			line.color.r = colorR;
			line.color.g = colorG;
			line.color.b = colorB;
			
			XML.popTag();
		}
	}
    
	void loadPresetXML(string filePath)
	{
		loadXML(filePath);
	}
	
private:
    ofxXmlSettings XML;
};

