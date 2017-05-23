// 
// SoundCube.h - RAMDanceToolkit
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

#include "ramMain.h"

class SoundCube : public rdtk::BaseScene
{
public:
	
	class Shape
	{
	public:
		
		ofColor color;
		ofSoundPlayer *player;
		bool trigger_mode;
		bool visible;
		
		rdtk::CollisionEvent event;
		
		Shape() : id(-1), obj(NULL), alpha(0), player(NULL), trigger_mode(false), visible(true) {}
		
		~Shape()
		{
			if (obj)
			{
				delete obj;
			}
			
			if (player)
			{
				player->stop();
				delete player;
			}
		}
		
		void set(int id, rdtk::Primitive *obj)
		{
			this->id = id;
			this->obj = obj;
			
			event.setPrimitive(obj);
		}
		
		void loadSound(const string &path, bool trigger = false, bool loop = true)
		{
			if (player) delete player;
			
			player = new ofSoundPlayer;
			player->load(path);
			player->setLoop(loop ? OF_LOOP_NORMAL : OF_LOOP_NONE);
			trigger_mode = trigger;
			
			volume = volume_t = 0;
			
			player->play();
			
			event.setTrigger(rdtk::RAM_TRIGGER_BOTH);
		}
		
		void draw(float fade = 0.1)
		{
			if (event.update())
			{
				if (event.getState())
				{
					if (trigger_mode) volume_t = 1;
					else volume_t = 0;
				}
				else
				{
					if (!trigger_mode) volume_t = 1;
					else volume_t = 0;
				}
			}
			
			ofPushStyle();
			
			if (event.getState())
			{
				alpha += (0 - alpha) * 0.1;
			}
			else
			{
				alpha += (1 - alpha) * 0.1;
			}
			
			volume += (volume_t - volume) * fade;
			player->setVolume(volume);
			
			if(visible)
			{
				ofSetColor(color, 127 + 127 * (1 - alpha));
				
				obj->transformGL();
				ofDrawBitmapString(ofToString(id), 0, 0);
				obj->restoreTransformGL();
				
				obj->draw();
			}
			
			ofPopStyle();
		}
		
	private:
		
		int id;
		rdtk::Primitive *obj;
		
		float alpha;
		float volume, volume_t;
	};
	
	string getName() const { return "SoundCube"; }
	
	bool fill;
	float line_width;
	
	float fade;
	bool show_box;
	
	void setupControlPanel()
	{
		fade = 0.5;
		
		rdtk::GetGUI().addSlider("line width", 0, 10, &line_width);
		rdtk::GetGUI().addSlider("fade", 0, 1, &fade);
		rdtk::GetGUI().addToggle("show box", &show_box);
	}
	
	void drawImGui()
	{
		ImGui::DragFloat("line width", &line_width, 0.1, 0, 10);
		ImGui::DragFloat("fade", &fade, 0.01, 0, 1);
		ImGui::Checkbox("show box", &show_box);
	}
	
	void setup()
	{
		show_box = true;
		ofAddListener(ofEvents().keyPressed, this, &SoundCube::onKeyPressed);
	}
	
	void update()
	{
		for(int i=0; i<shapes.size(); i++)
			shapes.at(i)->visible = show_box;
	}
	
	void onEnabled()
	{
		cout << "[Unit enabled] " << getName() << endl;
		loadXML();
	}
	
	void onDisabled()
	{
		cout << "[Unit disabled] " << getName() << endl;
        
		for (int i = 0; i < shapes.size(); i++)
		{
			shapes[i]->player->stop();
		}
	}
	
	void draw()
	{
		rdtk::BeginCamera();
		
		ofNoFill();
		ofPushStyle();
		ofSetLineWidth(line_width);
		
		if (show_box) ofDrawAxis(100);
		for (int i = 0; i < shapes.size(); i++)
		{
			shapes[i]->draw(fade);
		}
		
		ofPopStyle();
		rdtk::EndCamera();
	}
	
