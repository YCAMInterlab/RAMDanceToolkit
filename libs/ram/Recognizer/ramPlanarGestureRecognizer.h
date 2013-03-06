#pragma once

#include "ramBaseRecognizer.h"
#include "ramGeometry.h"
#include "ofxCv.h"

// given a single update(ofPolyline&) this returns an idealized version via getPolyline()
// or a cv::RotatedRect via getRect() describing either the ellipse, or bounding rect in the case of a line
class ramPlanarGestureRecognizer : public ramBaseRecognizer {	
public:
	enum GestureType {GESTURE_LINE, GESTURE_ARC}; 
	
	ramPlanarGestureRecognizer();
	void setLineRatio(float lineRatio);
	GestureType getGestureType() const;
	float getFitError() const;
	cv::RotatedRect getRect();
	ofPolyline& getPolyline();
	void update(ofPolyline& polyline);
	
	string getName() const { return "Planar Gesture Recognizer"; }
	
protected:
	cv::RotatedRect ellipse, rect;
	ofVec2f linePoint, lineDirection;
	ofPolyline idealized;
	float lineRatio, fitError;
	GestureType gestureType;
};
