#pragma once

#include "ramMain.h"

/*
 
 XML format
 
 ====
 
 <scene>
 <shape type="cube" x="-100" y="50" z="0" rx="0" ry="90" rz="0" sx="200" sy="100" sz="100" color="FFFFFF" sound="Sounds/dlone_1m.aif" trigger="off" loop="on"/>
 <shape type="cube" x="200" y="50" z="0" rx="0" ry="0" rz="0" sx="100" sy="100" sz="100" color="FFFFFF" sound="Sounds/dlone_2m.aif" trigger="on" loop="on"/>
 <shape type="cube" x="200" y="50" z="200" rx="0" ry="0" rz="0" sx="100" sy="100" sz="100" color="FFFFFF" sound="Sounds/dlone_3m.aif" trigger="off" loop="on"/>
 <shape type="cube" x="200" y="50" z="-200" rx="0" ry="0" rz="0" sx="100" sy="100" sz="100" color="FFFFFF" sound="Sounds/dlone_4m.aif" trigger="on" loop="on"/>
 </scene>
 ====
 
 */
class SoundCube : public ramBaseScene
{
public:
	
	class Shape
	{
	public:
		
		ofColor color;
		ofSoundPlayer *player;
		bool trigger_mode;
		
		Shape() : id(-1), obj(NULL), alpha(0), is_inside(false), player(NULL), trigger_mode(false) {}
		
		~Shape()
		{
			if (obj)
			{
				delete obj;
			}
			
			if (player)
			{
				delete player;
			}
		}
		
		void set(int id, ramPrimitive *obj)
		{
			this->id = id;
			this->obj = obj;
			
			struct Callback
			{
				Shape *shape;
				Callback(Shape *shape) : shape(shape) {}
				
				void operator()()
				{
					shape->onCollision();
				}
			};
			
			obj->getRigidBody().setCollisionCallback(Callback(this));
		}
		
		void loadSound(const string &path, bool trigger = false, bool loop = true)
		{
			if (player) delete player;
			
			player = new ofSoundPlayer;
			player->loadSound(path);
			player->setLoop(loop ? OF_LOOP_NORMAL : OF_LOOP_NONE);
			trigger_mode = trigger;
		}
		
		void draw()
		{
			ofPushStyle();
			
			bool b = ofGetElapsedTimef() - last_collision_time < 0.1;
			
			if (is_inside != b)
			{
				is_inside = b;
				
				if (b)
					triggerUp();
				else
					triggerDown();
			}
			
			if (b)
			{
				alpha += (0 - alpha) * 0.1;
			}
			else
			{
				alpha += (1 - alpha) * 0.1;
				
			}
			
			ofSetColor(color, 127 + 127 * (1 - alpha));
			
			obj->transformGL();
			ofDrawBitmapString(ofToString(id), 0, 0);
			obj->restoreTransformGL();
			
			obj->draw();
			
			ofPopStyle();
		}
		
		void onCollision()
		{
			last_collision_time = ofGetElapsedTimef();
		}
		
		void triggerUp()
		{
			if (trigger_mode)
			{
				if (player) player->play();
			}
			else
			{
				if (player) player->stop();
			}
			
			// cout << "trigger up: " << ofGetElapsedTimef() << endl;
		}
		
		void triggerDown()
		{
			if (!trigger_mode)
			{
				if (player) player->play();
			}
			else
			{
				if (player) player->stop();
			}

			
			// cout << "trigger dow: " << ofGetElapsedTimef() << endl;
		}
		
	private:
		
		int id;
		ramPrimitive *obj;
		
		float alpha;
		
		bool is_inside;
		float last_collision_time;
	};
	
	const string getName() { return "SoundCube"; }
	
	bool fill;
	float line_width;
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		
		panel->addWidgetDown(new ofxUILabel(getName(), OFX_UI_FONT_LARGE));
		panel->addSpacer(gui.kLength, 2);
		
		panel->addSlider("line width", 1, 10, &line_width, gui.kLength, gui.kDim);
	}
	
	void setup()
	{
		loadXML();
		
		ofAddListener(ofEvents().keyPressed, this, &SoundCube::onKeyPressed);
	}
	
	void update()
	{
		
	}
	
	void draw()
	{
		ramBeginCamera();
		
		ofNoFill();
		
		ofSetLineWidth(line_width);
		
		ofDrawAxis(100);
		for (int i = 0; i < shapes.size(); i++)
		{
			shapes[i]->draw();
		}
		
		ramEndCamera();
	}
	
	void loadXML()
	{
		clear();
		
#define _S(src) #src
		
		string default_xml = _S(
<scene>
<shape type="cube" x="-100" y="50" z="0" rx="0" ry="90" rz="0" sx="200" sy="100" sz="100" color="FFFFFF" sound="Sounds/dlone_1m.aif" trigger="off" loop="on"/>
<shape type="cube" x="200" y="50" z="0" rx="0" ry="0" rz="0" sx="100" sy="100" sz="100" color="FFFFFF" sound="Sounds/dlone_2m.aif" trigger="on" loop="on"/>
<shape type="cube" x="200" y="50" z="200" rx="0" ry="0" rz="0" sx="100" sy="100" sz="100" color="FFFFFF" sound="Sounds/dlone_3m.aif" trigger="off" loop="on"/>
<shape type="cube" x="200" y="50" z="-200" rx="0" ry="0" rz="0" sx="100" sy="100" sz="100" color="FFFFFF" sound="Sounds/dlone_4m.aif" trigger="on" loop="on"/>
</scene>
		);

#undef _S
		
		if (!ofFile::doesFileExist("SoundCube.xml"))
		{
			ofBuffer buf(default_xml);
			ofBufferToFile("SoundCube.xml", buf);
		}
		
		ofxXmlSettings xml;
		xml.loadFile("SoundCube.xml");
		
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
				ramPrimitive *s;
				
				if (type == "cube")
				{
					s = new ramBoxPrimitive(size);
				}
				else if (type == "pyramid")
				{
					s = new ramPyramidPrimitive(size.x);
				}
				else if (type == "sphere")
				{
					s = new ramSpherePrimitive(size.x);
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
					o->loadSound(sound, toggle_mode, loop);
				}
				
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
		loadXML();
	}
	
protected:
	
	vector<Shape*> shapes;
	
};
