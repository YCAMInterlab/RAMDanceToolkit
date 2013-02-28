#pragma once

#include "ramMain.h"
#include "ofxCv.h"
#include "ramPlanarGestureRecognizer.h"

class HistoryPoint : public ramFading
{
public:
	ofVec3f point;
	
	HistoryPoint(ofVec3f point)
	:point(point)
	{}
};

class SpatialMark : public ramFading
{
public:
	ofVec3f position, normal;
	ofPolyline mark;
	
	SpatialMark(ofVec3f position, ofVec3f normal, ofPolyline& mark)
	:position(position)
	,normal(normal)
	,mark(mark)
	{
	}
	
	void draw3d(float lifespan)
	{
		ofPushMatrix();
		ofTranslate(position);
		rotateToNormal(normal);
		draw2d(lifespan);
		ofPopMatrix();
	}
	
	void draw2d(float lifespan)
	{
		ofPushMatrix();
		ofSetColor(255, 255 * getLife(lifespan));
		mark.draw();
		ofPopMatrix();
	}
};

class Notation : public ramBaseScene
{
public:
	
	map<int, list<HistoryPoint> > history;
	map<int, ofVec3f> centers, normals;
	map<int, ofPolyline> projected, idealized;
	list<SpatialMark> allMarks;
	
	float maxNotationLife, maxMarkLife;
	float threshold;
	bool onlyLimbs, drawDebug;
	
	float centerLerpRate, normalLerpRate;
	
	ramPlanarGestureRecognizer recognizer;
	
	void setupControlPanel()
	{
		onlyLimbs = true;
		maxNotationLife = 1;
		maxMarkLife = 15;
		centerLerpRate = .1;
		normalLerpRate = .1;
		threshold = .3;
		drawDebug = false;
		ofxUICanvas* panel = gui().getCurrentUIContext();
		panel->addToggle("Only limbs", &onlyLimbs, 20, 20);
		panel->addToggle("Draw debug", &drawDebug, 20, 20);
		panel->addSlider("History fade out", 0, 5, &maxNotationLife, 300, 20);
		panel->addSlider("Mark fade out", 0, 30, &maxMarkLife, 300, 20);
		panel->addSlider("Threshold", 0, 1, &threshold, 300, 20);
		panel->addSlider("Center lerp rate", 0, 1, &centerLerpRate, 300, 20);
		panel->addSlider("Normal lerp rate", 0, 1, &normalLerpRate, 300, 20);
	}
	
	void setup()
	{
		
	}
	
	void update()
	{
		ramFading::bury(allMarks, maxMarkLife);
		
		// remove any dead points
		map<int, list<HistoryPoint> >::iterator itr;
		for(itr = history.begin(); itr != history.end(); itr++)
		{
			list<HistoryPoint>& cur = itr->second;
			ramFading::bury(cur, maxNotationLife);
			vector<ofVec3f> all;
			list<HistoryPoint>::iterator curItr;
			for(curItr = cur.begin(); curItr != cur.end(); curItr++)
			{
				all.push_back(curItr->point);
			}
			if(cur.size() > 5) {
				ofVec3f center, normal;
				approximatePlane(all, 1000, center, normal);
				centers[itr->first].interpolate(center, centerLerpRate);
				normals[itr->first].interpolate(normal, normalLerpRate);		
				normals[itr->first].normalize();
				
				center = centers[itr->first];
				normal = normals[itr->first];
				
				ofQuaternion q;
				ofVec3f axis(0, 0, 1);
				q.makeRotate(normal, axis);
				float rotationAmount;
				ofVec3f rotationAngle;
				q.getRotate(rotationAmount, rotationAngle);
				
				ofPolyline polyline;
				for(int i = 0; i < all.size(); i++) 
				{
					all[i] -= center;
					all[i].rotate(rotationAmount, rotationAngle);
					all[i].z = 0;
					polyline.addVertex(all[i]);
				}
				projected[itr->first] = polyline;
				recognizer.update(polyline);
				
				idealized[itr->first] = recognizer.getPolyline();
				if(recognizer.getFitError() < threshold)
				{
					allMarks.push_back(SpatialMark(center, normal, recognizer.getPolyline()));
				}
			}
		}
	}
	
	void drawHUD()
	{
		if(drawDebug)
		{
			ofPushMatrix();
			ofTranslate(200, 200);
			map<int, ofPolyline>::iterator itr;
			for(itr = projected.begin(); itr != projected.end(); itr++)
			{
				int i = itr->first;
				projected[i].draw();
				idealized[i].draw();
				ofTranslate(0, 200);
			}
			ofPopMatrix();
		}
	}
	
	void draw()
	{	
		ramBeginCamera();
		ofEnableAlphaBlending();
		glDisable(GL_DEPTH_TEST);
		
		ofNoFill();
		list<SpatialMark>::iterator markItr;
		for(markItr = allMarks.begin(); markItr != allMarks.end(); markItr++)
		{
			markItr->draw3d(maxMarkLife);
		}
		
		if(drawDebug)
		{
			ofSetColor(255);
			map<int, list<HistoryPoint> >::iterator itr;
			for(itr = history.begin(); itr != history.end(); itr++)
			{
				list<HistoryPoint>& cur = itr->second;
				ofMesh mesh;
				mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
				list<HistoryPoint>::iterator curItr;
				for(curItr = cur.begin(); curItr != cur.end(); curItr++)
				{
					mesh.addVertex(curItr->point);
				}
				mesh.draw();
			}
			
			map<int, ofVec3f>::iterator centersItr;
			for(centersItr = centers.begin(); centersItr != centers.end(); centersItr++)
			{
				int i = centersItr->first;
				ofVec3f& center = centersItr->second;
				ofVec3f& normal = normals[i];
				//ofLine(center, center + normal * 100);
				
				ofPushMatrix();
				ofTranslate(center);
				rotateToNormal(normal);
				ofNoFill();
				ofRect(projected[i].getBoundingBox());
				ofPopMatrix();
			}
		}
		
		ramEndCamera();
	}
	
	void drawActor(ramActor &actor)
	{	
		for (int i=0; i<actor.getNumNode(); i++)
		{
			if(onlyLimbs)
			{
				if(i != ramActor::JOINT_LEFT_ANKLE &&
				   i != ramActor::JOINT_RIGHT_ANKLE &&
				   i != ramActor::JOINT_LEFT_HAND &&
				   i != ramActor::JOINT_RIGHT_HAND)
				{
					continue;
				}
			}
			list<HistoryPoint>& curHistory = history[i];
			curHistory.push_back(actor.getNode(i).getGlobalPosition());
		}
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
	}
	
	const string getName() { return "Notation"; }
	
};
