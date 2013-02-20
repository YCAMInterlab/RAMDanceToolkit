#pragma once
#include <vector>
#include <fstream>
#include "ofMain.h" 

struct Vec2d {
	double x,y;
	Vec2d():x(0),y(0){}
	Vec2d(double nX, double nY):x(nX),y(nY){};
	void set(double nX, double nY) { x = nX; y = nY; };
	double length() {  return sqrtf(x*x + y*y); }
		
	Vec2d operator -(const Vec2d& rOther) {
		Vec2d tmp;
		tmp.x = x - rOther.x;
		tmp.y = y - rOther.y;
		return tmp;
	}
	
	Vec2d operator =(const Vec2d& rOther) {
		x = rOther.x;
		y = rOther.y;
	}
};

struct ofxOneDollarRect {
	double x, y, w, h;
};

struct ofxGesture {
	ofxGesture() {
		golden_ratio = 0.5 * (-1.0 + sqrt(5.0));
		angle_precision = 1.0;
	}
	
	void setName(string sName) {
		name = sName;
	}
	
	void addPoint(double nX, double nY) {
		points.push_back(Vec2d(nX, nY));
	}
	
	void draw() {
		std::vector<Vec2d>::iterator it = points.begin();
		glBegin(GL_LINE_STRIP);
		while(it != points.end()) {
			glVertex2f((*it).x, (*it).y);
			++it;
		}
		glEnd();
		
		glPointSize(4);
		std::vector<Vec2d>::iterator it_res = resampled_points.begin();
		glBegin(GL_POINTS);
		while(it_res != resampled_points.end()) {
			glVertex2f((*it_res).x, (*it_res).y);
			++it_res;
		}
		glEnd();
	}

	void resample(int n) {
		double I = length()/(n - 1);
		double D = 0;
		
		for(int i = 1; i < points.size(); ++i) {
			Vec2d curr = points[i];
			Vec2d prev = points[i-1];
			Vec2d dir = prev - curr;
			double d = dir.length();
			if( (D + d) >= I) {
				double qx = prev.x + ((I-D)/d) * (curr.x - prev.x);
				double qy = prev.y + ((I-D)/d) * (curr.y - prev.y);
				Vec2d resampled(qx, qy);
				resampled_points.push_back(resampled);
				points.insert(points.begin() + i, resampled); 
				D = 0.0;
			}
			else {
				D += d;
			}
		}
		// we had to do some freaky resizing because of rounding issues.
		while(resampled_points.size() <= (n - 1)) {
			resampled_points.push_back(points.back());
		}
		if(resampled_points.size() > n) {
			resampled_points.erase(resampled_points.begin(), resampled_points.begin()+n);
		}
	}
	
	Vec2d centroid() {	
		double x = 0;
		double y = 0;
		std::vector<Vec2d>::iterator it = resampled_points.begin();
		while(it != resampled_points.end()) {
			x += (*it).x;
			y += (*it).y;
			++it;
		}
		x /= resampled_points.size();
		y /= resampled_points.size();
		Vec2d tmp(x,y);
		return tmp;
	}
	
	double length() {
		double len = 0;
		for(int i = 1; i < points.size(); ++i) {
			len += (points[i-1] - points[i]).length();
		}
		return len;
	}	
	
	double indicativeAngle() { 
		Vec2d c = centroid(); // TODO: optimize
		double angle = (c.y - resampled_points[0].y, c.x - resampled_points[0].x);
		return angle;
	}
	
	void rotateToZero() {
		double angle = indicativeAngle();
		undoRotation = angle;
		resampled_points = rotateBy(resampled_points, -angle);
	}
	
	vector<Vec2d> rotateBy(vector<Vec2d> oPoints, double nRad) {
		vector<Vec2d> rotated;
		Vec2d c = centroid(); // TODO: optimize
		center = c; // TODO optimize
		double cosa = cos(nRad);
		double sina = sin(nRad);
		vector<Vec2d>::iterator it = oPoints.begin();
		while(it != oPoints.end()) {
			Vec2d v = (*it);
			double dx = v.x - c.x;
			double dy = v.y - c.y;
			v.x = dx * cosa - dy * sina + c.x;
			v.y = dx * sina + dy * cosa + c.y;
			rotated.push_back(v);
			++it;
		}
		return rotated;
	}
	
	void scaleTo(double nSize = 250.0) {
		ofxOneDollarRect rect = boundingBox();
		undoScaling.set(nSize / rect.w, nSize / rect.h);
		std::vector<Vec2d>::iterator it = resampled_points.begin();
		while(it != resampled_points.end()) {
			Vec2d* v = &(*it);
			v->x = v->x * (nSize/rect.w);
			v->y = v->y * (nSize/rect.h);
			++it;
		};
	}
	
	// translates to origin.
	void translate() {
		Vec2d c = centroid(); //TODO: optimize
		undoTranslation.set(c.x, c.y);
		std::vector<Vec2d>::iterator it = resampled_points.begin();
		while(it != resampled_points.end()) {
			Vec2d* v = &(*it);
			v->x = v->x - c.x;
			v->y = v->y - c.y;
			++it;
		};
	}
	
	ofxOneDollarRect boundingBox() {
		double min_x = FLT_MAX, min_y = FLT_MAX, max_x = FLT_MIN, max_y = FLT_MIN;
		std::vector<Vec2d>::const_iterator it = resampled_points.begin();
		while(it != resampled_points.end()) {
			Vec2d v = (*it);
			if(v.x < min_x) min_x = v.x;
			if(v.x > max_x) max_x = v.x;
			if(v.y < min_y) min_y = v.y;
			if(v.y > max_y) max_y = v.y;
			++it;
		}

		ofxOneDollarRect rect;
		rect.x = min_x;
		rect.y = min_y;
		rect.w = (max_x - min_x);
		rect.h = (max_y - min_y);
		return rect;
	}
	
