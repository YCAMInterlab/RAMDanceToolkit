#pragma once
#include "ramSceneBase.h"
#include "DelaunayTriangles.h"

string
kPoints = "points",
kSpeed = "speed",
kReconstruct = "reconstruct";
int points;

vector<Vector> vertices;
set<Triangle> triangles;


class Triangles : public ramSceneBase
{
	string key_master_size, key_line_width;

	
	void refreshVertices()
	{
		ramActor& actor = getActor( myActorName );
		vector<ramNode> nodes;
		nodes.clear();
		
		nodes.push_back( actor.getNode(ramActor::JOINT_HEAD) );
		nodes.push_back( actor.getNode(ramActor::JOINT_RIGHT_HAND) );
		nodes.push_back( actor.getNode(ramActor::JOINT_RIGHT_TOE) );
		nodes.push_back( actor.getNode(ramActor::JOINT_LEFT_HAND) );
		nodes.push_back( actor.getNode(ramActor::JOINT_LEFT_TOE) );
		
		vertices.clear();
		vertices.resize(nodes.size());
		
		for(int i = 0; i < nodes.size(); ++i)
		{
			ramNode &node = nodes.at(i);
			
			Vector v;
			v.x = node.getPosition().x;
			v.y = node.getPosition().y;
			v.z = node.getPosition().z;
			
			float speed = guiPtr->getValueF(kSpeed);
			v.speed = ofRandom(-speed, speed);
			
			v.color.setHsb( 0,0,0 );
			vertices.at(i) = v;
		}
		
		Delaunay3d::getDelaunayTriangles(vertices, &triangles);
	}
	
public:
	
	/*!
	 Scene params
	 */
	vector<float> jointSizes;
	float bigBoxLineWidth;
	
	Triangles()
	{
		setSceneName("Triangles");
	}
	
	void refreshControlPanel(ramControlPanel& gui)
	{
		guiPtr = &gui;
		guiPtr->addPanel(getSceneName());
		guiPtr->addToggle(kReconstruct);
		
		guiPtr->addSlider("obj scale", 1, 0.1, 10);
		
//		guiPtr->addSlider(kPoints, 4, 4, 20);
//		guiPtr->addSlider(kSpeed, 0.1, -10, 10);
//		guiPtr->addSlider("x", 300, 10, 1000);
//		guiPtr->addSlider("y", 300, 10, 1000);
//		guiPtr->addSlider("z", 300, 10, 1000);
		
		refreshVertices();
	}
	
	void setup()
	{
		points = 4;
	}
	
	void update()
	{
		if (!bEnabled) return;
		
		if ( guiPtr->hasValueChanged(kReconstruct) )
		{
			refreshVertices();
		}
		
		if ( guiPtr->hasValueChanged(kPoints) )
		{
			points = guiPtr->getValueI(kPoints);
		}
		
//		for (int i=0; i<vertices.size(); i++)
//		{
//			float delta = ofGetElapsedTimef();
//			
//			Vector& v = vertices.at(i);
//			v.x = cos(delta * v.speed) * guiPtr->getValueF("x");
//			v.z = sin(delta * v.speed) * guiPtr->getValueF("z");
//		}
	}
	
	void draw()
	{
		if (!bEnabled) return;
		
		ramBeginCamera();
		ofEnableAlphaBlending();
		
		ofColor shadowColor = ramColor::BLACK;
		shadowColor.a = 30;
		
		ofPushStyle();
		ofPushMatrix();
		
		
		float scale = guiPtr->getValueF("obj scale");
		ofScale(scale, scale, scale);
		
		int i=0;
		for(std::set<Triangle>::iterator it = triangles.begin(); it != triangles.end(); ++it)
		{
			
			Triangle t = *it;
			
			for(int i = 0; i < 3; ++i)
			{
				ofVec3f v1, v2;
				v1 = ofVec3f(t.p[i]->x,
							 t.p[i]->y,
							 t.p[i]->z);
				
				v2 = ofVec3f(t.p[(i+1)%3]->x,
							 t.p[(i+1)%3]->y,
							 t.p[(i+1)%3]->z);
				
				ofSetColor( ramColor::BLACK );
				ofSetLineWidth(3);
				
				if(v1.distance(v2) < 2000)
				{
					
					glEnable(GL_DEPTH_TEST);
					{
						ofLine(v1, v2);
						ofSetColor(0);
						ofBox(v1, 2);
					}
					glDisable(GL_DEPTH_TEST);
					
					glPushMatrix();
					{
						glMultMatrixf(getMatrix().getPtr());
						ofEnableAlphaBlending();
						
						ofSetColor(shadowColor);
						ofLine(v1, v2);
					}
					glPopMatrix();
				}
			}
			
			i++;
		}
		ofPopMatrix();
		ofPopStyle();
		
		ramEndCamera();
	}
	
	
	void drawActor(ramActor& actor)
	{
		
	}
	
	void drawRigidBody(ramRigidBody& rigid)
	{
		
	}
	void drawFloor()
	{
		
	}
};

