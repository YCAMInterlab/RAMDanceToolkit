#include "testApp.h"

class SoundCube : public ramBaseScene
{
public:
	
	class Shape
	{
	public:
		
		Shape() : id(-1), obj(NULL), alpha(0) {}
		
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
			
			if (ofGetElapsedTimef() - last_collision_time < 0.1)
			{
				alpha += (0 - alpha) * 0.1;
			}
			else
			{
				alpha += (1 - alpha) * 0.1;
				
			}
			
			ofSetColor(255, 255 * alpha, 255 * alpha);
			
			obj->draw();

			ofPopStyle();
		}
		
		void onCollision()
		{
			last_collision_time = ofGetElapsedTimef();
		}
		
	private:
		
		int id;
		ramPrimitive *obj;
		
		float alpha;
		
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

		panel->addToggle("fill", &fill, 20, 20);
		panel->addSlider("line width", 0, 10, &line_width, gui.kLength, gui.kDim);
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
		
		if (fill)
			ofFill();
		else
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
//				s->getRigidBody().setDisableDeactivation(true);
				
				Shape *o = new Shape;
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

ramSceneManager SM;

SoundCube soundCube;

#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(ramColor::WHITE);
	
	
	/// ram setup
	// ------------------
	ramInit();
	oscReceiver.setup(10000);
	
	vector<ramBaseScene*> scenes;
	scenes.push_back(soundCube.getPtr());
	SM.setup(scenes);
}

//--------------------------------------------------------------
void testApp::update()
{
	/// Entities update
	// ------------------
	oscReceiver.update();
	
	SM.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	SM.draw();
	
	ramBeginCamera();
	
//	ramPhysics::instance().debugDraw();
	
	ramEndCamera();
}



#pragma mark - ram methods

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	ramEnablePhysicsPrimitive();
	ramDrawBasicActor(actor);
}

//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
{
	
}




#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{
	
}