	double distanceAtBestAngle(ofxGesture* pGesture) {
		double angle_range = PI;
		double start_range = -angle_range;
		double end_range = angle_range;
		double x1 = golden_ratio * start_range + (1.0 - golden_ratio) * end_range;
		double f1 = distanceAtAngle(x1, pGesture);
		double x2 = (1.0 - golden_ratio) * start_range + golden_ratio * end_range;
		double f2 = distanceAtAngle(x2, pGesture);
		while(abs(end_range - start_range) > angle_precision) {	
			if(f1 < f2) {
				end_range = x2;
				x2 = x1;
				f2 = f1;
				x1 =  golden_ratio * start_range + (1.0 - golden_ratio) * end_range;
				f1 = distanceAtAngle(x1, pGesture);
			}
			else {
				start_range = x1;
				x1 = x2;
				f1 = f2;
				x2 = (1.0 - golden_ratio) * start_range + golden_ratio * end_range;
				f2 = distanceAtAngle(x2, pGesture); 
			}
		}
		return min(f1, f2);
	}
	
	double distanceAtAngle(double nAngle, ofxGesture* pGesture) {
		vector<Vec2d> points_tmp = resampled_points;
		points_tmp = rotateBy(points_tmp, nAngle);
		return pathDistance(points_tmp, pGesture);
	}
	
	// distance between two paths.
	double pathDistance(vector<Vec2d> oPoints, ofxGesture* pGesture) {
		// sizes are not equal (?)
		if(oPoints.size() != pGesture->resampled_points.size()) {
			return -1.0;
		}
		double d = 0;
		for(int i = 0; i < resampled_points.size(); ++i) {
			 d += (oPoints[i] - pGesture->resampled_points[i]).length();
		}
		return d/oPoints.size();
	}
	
	
	void normalize(int nNumSamples) {
		resample(nNumSamples);
		rotateToZero();
		scaleTo();
		translate();
	}
	
	void reset() {
		resampled_points.erase(resampled_points.begin(), resampled_points.end());
		points.erase(points.begin(), points.end());
		center.set(0,0);
	}
	
	// serialize and deserialize.
	friend ostream& operator <<(ostream& os, const ofxGesture& rGesture) {
		os << rGesture.name.c_str() << ' ';
		std::vector<Vec2d>::const_iterator it = rGesture.resampled_points.begin();
		while(it != rGesture.resampled_points.end()) {
			os << (*it).x << ';' << (*it).y << ';';
			++it;
		};
		return os;
	}
	
	friend istream& operator >>(istream& is, ofxGesture& rGesture) {
		double x,y;
		istringstream ss;
		is >> rGesture.name;
		while(is) {	
			is >> x; is.ignore(1); is >> y; is.ignore(1);
			if(is) {
				rGesture.resampled_points.push_back(Vec2d(x,y));
			}
		}
		return is;
	}
	
	std::string name;
	Vec2d center;
	std::vector<Vec2d> points;
	std::vector<Vec2d> resampled_points;
	double golden_ratio;
	double angle_precision;
	
	ofVec2f undoTranslation;
	ofVec2f undoScaling;
	float undoRotation;
};


class ofxOneDollar {
public:
	ofxOneDollar() 
		:num_samples(64)
		,square_size(250.0)
		,angle_precision(1.0)
	{
		half_diagonal = 0.5 * sqrt((square_size*square_size) + (square_size*square_size));
	}
	
	void addGesture(ofxGesture* pGesture) {
		// finalize and add.
		pGesture->angle_precision = angle_precision;
		pGesture->normalize(num_samples);
		gestures.push_back(pGesture);
	}
	
	bool save(string sFile) {
		std::ofstream out_file(sFile.c_str(), ios::out | ios::trunc);
		if(!out_file.is_open()) {
			return false;;
		}
		
		std::vector<ofxGesture*>::const_iterator it = gestures.begin();
		while(it != gestures.end()) {
			out_file << *(*it) << std::endl;
			++it;
		}
		out_file.close();
	}
	
	void load(string sFile) {
		std::ifstream in_file(sFile.c_str());
		if(!in_file) {
			ofLog(OF_LOG_ERROR, "Error while loading gesture file: '%s'", sFile.c_str());
			return;
		}
		// TODO: reset first.
		stringstream ss;
		ss << in_file.rdbuf();
		in_file.close();
		string line;
		while(getline(ss, line)) {	
			stringstream iss;
			iss << line;
			ofxGesture* gesture = new ofxGesture();
			iss >> *(gesture);
			gestures.push_back(gesture);
			ofLog(OF_LOG_VERBOSE, "Loaded gesture: '%s' with '%d' samples", gesture->name.c_str(), gesture->resampled_points.size());
		}
	}
	
	ofxGesture* match(ofxGesture* pGesture, double* pScore) {
		double min_dist = FLT_MAX;
		ofxGesture* found_gesture = NULL;
		pGesture->normalize(num_samples);
		std::vector<ofxGesture*>::const_iterator it = gestures.begin();
		while(it != gestures.end()) {
			double dist = (*it)->distanceAtBestAngle(pGesture);
			if(dist > 0 && dist < min_dist) {
				min_dist = dist;
				found_gesture = (*it);
			}
			++it;
		}
		*pScore = 1.0 - (min_dist/half_diagonal);
		return found_gesture;
	}
	
	std::vector<ofxGesture*> gestures;
	double square_size;
	double half_diagonal;
	int num_samples;
	double angle_precision;
};