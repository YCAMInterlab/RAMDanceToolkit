#include "testApp.h"
#include "ofxValue.h"
/*
 
 Json Example:
 -------------
 [
	 {
		 "style": {
			 "type": "curve",
			 "curve_gain": 250,
			 "line_width": 3,
			 "color": [255, 100, 0],
			 "effect": {
				 "resampling": 10,
				 "dotted": 2
			 }
		 },
		 "from": {
			 "type": "actor",
			 "target": "Ando_2012-09-01_18-49-10",
			 "node_id": 1
		 },
		 "from_cp": {
			 "type": "actor",
			 "target": "Ando_2012-09-01_18-49-10",
			 "node_id": 0
		 },
		 "to": {
			 "type": "actor",
			 "target": "Ando_2012-09-01_18-49-10",
			 "node_id": 21
		 },
		 "to_cp": {
			 "type": "actor",
			 "target": "Ando_2012-09-01_18-49-10",
			 "node_id": 22
		 }
	 }
 ]
 -------------
 */

class DrawLine
{
	
public:
	DrawLine() {}
	
	void setup()
	{
		filename = "DrawLines.settings.json";
		reloadSettings();
		ofAddListener(ofEvents().keyPressed, this, &DrawLine::keyPressed);
	}
	void setup(string file)
	{
		filename = file;
		reloadSettings();
		ofAddListener(ofEvents().keyPressed, this, &DrawLine::keyPressed);
	}
	
	void exit()
	{
		ofRemoveListener(ofEvents().keyPressed, this, &DrawLine::keyPressed);
	}
	
	void draw()
	{
		ofSetColor(255);
		
		for (int i = 0; i < lines.size(); i++)
		{
			Line &o = lines[i];
			int dotted = 1;
			
			ofPushStyle();
			
//			ramNode& from = getNode(o.value["from"]);
			ramNode from = ramNode();
			from.setPosition(0, 0, 0);
			ramNode& to = getNode(o.value["to"]);
			
			ofPolyline poly;
			
			ofVec3f p1 = from.getTransformMatrix().preMult(ofVec3f(0, 0, 0));
			ofVec3f p2 = to.getTransformMatrix().preMult(ofVec3f(0, 0, 0));
				
			if (p1.distance(p2) < 1000)
			{
				ofxValue &style = o.value["style"];
				string style_type = style["type"].as<string>();
				
				// apply style
				{
					if (style.hasKey("color"))
					{
						ofColor c;
						c.r = style["color"][0].as<int>();
						c.g = style["color"][1].as<int>();
						c.b = style["color"][2].as<int>();
						ofSetColor(c);
					}
					
					if (style.hasKey("line_width"))
					{
						ofSetLineWidth(style["line_width"].as<float>());
					}
				}
				
				// line type
				{
					if (style_type == ""
						|| style_type == "line")
					{
						poly.addVertex(p1);
						poly.addVertex(p2);
					}
					else if (style_type == "curve")
					{
						ramNode& from_cp = getNode(o.value["from_cp"]);
						ramNode& to_cp = getNode(o.value["to_cp"]);
						
						float gain = style["curve_gain"].as<float>();
						
						ofVec3f cp1 = from_cp.getTransformMatrix().preMult(ofVec3f(0, 0, 0));
						ofVec3f cp2 = to_cp.getTransformMatrix().preMult(-ofVec3f(0, 0, 0));
						
						ofVec3f pp1 = (p1 - cp1).normalized() * gain;
						ofVec3f pp2 = (p2 - cp2).normalized() * gain;
						
						pp1 += p1;
						pp2 += p2;
						
						poly.addVertex(p1);
						poly.bezierTo(pp1, pp2, p2, 64);
					}
				}
				
				// vertex effects
				{
					if (style.hasKey("effect"))
					{
						ofxValue &effect = style["effect"];
						
						float resampling = 10;
						
						if (effect.hasKey("resampling"))
						{
							resampling = effect["resampling"].as<float>();
						}
						
						poly = poly.getResampledBySpacing(resampling);
						
						if (effect.hasKey("dotted"))
						{
							dotted = effect["dotted"].as<int>();
						}
						
						if (effect.hasKey("spiral"))
						{
							ofxValue &spiral = effect["spiral"];
							float radius = 10;
							float freq = 10;
							
							if (spiral.hasKey("freq"))
								freq = spiral["freq"].as<float>();
							
							if (spiral.hasKey("radius"))
								radius = spiral["radius"].as<float>();
							
							ofPolyline pp = poly;
							
							float phase = 0;
							for (int i = 0; i < poly.size() - 1; i++)
							{
								// FIXME: I'm not sure about this impl...
								ofVec3f &v0 = poly[i];
								ofVec3f &v1 = poly[i + 1];
								
								ofVec3f d = (v1 - v0);
								ofVec3f c = d.crossed(ofVec3f(0, 1, 0)).normalized();
								
								c *= radius;
								c.rotate(phase, ofVec3f(d));
								
								pp[i] = v0 + c;
								
								phase += freq;
							}
							
							poly = pp;
						}
						
						// TODO: display_style_arrow
						// TODO: extend line over control point
					}
				}
			}
			
			if (dotted < 0)
				dotted = 1;
			
			if (poly.size() > 0)
			{
				glBegin(GL_LINES);
				for (int i = 0; i < poly.size() - 1; i += dotted)
				{
					ofVec3f &v0 = poly[i];
					ofVec3f &v1 = poly[i + 1];
					glVertex3fv(v0.getPtr());
					glVertex3fv(v1.getPtr());
				}
				glEnd();
			}
			
			ofPopStyle();
		}
	}
	
