#include "CircleSensor.h"

using namespace ofxCv;
using namespace cv;

const float FovX = 1.0144686707507438;
const float FovY = 0.78980943449644714;
const float XtoZ = tanf(FovX / 2) * 2;
const float YtoZ = tanf(FovY / 2) * 2;
const unsigned int Xres = 640;
const unsigned int Yres = 480;

ofVec3f ConvertProjectiveToRealWorld(const ofVec3f& point) {
	return ofVec3f((point.x / Xres - .5f) * point.z * XtoZ,
                   (point.y / Yres - .5f) * point.z * YtoZ,
                   point.z);
}

float distance(ofVec2f reference, vector<ofVec2f>& targets) {
    float minDistance = 0;
    for(int i = 0; i < targets.size(); i++) {
        float distance = reference.distance(targets[i]);
        if(i == 0 || distance < minDistance) {
            minDistance = distance;
        }
    }
    return minDistance;
}

CircleSensor::CircleSensor()
:backgroundClear(false)
,registrationClear(false)
,backgroundCalibrate(false)
,backgroundThreshold(10)
,sampleRadius(1.2)
,deadZoneRadius(24)
,minSamples(6) {
}

void CircleSensor::setup() {
	kinect.init(true, true);
	kinect.open();
	
	string registrationFile = kinect.getSerial() + "-registration.txt";
	if(ofFile::doesFileExist(registrationFile)) {
		ofFile file(registrationFile, ofFile::ReadOnly);
		file >> registration;
	}
	
	imitate(background, kinect, CV_8UC1);
	imitate(valid, kinect, CV_8UC1);
}
void CircleSensor::update() {
    kinect.update();
	if(kinect.isFrameNew()) {
		circleFinder.update(kinect);
		
		int width = kinect.getWidth(), height = kinect.getHeight();
		float* distancePixels = kinect.getDistancePixels(); // distance in millimeters
		
		unsigned char* backgroundPixels = background.getPixels();
		int n = width * height;
		if(backgroundClear) {
			for(int i = 0; i < n; i++) {
				backgroundPixels[i] = 0;
			}
			background.update();
			backgroundClear = false;
		}
		
		if(registrationClear) {
			registrationSamples.clear();
			registrationClear = false;
		}
		
		unsigned char* kinectPixels = kinect.getDepthPixels();
		if(backgroundCalibrate) {
			for(int i = 0; i < n; i++) {
				if(kinectPixels[i] > 0) {
					if(backgroundPixels[i] == 0) {
						backgroundPixels[i] = kinectPixels[i];
					}	else {
						backgroundPixels[i] = (backgroundPixels[i] + kinectPixels[i]) / 2;
					}
				}
			}
			background.update();
		}
		
		unsigned char* validPixels = valid.getPixels();
		for(int i = 0; i < n; i++) {
			int kinectPixel = kinectPixels[i];
			int backgroundPixel = backgroundPixels[i];
			bool far = abs(kinectPixel - backgroundPixel) > backgroundThreshold;
			if(kinectPixel < 255 && kinectPixel > 0 && (backgroundPixel == 0 || (backgroundPixel > 0 && far))) {
				validPixels[i] = 255;
			} else {
				validPixels[i] = 0;
			}
		}
		valid.update();
		
		cloud.clear();
		cloud.setMode(OF_PRIMITIVE_POINTS);
		for(int y = 0; y < height; y++) {
			for(int x = 0; x < width; x++) {
				int i = y * width + x;
				if(validPixels[i] > 0) {
					float z = distancePixels[i];
					ofVec3f cur = ConvertProjectiveToRealWorld(ofVec3f(x, y, z));
					cloud.addVertex(cur);
				}
			}
		}
		
        centers.clear();
        radii.clear();
		trackedPositions.clear();
		for(int i = 0; i < circleFinder.size(); i++) {
			const ofVec2f& center = circleFinder.getCenter(i);
            if(distance(center, deadZones) > deadZoneRadius) {
                ofVec3f sum;
                int count = 0;
                float radius = sampleRadius * circleFinder.getRadius(i);
                for(int y = -radius; y < +radius; y++) {
                    for(int x = -radius; x < +radius; x++) {
                        if(ofDist(0, 0, x, y) <= radius) {
                            int curx = center.x + x;
                            int cury = center.y + y;
                            if(curx > 0 && curx < width &&
                               cury > 0 && cury < height) {
                                int i = cury * width + curx;
                                if(validPixels[i] > 0) {
                                    sum.x += curx;
                                    sum.y += cury;
                                    sum.z += distancePixels[i];
                                    count++;
                                }
                            }
                        }
                    }
                }
                if(count > minSamples) {
                    centers.push_back(center);
                    radii.push_back(circleFinder.getRadius(i));
                    trackedPositions.push_back(ConvertProjectiveToRealWorld(sum / count));
                }
            }
		}
	}
}
void CircleSensor::drawCloud() {
	ofPushMatrix();
	ofPushStyle();
	
	ofMultMatrix(registration);
	cloud.draw();
	ofFill();
	ofSetColor(cyanPrint, 128);
	for(int i = 0; i < trackedPositions.size(); i++) {
		ofBox(trackedPositions[i], 8);
	}
    
	ofSetColor(magentaPrint, 128);
	ofMesh registrationLine;
	registrationLine.setMode(OF_PRIMITIVE_LINE_STRIP);
	registrationLine.addVertices(registrationSamples);
	registrationLine.draw();
	ofPopMatrix();
    
    ofSetColor(yellowPrint);
	registrationLine.clear();
	registrationLine.addVertices(registrationSamples);
	registrationLine.draw();
	ofPopStyle();	
}
void CircleSensor::drawDebug() {
	ofPushStyle();
	ofPushMatrix();
	ofSetColor(255);
	kinect.draw(0, 0);
	kinect.drawDepth(640, 0, 320, 240);
	background.draw(640, 240, 320, 240);
	
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofSetColor(yellowPrint);
	valid.draw(0, 0);
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	
    ofFill();
	for(int i = 0; i < deadZones.size(); i++) {
        ofSetColor(magentaPrint);
        ofCircle(deadZones[i], deadZoneRadius);
    }
	ofNoFill();
	for(int i = 0; i < trackedPositions.size(); i++) {
        ofSetColor(magentaPrint);
        ofCircle(centers[i], radii[i]);
        ofSetColor(cyanPrint);
        ofCircle(centers[i], radii[i] * sampleRadius);
        ofDrawBitmapString(ofToString(trackedPositions[i]), centers[i]);
	}
	for(int i = 0; i < circleFinder.size(); i++) {
        ofSetColor(yellowPrint, 128);
        ofCircle(circleFinder.getCenter(i), circleFinder.getRadius(i));
	}
	ofPopMatrix();
	ofPopStyle();
}
void CircleSensor::updateRegistration(ofVec3f& reference, float accuracy) {
    registrationSamples.push_back(trackedPositions[0]);
    referenceSamples.push_back(reference);
    registration = estimateAffine3D(registrationSamples, referenceSamples, accuracy);
    ofFile file(kinect.getSerial() + "-registration.txt", ofFile::WriteOnly);
    file << registration;
}
bool CircleSensor::oneTrackedPosition() {
	return trackedPositions.size() == 1 && (trackedPositions[0].distance(ofVec3f()) > 1);
}
void CircleSensor::setDeadZones() {
    deadZones = circleFinder.getCenters();
}
CircleSensor::~CircleSensor() {
	kinect.close();
}