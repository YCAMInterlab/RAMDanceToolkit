#include "ramPlanarGestureRecognizer.h"

ramPlanarGestureRecognizer::ramPlanarGestureRecognizer()
:lineRatio(6)
,gestureType(GESTURE_LINE)
,fitError(0)
{}
void ramPlanarGestureRecognizer::setLineRatio(float lineRatio) {
	this->lineRatio = lineRatio;
}
ramPlanarGestureRecognizer::GestureType ramPlanarGestureRecognizer::getGestureType() const {
	return gestureType;
}
float ramPlanarGestureRecognizer::getFitError() const {
	return fitError;
}
cv::RotatedRect ramPlanarGestureRecognizer::getRect() {
	return gestureType == GESTURE_LINE ? rect : ellipse;
}
ofPolyline& ramPlanarGestureRecognizer::getPolyline() {
	return idealized;
}
void ramPlanarGestureRecognizer::update(ofPolyline& polyline) {
	if(polyline.size() > 5) {
		ellipse = ofxCv::fitEllipse(polyline);
	}
	rect = ofxCv::minAreaRect(polyline);
	ofxCv::fitLine(polyline, linePoint, lineDirection);
	
	float lineSum = 0, ellipseSum = 0;
	for(int i = 0; i < polyline.size(); i++) {
		lineSum += distanceToRay(polyline[i], linePoint, linePoint + lineDirection);
		ellipseSum += distanceToEllipse(polyline[i], ellipse);
	}
	float perimeter = polyline.getPerimeter();
	lineSum /= perimeter, ellipseSum /= perimeter;
	
	bool isLine = rect.size.width / rect.size.height > lineRatio
	|| rect.size.height / rect.size.width > lineRatio
	|| (lineSum < ellipseSum)
	|| ellipseSum != ellipseSum;
	idealized.clear();
	if(isLine) {
		gestureType = GESTURE_LINE;
		fitError = lineSum;
		idealized.addVertex(closestPointOnRay(linePoint, linePoint + lineDirection, polyline[0]));
		idealized.addVertex(closestPointOnRay(linePoint, linePoint + lineDirection, polyline[polyline.size() - 1]));
	} else {
		gestureType = GESTURE_ARC;
		fitError = ellipseSum;
		ofVec2f center(ellipse.center.x, ellipse.center.y);
		// it would make more sense to do this at a fixed resolution
		for(int i = 0; i < polyline.size(); i++) {
			ofVec2f cur = polyline[i];
			cur -= center, cur.rotate(-ellipse.angle);
			float a = ellipse.size.width / 2, b = ellipse.size.height / 2, x0 = cur.x, y0 = cur.y;
			cur *= (a * b) / sqrtf(a * a * y0 * y0 + b * b * x0 * x0);
			cur.rotate(ellipse.angle), cur += center;
			idealized.addVertex(cur);
		}
	}
}