	void reloadSettings()
	{
		ofxValue v;
		v.load(filename);
		
		if (!v.isArray()) return;
		lines.clear();
		
		for (int i = 0; i < v.size(); i++)
		{
			ofxValue p = v[i];
			if (!p.isMap()) continue;
			
			Line o;
			o.value = p;
			lines.push_back(o);
			
			cout << "DrawLine ";
			cout << p["from"]["target"].as<string>() << ":" << p["from"]["node_id"].as<int>();
			cout << " -> ";
			cout << p["to"]["target"].as<string>() << ":" << p["to"]["node_id"].as<int>();
			cout << endl;
		}
	}
	
private:
	
	struct Line
	{
		ofxValue value;
	};
	
	vector<Line> lines;
	string filename;
	
	ramNode& getNode(ofxValue &v)
	{
		const string type = ofToLower( v["type"].as<string>() );
		const string target = v["target"].as<string>();
		const int node_id = v["node_id"].as<int>();
		
		if (!type.empty() && !target.empty())
		{
			if (type == "actor")
			{
				return ramActorManager::instance().getActor(target).getNode(node_id);
			}
			else
			{
				return ramActorManager::instance().getRigidBody(target).getNode(node_id);
			}
		}
	}

	void keyPressed(ofKeyEventArgs& e)
	{
		if (e.key == 'r')
			reloadSettings();
	}
};


static const string myActorName = "default";
DrawLine line;


#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(0);
	oscReceiver.setup(10001);
	
	// enable ramBaseApp::setup, update, draw, exit
	ramEnableAllEvents();
	
	// Line
	line.setup();
}

//--------------------------------------------------------------
void testApp::update()
{
	oscReceiver.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	
}




#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawFloor()
{
	ramBasicFloor(600., 50.);
}

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	line.draw();
	
	ofPushStyle();
	ofNoFill();
	for (int i=0; i<actor.getNumNode(); i++)
	{
		ramNode &node = actor.getNode(i);
		ofBox(node, (i==ramActor::JOINT_HEAD) ? 6 : 3);
		
		if (node.hasParent())
			ofLine(node, *node.getParent());
	}
	ofPopStyle();
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

