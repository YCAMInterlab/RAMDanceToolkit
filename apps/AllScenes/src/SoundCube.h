#pragma once

#include "ramMain.h"

/*
 
 XML format
 
 ====
 
 <scene>
 <shape type="cube" x="-100" y="50" z="0" rx="0" ry="90" rz="0" sx="200" sy="100" sz="100" color="FF0000" sound=""/>
 <shape type="sphere" x="200" y="50" z="0" rx="0" ry="0" rz="0" sx="100" sy="100" sz="100" color="FF00FF" sound=""/>
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
		
		Shape() : id(-1), obj(NULL), alpha(0), is_inside(false) {}
		
		~Shape()
		{
			if (obj)
			{
				delete obj;
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
			
			obj->draw();
			
			ofPopStyle();
		}
		
		void onCollision()
		{
			last_collision_time = ofGetElapsedTimef();
		}
		
		void triggerUp()
		{
			cout << "trigger up: " << ofGetElapsedTimef() << endl;
		}
		
		void triggerDown()
		{
			cout << "trigger dow: " << ofGetElapsedTimef() << endl;
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
		
		//		panel->addToggle("fill", &fill, 20, 20);
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
		
		//		if (fill)
		//			ofFill();
		//		else
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
    <shape type="cube" x="-100" y="50" z="0" rx="0" ry="90" rz="0" sx="200" sy="100" sz="100" color="FF0000" sound=""/>
    <shape type="sphere" x="200" y="50" z="0" rx="0" ry="0" rz="0" sx="100" sy="100" sz="100" color="FF00FF" sound=""/>
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
				shapes.push_back(o);
			}
			else
			{
				ofLogError("Shape") << "invalid shape type";
				continue;
			}
			
			shapes;
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