	void loadXML()
	{
		clear();
		
#define _S(src) #src
		
		string default_xml = _S(
<scene>
<shape type="cube" x="-100" y="50" z="0" rx="0" ry="90" rz="0" sx="200" sy="100" sz="100" color="FFFFFF" sound="../../../../resources/Sounds/1.aif" trigger="off" loop="on"/>
<shape type="cube" x="200" y="50" z="0" rx="0" ry="0" rz="0" sx="100" sy="100" sz="100" color="FFFFFF" sound="../../../../resources/Sounds/2.aif" trigger="on" loop="on"/>
<shape type="cube" x="200" y="50" z="200" rx="0" ry="0" rz="0" sx="100" sy="100" sz="100" color="FFFFFF" sound="../../../../resources/Sounds/3.aif" trigger="off" loop="on"/>
<shape type="cube" x="200" y="50" z="-200" rx="0" ry="0" rz="0" sx="100" sy="100" sz="100" color="FFFFFF" sound="../../../../resources/Sounds/4.aif" trigger="on" loop="on"/>
</scene>
		);

#undef _S
		
		const string filePath = "SoundCube.xml";
		
		if (!ofFile::doesFileExist(filePath))
		{
			ofBuffer buf(default_xml);
			ofBufferToFile(filePath, buf);
		}
		
		ofxXmlSettings xml;
		xml.loadFile(filePath);
		
		xml.pushTag("scene");
		
		int n = xml.getNumTags("shape");
		for (int i = 0; i < n; i++)
		{
			ofVec3f pos;
			ofVec3f rot;
			ofVec3f size;
			
			pos.x = xml.getAttribute("shape", "x", 0, i);
			pos.y = xml.getAttribute("shape", "y", 0, i);
			pos.z = xml.getAttribute("shape", "z", 0, i);
			
			rot.x = xml.getAttribute("shape", "rx", 0, i);
			rot.y = xml.getAttribute("shape", "ry", 0, i);
			rot.z = xml.getAttribute("shape", "rz", 0, i);
			
			size.x = xml.getAttribute("shape", "sx", 1, i);
			size.y = xml.getAttribute("shape", "sy", 1, i);
			size.z = xml.getAttribute("shape", "sz", 1, i);
			
			string hex = xml.getAttribute("shape", "color", "777777", i);
			ofColor color = ofColor::fromHex(ofHexToInt(hex));
			
			string type = xml.getAttribute("shape", "type", "", i);
			
			if (type != "")
			{
				rdtk::Primitive *s;
				
				if (type == "cube")
				{
					s = new rdtk::BoxPrimitive(size);
				}
				else if (type == "pyramid")
				{
					s = new rdtk::PyramidPrimitive(size.x);
				}
				else if (type == "sphere")
				{
					s = new rdtk::SpherePrimitive(size.x);
				}
				else
				{
					ofLogError("Shape") << "invalid shape type";
					continue;
				}
				
				s->setPosition(pos);
				s->setOrientation(rot);
				s->updatePhysicsTransform();
				
				s->getRigidBody().setStatic(true);
				
				Shape *o = new Shape;
				o->color = color;
				o->set(i, s);
				
				string sound = xml.getAttribute("shape", "sound", "", i);
				bool toggle_mode = xml.getAttribute("shape", "trigger", "off", i) == "on";
				bool loop = xml.getAttribute("shape", "loop", "off", i) == "on";
				
				if (sound != "")
				{
					o->loadSound(rdtk::ToResourcePath(sound), toggle_mode, loop);
				}
				
				cout << type << " loaded." << endl;
				
				shapes.push_back(o);
			}
			else
			{
				ofLogError("Shape") << "invalid shape type";
				continue;
			}
		}
		
		xml.popTag();
	}
	
	
	
	void clear()
	{
		for (int i = 0; i < shapes.size(); i++)
			delete shapes[i];
		shapes.clear();
	}
	
	void onKeyPressed(ofKeyEventArgs &e)
	{
		if (isEnabled()) return;
		
		if (e.key == 'r')
			loadXML();
	}
	
	
protected:
	
	vector<Shape*> shapes;
	
};
