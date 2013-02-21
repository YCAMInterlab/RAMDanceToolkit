#pragma once

#include "ramMain.h"
#include "ofxOneDollar.h"
#include "ofxCv.h"

#include "Fading.h"

class HistoryPoint : public Fading
{
public:
	ofVec3f point;
	float birth;
	
	HistoryPoint(ofVec3f point)
	:point(point)
	{}
};

// needs improvement. right now it just looks for the biggest cross product
void approximatePlane(const vector<ofVec3f>& points, int iterations, ofVec3f& center, ofVec3f& normal)
{
	int n = points.size();
	for(int i = 0; i < n; i++)
	{
		center += points[i];
	}
	center /= n;
	float maxLength = 0;
	for(int i = 0; i < n; i++)
	{
		ofVec3f side1 = points[i] - center;
		for(int j = i + 1; j < n; j++)
		{
			ofVec3f side2 = points[j] - center;
			ofVec3f curNormal = side1.getCrossed(side2);
			if(curNormal.z < 0) {
				curNormal *= -1;
			}
			float length = curNormal.length();
			if(length > maxLength)
			{
				normal = curNormal;
				maxLength = length;
			}
		}
	}
	normal.normalize();
}

float angleCompleteness(const ofPolyline& polyline, const ofVec2f& center, int bins = 12) 
{
	vector<bool> allBins(bins, false);
	int filledBins = 0;
	for(int i = 0; i < polyline.size(); i++)
	{
		float angle = center.angle(polyline[i]);
		int bin = ofClamp(ofMap(angle, -180, 180, 0, bins), 0, bins - 1);
		if(!allBins[bin])
		{
			filledBins++;
		}
		allBins[bin] = true;
	}
	return (float) filledBins / (float) bins;
}

class SpatialEllipse : public Fading
{
public:
	ofVec3f position, normal;
	cv::RotatedRect ellipse;
	
	SpatialEllipse(ofVec3f position, ofVec3f normal, cv::RotatedRect ellipse)
	:position(position)
	,normal(normal)
	,ellipse(ellipse)
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
		ofTranslate(ellipse.center.x, ellipse.center.y);
		ofRotate(ellipse.angle);
		ofSetColor(255, 255 * getLife(lifespan));
		ofEllipse(0, 0, ellipse.size.width, ellipse.size.height);
		ofPopMatrix();
	}
};

class Notation : public ramBaseScene
{
public:
	
	map<int, list<HistoryPoint> > history;
	map<int, vector<ofVec3f> > projected;
	map<int, ofRectangle > boundingBoxes;
	map<int, ofVec3f> centers, normals;
	map<int, double> scores;
	map<int, ofxGesture*> matches;
	
	map<int, ofVec2f> undoTranslation;
	map<int, ofVec2f> undoScaling;
	map<int, float> undoRotation;
	map<int, ofPolyline> normalized;
	map<int, cv::RotatedRect> ellipses;
	map<int, float> completeness;
	
	list<SpatialEllipse> allEllipses;
	
	
	float maxNotationLife, maxEllipseLife;
	float threshold;
	bool onlyLimbs, drawDebug;
	
	ofxOneDollar dollar;
	ofxGesture *circleGesture, *squareGesture, *uGesture, *lineGesture;
	
