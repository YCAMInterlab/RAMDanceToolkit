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
	int maxRadius;
	unsigned char threshold;
	
	CircleFinder()
	:blurRadius(3)
	,threshold(192)
	,maxRadius(6) {
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
		
		centers.clear();
		radii.clear();
		
		// could also just do contour detection here
		unsigned char* pixels = img.getPixels();
		unsigned char* blurredPixels = blurred.getPixels();
		vector<ofVec2f> candidates;
		int searchRadius = maxRadius * 2;
		for(int y = searchRadius; y < img.getHeight() - searchRadius; y++) {
			for(int x = searchRadius; x < img.getWidth() - searchRadius; x++) {
				int i = y * img.getWidth() + x;
				if(blurredPixels[i] > threshold) {
					ofVec2f avg;
					float radius;
					float weightSum = 0;
					for(int yd = -searchRadius; yd < searchRadius; yd++) {
						for(int xd = -searchRadius; xd < searchRadius; xd++) {
							int j = (y + yd) * img.getWidth() + (x + xd);
							unsigned char brightness = pixels[j];
							ofVec2f position(x + xd, y + yd);
							avg += brightness * position;
							weightSum += pixels[j];
						}
					}
					avg /= weightSum;
					candidates.push_back(avg);
				}
			}
		}
		
		// collect multiple matches
		for(int i = 0; i < candidates.size(); i++) {
			ofVec2f& cur = candidates[i];
			bool exists = false;
			for(int j = 0; j < centers.size(); j++) {
				if(cur.distance(centers[j]) < searchRadius) {
					exists = true;
					break;
				}
			}
			if(!exists) {
				centers.push_back(cur);
			}
		}
		
		// this second half estimates the size of the circle
		float areaThreshold = 64;
		for(int i = 0; i < centers.size(); i++) {
			float weightSum = 0;
			ofVec2f positionSum;
			ofVec2f& avg = centers[i]; 
			int xa = avg.x, ya = avg.y;
			float area = 0;
			for(int yd = -searchRadius; yd < searchRadius; yd++) {
				for(int xd = -searchRadius; xd < searchRadius; xd++) {
					int j = (ya + yd) * img.getWidth() + (xa + xd);
					float brightness = (float) pixels[j];
					ofVec2f position(xa + xd, ya + yd);
					float radius = avg.distance(position);
					float weight = brightness;
					positionSum += weight * position;
					weightSum += weight;
					if(brightness > areaThreshold) {
						area += brightness;
					}
				}
			}
			radii.push_back(sqrtf(area / (PI * 255)));
			centers[i] = positionSum / weightSum;
		}
	};
};

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofImage img;
	CircleFinder circleFinder;
};
