#pragma once

#include "ofMain.h"
#include "ofxCv.h"

class CircleFinder {
protected:
	ofImage blurred;
	vector<ofVec2f> centers;
	vector<float> radii;
	
public:
	int blurRadius;
	float minRadius, maxRadius;
	unsigned char threshold;
	
	ofxCv::ContourFinder contourFinder;
	
	CircleFinder()
	:blurRadius(2) // good value to remove kinect speckle
	,threshold(150)
	,minRadius(2)
	,maxRadius(8) {
	}
	
	int size() const {
		return centers.size();
	}
	
	const ofVec2f& getCenter(int i) const {
		return centers[i];
	}
	
	float getRadius(int i) const {
		return radii[i];
	}
	
	template <class T>
	void update(T& img) {
		blur(img, blurred, blurRadius);
		
		contourFinder.setSimplify(false);
		contourFinder.setMinAreaRadius(minRadius - 1);
		contourFinder.setMaxAreaRadius(maxRadius + 1);
		contourFinder.setThreshold(threshold);
		contourFinder.findContours(blurred);
		
		centers.clear();
		radii.clear();
		
		for(int i = 0; i < contourFinder.size(); i++) {
			cv::RotatedRect ellipse = contourFinder.getFitEllipse(i);
			float radius = (ellipse.size.width + ellipse.size.height) / 2.;
			if(radius > minRadius && radius < maxRadius) {
				centers.push_back(ofxCv::toOf(ellipse.center));
				radii.push_back(radius);
			}
		}
	};
};