	float centerLerpRate, normalLerpRate;
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		onlyLimbs = true;
		maxNotationLife = 1;
		maxEllipseLife = 5;
		centerLerpRate = .1;
		normalLerpRate = .1;
		threshold = .9;
		drawDebug = false;
		panel->addToggle("Only limbs", &onlyLimbs, 20, 20);
		panel->addToggle("Draw debug", &drawDebug, 20, 20);
		panel->addSlider("History fade out", 0, 5, &maxNotationLife, 300, 20);
		panel->addSlider("Ellipse fade out", 0, 10, &maxEllipseLife, 300, 20);
		panel->addSlider("Threshold", 0, 1, &threshold, 300, 20);
		panel->addSlider("Center lerp rate", 0, 1, &centerLerpRate, 300, 20);
		panel->addSlider("Normal lerp rate", 0, 1, &normalLerpRate, 300, 20);
	}
	
	void setup()
	{
		// note: clockwise vs counterclockwise gestures
		
		circleGesture = new ofxGesture();
		circleGesture->setName("Circle");
		circleGesture->reset();
		for(int i = 0; i < 64; i++) 
		{
			float theta = ofMap(i, 0, 64, 0, TWO_PI);
			circleGesture->addPoint(cos(theta), sin(theta));
		}
		dollar.addGesture(circleGesture);
		
		squareGesture = new ofxGesture();
		squareGesture->setName("Square");
		squareGesture->reset();
		for(int i = 0; i < 4; i++) 
		{
			for(int j = 0; j < 16; j++) 
			{
				float t = ofMap(j, 0, 16, -1, +1);
				switch(i) 
				{
					case 0: squareGesture->addPoint(t, -1); break;
					case 1: squareGesture->addPoint(+1, t); break;
					case 2: squareGesture->addPoint(-t, +1); break;
					case 3: squareGesture->addPoint(-1, -t); break;
				}
			}
		}
		dollar.addGesture(squareGesture);
		
		uGesture = new ofxGesture();
		uGesture->setName("U");
		uGesture->reset();
		for(int i = 0; i < 64; i++) 
		{
			float theta = ofMap(i, 0, 64, PI, TWO_PI);
			uGesture->addPoint(cos(theta), sin(theta));
		}
		dollar.addGesture(uGesture);
		
		lineGesture = new ofxGesture();
		lineGesture->setName("Line");
		lineGesture->reset();
		for(int i = 0; i < 16; i++) 
		{
			float t = ofMap(i, 0, 16, -1, +1);
			lineGesture->addPoint(i, 0);
		}
		dollar.addGesture(lineGesture);
	}
	
	void update()
	{
		Fading::bury(allEllipses, maxEllipseLife);
		
		// remove any dead points
		map<int, list<HistoryPoint> >::iterator itr;
		for(itr = history.begin(); itr != history.end(); itr++)
		{
			list<HistoryPoint>& cur = itr->second;
			Fading::bury(cur, maxNotationLife);
			vector<ofVec3f> all;
			list<HistoryPoint>::iterator curItr;
			for(curItr = cur.begin(); curItr != cur.end(); curItr++)
			{
				all.push_back(curItr->point);
			}
			if(all.size() > 5) {
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
				for(int i = 0; i < all.size(); i++) 
				{
					all[i] -= center;
					all[i].rotate(rotationAmount, rotationAngle);
					all[i].z = 0;
				}
				projected[itr->first] = all;
				
				ofPolyline polyline;
				polyline.addVertices(all);
				boundingBoxes[itr->first] = polyline.getBoundingBox();
				
				/*
				 ofxGesture* gesture = new ofxGesture();
				 for(int i = 0; i < all.size(); i++) 
				 {
				 gesture->addPoint(all[i].x, all[i].y);
				 }
				 double score = 0;
				 ofxGesture* match = dollar.match(gesture, &score);
				 scores[itr->first] = score;
				 matches[itr->first] = match;
				 undoTranslation[itr->first] = gesture->undoTranslation;
				 undoScaling[itr->first] = gesture->undoScaling;
				 undoRotation[itr->first] = gesture->undoRotation;
				 normalized[itr->first] = gesture->getNormalized();
				 delete gesture;
				 */
				
				cv::RotatedRect ellipse = ofxCv::fitEllipse(polyline);
				ellipses[itr->first] = ellipse;
				completeness[itr->first] = angleCompleteness(polyline, ofVec2f(ellipse.center.x, ellipse.center.y));
				if(completeness[itr->first] > threshold)
				{
					allEllipses.push_back(SpatialEllipse(center, normal, ellipse));
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
			map<int, vector<ofVec3f> >::iterator itr;
			for(itr = projected.begin(); itr != projected.end(); itr++)
			{
				ofMesh mesh;
				mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
				mesh.addVertices(itr->second);
				mesh.draw();
				int i = itr->first;
				ofPushMatrix();
				if(matches[i] != NULL) // && scores[i] > threshold)
				{
					ofDrawBitmapString(matches[i]->name + " " + ofToString(scores[i]), 10, 20);
					/*
					 ofTranslate(undoTranslation[i]);
					 ofScale(1/undoScaling[i].x, 1/undoScaling[i].y);
					 ofRotate(-ofRadToDeg(undoRotation[i]));
					 */
					normalized[i].draw();
					matches[i]->draw();
				}
				
				if(completeness[itr->first] > threshold)
				{
					ofTranslate(ellipses[i].center.x, ellipses[i].center.y);
					ofRotate(ellipses[i].angle);
					ofNoFill();
					ofEllipse(0, 0, ellipses[i].size.width, ellipses[i].size.height);
					//ofRect(-10, -10, 20, 20);
				}
				ofPopMatrix();
				
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
		list<SpatialEllipse>::iterator ellipseItr;
		for(ellipseItr = allEllipses.begin(); ellipseItr != allEllipses.end(); ellipseItr++)
		{
			ellipseItr->draw3d(maxEllipseLife);
		}
		
		ofSetColor(255);
		if(drawDebug)
		{
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
				ofRect(boundingBoxes[i]);
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
