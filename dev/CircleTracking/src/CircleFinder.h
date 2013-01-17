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
	
	CircleFinder()
	:blurRadius(3)
	,threshold(192)
	,minRadius(2)
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
		int searchRadius = ceilf(maxRadius * 2);
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
		vector<ofVec2f> proposals;
		for(int i = 0; i < candidates.size(); i++) {
			ofVec2f& cur = candidates[i];
			bool exists = false;
			for(int j = 0; j < proposals.size(); j++) {
				if(cur.distance(proposals[j]) < searchRadius) {
					exists = true;
					break;
				}
			}
			if(!exists) {
				proposals.push_back(cur);
			}
		}
		
		// this second half estimates the size of the circle
		float areaThreshold = 64;
		for(int i = 0; i < proposals.size(); i++) {
			float weightSum = 0;
			ofVec2f positionSum;
			ofVec2f& avg = proposals[i]; 
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
			float radius = sqrtf(area / (PI * 255));
			if(radius < maxRadius && radius > minRadius) {
				radii.push_back(radius);
				centers.push_back(positionSum / weightSum);
			}
		}
	};
};
