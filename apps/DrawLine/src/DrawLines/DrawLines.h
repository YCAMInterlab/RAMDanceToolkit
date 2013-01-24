#include "ofxValue.h"
/*
 
 Json Example:
 -------------
 [
 {
 "style": {  "type": "curve",
 "curve_gain": 250,
 "line_width": 3,
 "color": [255, 100, 0],
 "effect": {
 "resampling": 2,
 "dotted": 10,
 "spiral":
 {
 "freq":20
 }
 }
 },
 "from"	  : { "type": "actor", "target": "Ando_2012-09-01_18-49-10", "node_id": 0 },
 "from_cp": { "type": "actor", "target": "Ando_2012-09-01_18-49-10", "node_id": 13 },
 "to"	  : { "type": "actor", "target": "Ando_2012-09-01_18-49-10", "node_id": 21 },
 "to_cp"  : { "type": "actor", "target": "Ando_2012-09-01_18-49-10", "node_id": 22 }
 }
 ]
 -------------
 */

class DrawLines : public ramSceneBase
{
	
	ofxValue myLine;
	string target_type, target_actor;
	
public:
	DrawLines()
	{
		setSceneName("DrawLines");
		
		// ---
		
		target_type = "actor";
		target_actor = "Ando_2012-09-01_18-49-10";
	}
		
	
	void setup()
	{
		filename = "DrawLines.settings.json";
		reloadSettings();
		ofAddListener(ofEvents().keyPressed, this, &DrawLines::keyPressed);
	}
	
	void setup(string file)
	{
		filename = file;
		reloadSettings();
		ofAddListener(ofEvents().keyPressed, this, &DrawLines::keyPressed);
	}
	
	void exit()
	{
		ofRemoveListener(ofEvents().keyPressed, this, &DrawLines::keyPressed);
	}
	
	void refreshControlPanel(ramControlPanel& gui)
	{
		guiPtr = &gui;
		gui.addPanel( getSceneName() );
		gui.addMultiToggle("Line: Type", 1, variadic("line")("curve"));
		
		// nodes
		gui.addLabel(target_actor);
		gui.addTextDropDown("Line: node from", 0, ramActor::getJointNames());
		gui.addTextDropDown("Line: node from_cp", 1, ramActor::getJointNames());
		gui.addTextDropDown("Line: node to", 21, ramActor::getJointNames());
		gui.addTextDropDown("Line: node to_cp", 22, ramActor::getJointNames());
		
		// style
		gui.addSlider("Line: Curve gain", 250, 1, 1000);
		gui.addSlider("Line: Width", 3, 1, 20);
		gui.addSlider("Line: Color R", 2, 1, 255);
		gui.addSlider("Line: Color G", 2, 1, 255);
		gui.addSlider("Line: Color B", 2, 1, 255);
		
		// effect
		gui.addSlider("Line: Resampling", 1, 1, 100);
		gui.addSlider("Line: Dot size", 2, 1, 30);
		gui.addSlider("Line: Spiral freq", 1, 1, 1000);
		gui.addSlider("Line: Spiral radius", 1, 1, 100);
	}
	
