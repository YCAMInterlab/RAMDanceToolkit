// 
// BasicActor.h - RAMDanceToolkit
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

class BasicActor : public rdtk::BaseScene
{
	
	struct ControlSegment
	{
		string name;
		ofVec3f translate;
		float scale;
		float r,g,b;
        
        ControlSegment()
        {
			translate = ofVec3f::zero();
			scale = 1.0;
			r = ofRandom(0, 255);
			g = ofRandom(0, 255);
			b = ofRandom(0, 255);
        }
		
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
	
	ofLight light;
	bool enableLight;
	bool use_new_actor;
	
	ofxUICanvas* my_panel;
	
public:
	
	BasicActor() {}
	
	void setupControlPanel()
	{
		ramControlPanel &gui = rdtk::GetGUI();
		
		gui.addToggle("Enable light", &enableLight);
		
		use_new_actor = true;
		gui.addToggle("use_new_actor", &use_new_actor);
		
#ifdef RAM_GUI_SYSTEM_OFXUI
		
		my_panel = gui.getCurrentUIContext();
		
#endif
	}
	
	void setup()
	{
		enableLight = true;
		light.setPosition(300, 600, 300);
		
		mControlSegments.clear();
	}
	
	void addEntityControl(const rdtk::NodeArray &nodeArray)
	{
		ramControlPanel &gui = rdtk::GetGUI();
		
#ifdef RAM_GUI_SYSTEM_OFXUI
		
		ofxUICanvas* panel = my_panel;

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
		
#endif
	}
	
	void update()
	{
		int numSegments = mControlSegments.size();
		int numActor = getNumNodeArray();
		
		if (numSegments < numActor)
		{
			for (int i=numSegments; i<numActor; i++)
			{
				const rdtk::NodeArray copy = getNodeArray(i);
				addEntityControl( copy );
			}
		}
	}
	
	void draw()
	{
		if (enableLight) light.enable();
		
		rdtk::BeginCamera();
		for (int i=0; i<getNumNodeArray(); i++)
		{
            
			rdtk::NodeArray &array = getNodeArray(i);
            
            ControlSegment &segment = mControlSegments[array.getName()];
			
			glPushMatrix();
			{
				glTranslatef(segment.translate.x, segment.translate.y, segment.translate.z);
				glScalef(segment.scale, segment.scale, segment.scale);
				
				ofSetColor(segment.r, segment.g, segment.b);
				
				if (use_new_actor)
					drawNodes(array);
				else
					rdtk::DrawNodes(array);
			}
			glPopMatrix();
		}
		rdtk::EndCamera();
		
		if (enableLight) light.disable();
	}
	
	void drawNodes(const rdtk::NodeArray &actor)
	{
		ofPushStyle();
		
		ofNoFill();
		ofSetRectMode(OF_RECTMODE_CENTER);
		
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_CULL_FACE);
		
		ofColor front_color(ofGetStyle().color, 200);
		ofColor back_color(ofGetStyle().color, 150);
		
		for (int i = 0; i < actor.getNumNode(); i++)
		{
			ofSetColor(front_color);
			
			const rdtk::Node &node = actor.getNode(i);
			const rdtk::Node *parent = node.getParent();
			if (parent == NULL) continue;
			
			ramBox(node, 2);
			
			parent->beginTransform();
			
			ofVec3f axis(0, 0, 1);
			ofVec3f c = node.getPosition().normalized().crossed(axis);
			
			ofRotate(90, c.x, c.y, c.z);
			
			ofVec3f p0 = node.getGlobalPosition();
			ofVec3f p1 = parent->getGlobalPosition();
			
			float dist = p0.distance(p1);
			float offset = 0.2;
			int num = 4;
			
			glNormal3f(0, 0, 0);
			ofDrawLine(ofVec3f(0), ofVec3f(0, 0, -dist));
			
			if (i < 4)
				glScalef(1., 1.8, 1);
			else if (i == 4)
				glScalef(1, 1, 3);
			else
				glScalef(1., 0.8, 1);
			
			glBegin(GL_TRIANGLE_STRIP);
			for (int n = 0; n < 6; n++)
			{
				float d = ofMap(n, 0, 5, 0, 1);
				float dd = ofMap(d, 0, 1, offset, 1 - offset);
				
				float xx = sin(d * PI) * 2 + 4;
				float zz = dd * -dist;
				float w = 5;
				
				glNormal3f(1, 0.5, 0);
				glVertex3f(xx, w, zz);
				glNormal3f(1, -0.5, 0);
				glVertex3f(xx, -w, zz);
			}
			glEnd();
			
			ofSetColor(back_color);
			
			glBegin(GL_TRIANGLE_STRIP);
			for (int n = 0; n < 6; n++)
			{
				float d = ofMap(n, 0, 5, 0, 1);
				float dd = ofMap(d, 0, 1, offset, 1 - offset);
				
				float xx = -sin(d * PI) * 1 - 6;
				float zz = dd * -dist;
				float w = 3;
				
				glNormal3f(-1, 0.5, 0);
				glVertex3f(xx, -w, zz);
				glNormal3f(-1, -0.5, 0);
				glVertex3f(xx, w, zz);
			}
			glEnd();
			
			parent->endTransform();
		}
		
		glPopAttrib();
		
		ofPopStyle();
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
	
	string getName() const { return "Basic Actor"; }
};
