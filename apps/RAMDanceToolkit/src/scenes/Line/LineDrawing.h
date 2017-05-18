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

class LineDrawing : public rdtk::BaseScene
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
		
		rdtk::NodeLine nodeLine;
        ofxUIToggle *toggle;
		
		bool active;
		int id;
		
		void drawImGui()
		{
			ImGui::BeginChild(("win"+ofToString(id)).c_str(), ImVec2(400, 170),
							  false, ImGuiWindowFlags_AlwaysAutoResize);
			
			ImGui::Checkbox(("Line " + ofToString(id)).c_str(), &active);
			ImGui::Button("From");ImGui::SameLine();
			ImGui::Button("Control0");ImGui::SameLine();
			ImGui::Button("Control1");ImGui::SameLine();
			ImGui::Button("To");
			
			ImGui::ColorEdit3("Color", &color[0]);
			
			
			ImGui::Columns(2, NULL, true);
			
			ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.2f);
			ImGui::DragFloat("Width", &line_width, 0.1, 1, 10);
			ImGui::PopItemWidth();
			ImGui::NextColumn();
			
			ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.2f);
			ImGui::DragFloat("curve", &curve, 1, -400, 400);
			ImGui::PopItemWidth();
			ImGui::NextColumn();
			
			ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.2f);
			ImGui::DragFloat("spiral_radius", &spiral_radius,			1, 0, 200);
			ImGui::PopItemWidth();
			ImGui::NextColumn();
			
			ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.2f);
			ImGui::DragFloat("spiral_num_rotate", &spiral_num_rotate,	1, 0, 100);
			ImGui::PopItemWidth();
			ImGui::NextColumn();

			ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.2f);
			ImGui::DragFloat("noise_scale", &noise_scale,				1, 0, 200);
			ImGui::PopItemWidth();
			ImGui::NextColumn();

			ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.2f);
			ImGui::DragFloat("noise_freq", &noise_freq,					1, 0, 10);
			ImGui::PopItemWidth();
			ImGui::NextColumn();

			ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.2f);
			ImGui::DragFloat("extend from", &extend_from,				1, 0, 1000);
			ImGui::PopItemWidth();
			ImGui::NextColumn();

			ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.2f);
			ImGui::DragFloat("extend to", &extend_to,					1, 0, 1000);
			ImGui::PopItemWidth();
			ImGui::NextColumn();

			ImGui::Columns(1);
			ImGui::EndChild();
			ImGui::Spacing();
			
		}
		
		void setupControlPanel()
		{
			
#ifdef RAM_GUI_SYSTEM_OFXUI
			
			ofxUICanvasPlus* panel = rdtk::GetGUI().getCurrentUIContext();
			
			line_width = 2;
			
			set_from = false;
			set_control0 = false;
			set_control1 = false;
			set_to = false;
		
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
			
			panel->addSpacer(rdtk::GetGUI().kLength, 2);
			
#endif
		}
		
		void update()
		{
			if (!active) return;
			
			if (set_from) nodeLine.from = rdtk::ActorManager::instance().getLastSelectedNodeIdentifer();
			if (set_control0) nodeLine.control0 = rdtk::ActorManager::instance().getLastSelectedNodeIdentifer();
			if (set_control1) nodeLine.control1 = rdtk::ActorManager::instance().getLastSelectedNodeIdentifer();
			if (set_to) nodeLine.to = rdtk::ActorManager::instance().getLastSelectedNodeIdentifer();
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
			
			rdtk::Node node;
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
			rdtk::ActorManager &AM = rdtk::ActorManager::instance();
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
	
	void drawImGui()
	{
		
		if (ImGui::Button("Load Line Settings"))
			loadLineSettings();
		ImGui::SameLine();
		if (ImGui::Button("Save Line Settings"))
			saveLineSettings();
		
		if (ImGui::Button("Randomize"))
			for (int i = 0; i < NUM_LINE; i++) lines[i].randomize();
		
		ImGui::SliderFloat("Auto Random change time", &random_change_time, 0.0, 60.0);
		
		for (int i = 0; i < NUM_LINE; i++)
		{
			ImGui::PushID(ofToString(i).c_str());
			lines[i].drawImGui();
			ImGui::PopID();
		}
	}
	
	void setupControlPanel()
	{
		rdtk::GetGUI().getCurrentUIContext()->addLabelButton("Load Line Settings", false, rdtk::GetGUI().kLength);
		rdtk::GetGUI().getCurrentUIContext()->addLabelButton("Save Line Settings", false, rdtk::GetGUI().kLength);
        rdtk::GetGUI().getCurrentUIContext()->addSpacer(rdtk::GetGUI().kLength, 2);
        
		random_change_time = 60;
        rdtk::GetGUI().addButton("Randomize");
		rdtk::GetGUI().addSlider("Auto Random change time", 0.0, 60, &random_change_time);
        rdtk::GetGUI().getCurrentUIContext()->addSpacer();
        
		last_changed_time = ofGetElapsedTimef();
		
		for (int i = 0; i < NUM_LINE; i++)
		{
			lines[i].id = i;
			lines[i].setupControlPanel();
		}
        
        ofAddListener(rdtk::GetGUI().getCurrentUIContext()->newGUIEvent, this, &LineDrawing::onValueChanged);
	}
	
	void setup()
	{
        bool set_from = false;
        bool set_control0 = false;
        bool set_control1 = false;
        bool set_to = false;
        
        loadXML();
	}
    
    void onValueChanged(ofxUIEventArgs &e)
    {
        const string name = e.widget->getName();
        
        if (name == "Randomize")
        {
            ofxUIButton *button = (ofxUIButton *)e.widget;
            if (button->getValue())
                for (int i = 0; i < NUM_LINE; i++) lines[i].randomize();
        }
        
        if (name == "Load Line Settings")
        {
            ofxUIButton *button = (ofxUIButton *)e.widget;
            if (button->getValue()) loadLineSettings();
        }
        
        if (name == "Save Line Settings")
        {
            ofxUIButton *button = (ofxUIButton *)e.widget;
            if (button->getValue()) saveLineSettings();
        }
        
        saveXML();
    }
	
	void loadLineSettings()
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
	
	void saveLineSettings()
	{
		ofFileDialogResult result = ofSystemSaveDialog("Lines.xml", "Save Line Settings.");
		if (result.bSuccess)
			saveXML(result.getPath());
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
		rdtk::BeginCamera();
		
		ofPushStyle();
		for (int i = 0; i < NUM_LINE; i++)
		{
			lines[i].draw();
		}
		ofPopStyle();
		
		rdtk::EndCamera();
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
			fileName = rdtk::ToResourcePath("Settings/presets/preset.lines.xml");
		}
		
		XML.loadFile(fileName);
		
		int n = XML.getNumTags("line");
		for (int i = 0; i < n; i++)
		{
			if (i >= NUM_LINE) return;
			
			XML.pushTag("line", i);
			
			/// nodes
			const string from_name	= XML.getValue("from:name", "Yoko");
			const int	 from_id	= XML.getValue("from:id", rdtk::Actor::JOINT_RIGHT_HAND);
			
			const string cp0_name	= XML.getValue("control0:name", "Yoko");
			const int	 cp0_id		= XML.getValue("control0:id", rdtk::Actor::JOINT_RIGHT_TOE);
			
			const string cp1_name	= XML.getValue("control1:name", "Yoko");
			const int	 cp1_id		= XML.getValue("control1:id", rdtk::Actor::JOINT_LEFT_TOE);
			
			const string to_name	= XML.getValue("to:name", "Yoko");
			const int	 to_id		= XML.getValue("to:id", rdtk::Actor::JOINT_LEFT_HAND);
			
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
			line.nodeLine.from = rdtk::NodeIdentifer(from_name, from_id);
			line.nodeLine.control0 = rdtk::NodeIdentifer(cp0_name, cp0_id);
			line.nodeLine.control1 = rdtk::NodeIdentifer(cp1_name, cp1_id);
			line.nodeLine.to = rdtk::NodeIdentifer(to_name, to_id);
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