	void update()
	{
		if (guiPtr->hasValueChangedInPanel( getSceneName() ))
		{
			ofxValue o = ofxValue::Map();

			// style
			o["style"] = ofxValue::Map();
			o["style"]["type"] = guiPtr->getValueI("Line: Type") == 0 ? "line" : "curve";
			o["style"]["curve_gain"] = guiPtr->getValueI("Line: Curve gain");
			o["style"]["line_width"] = guiPtr->getValueI("Line: Width");
			
			// color
			o["style"]["color"] = ofxValue::Array();
			o["style"]["color"].push( guiPtr->getValueF("Line: Color R") );
			o["style"]["color"].push( guiPtr->getValueF("Line: Color G") );
			o["style"]["color"].push( guiPtr->getValueF("Line: Color B") );
			
			// effect
			o["style"]["effect"] = ofxValue::Map();
			o["style"]["effect"]["relampling"] = guiPtr->getValueF("Line: Resampling");;
			o["style"]["effect"]["dotted"] = guiPtr->getValueF("Line: Dot size");;
			
			o["style"]["effect"]["spiral"] = ofxValue::Map();
			o["style"]["effect"]["spiral"]["freq"] = guiPtr->getValueF("Line: Spiral freq");
			o["style"]["effect"]["spiral"]["radius"] = guiPtr->getValueF("Line: Spiral radius");;
			
			// nodes
			o["from"] = ofxValue::Map();
			o["from"]["type"] = target_type;
			o["from"]["target"] = target_actor;
			o["from"]["node_id"] = guiPtr->getValueI("Line: node from");

			o["from_cp"] = ofxValue::Map();
			o["from_cp"]["type"] = target_type;
			o["from_cp"]["target"] = target_actor;
			o["from_cp"]["node_id"] = guiPtr->getValueI("Line: node from_cp");

			o["to"] = ofxValue::Map();
			o["to"]["type"] = target_type;
			o["to"]["target"] = target_actor;
			o["to"]["node_id"] = guiPtr->getValueI("Line: node to");

			o["to_cp"] = ofxValue::Map();
			o["to_cp"]["type"] = target_type;
			o["to_cp"]["target"] = target_actor;
			o["to_cp"]["node_id"] = guiPtr->getValueI("Line: node to_cp");
			
			myLine = o;
			reloadSettings();
		}
	}
	
	void draw()
	{
//		if (!bEnabled) return;
		
		ramCameraBegin();
		
		ofSetColor(255);
		
		for (int i = 0; i < lines.size(); i++)
		{
			Line &o = lines[i];
			int dotted = 1;
			
			ofPushStyle();
			
			ramNode& from = getNode(o.value["from"]);
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
							ofxValue spiral = effect["spiral"];
							float radius = 10;
							float freq = 10;
							
							if (spiral.hasKey("freq"))
								freq = spiral["freq"].as<float>();
							
							if (spiral.hasKey("radius"))
								radius = spiral["radius"].as<float>();
							
							
							ofPolyline pp = poly;
							
							float phase = 0;
							for (int i = 0; i < poly.size(); i++)
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
			
			
			glEnable(GL_DEPTH_TEST);
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
				
				ofPushStyle();
				ofColor shadowColor = ramColor::GRAY;
				shadowColor.a = 80;
				ofSetColor(shadowColor);
				glPushMatrix();
				glMultMatrixf( getMatrix().getPtr() );
				glBegin(GL_LINES);
				for (int i = 0; i < poly.size() - 1; i += dotted)
				{
					ofVec3f &v0 = poly[i];
					ofVec3f &v1 = poly[i + 1];
					glVertex3fv(v0.getPtr());
					glVertex3fv(v1.getPtr());
				}
				glEnd();
				glPopMatrix();
				ofPopStyle();
				
			}
			glDisable(GL_DEPTH_TEST);
			
			ofPopStyle();
		}
		
		ramCameraEnd();
	}
	
	void reloadSettings()
	{
		
		lines.clear();
		
		if (guiPtr != NULL)
		{
			if (myLine.isMap())
			{
				Line l;
				l.value = myLine;
				lines.push_back(l);
			}
		}
		
		ofxValue v;
		v.load(filename);
		
		
		if(!v.isArray()) return;
		for (int i = 0; i < v.size(); i++)
		{
			ofxValue p = v[i];
			if (!p.isMap()) continue;
			
			Line o;
			o.value = p;
			lines.push_back(o);
			
			cout << "DrawLines ";
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
				return ramActorManager::instance().getActor(target).getNode(node_id);
			
			else
				return ramActorManager::instance().getRigidBody(target).getNode(node_id);
		}
	}
	
	void keyPressed(ofKeyEventArgs& e)
	{
		if (e.key == 'r')
			reloadSettings();
	}
};

