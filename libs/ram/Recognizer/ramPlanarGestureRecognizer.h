// 
// ramPlanarGestureRecognizer.h - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "ramBaseRecognizer.h"
#include "ramGeometry.h"
#include "ofxCv.h"

// given a single update(ofPolyline&) this returns an idealized version via getPolyline()
// or a cv::RotatedRect via getRect() describing either the ellipse, or bounding rect in the case of a line

namespace rdtk{
	class PlanarGestureRecognizer : public BaseRecognizer {
	public:
		enum GestureType {GESTURE_LINE, GESTURE_ARC};
		
		PlanarGestureRecognizer();
		void setLineRatio(float lineRatio);
		GestureType getGestureType() const;
		float getFitError() const;
		cv::RotatedRect getRect();
		ofPolyline& getPolyline();
		const ofPolyline& getPolyline() const
		{
			return const_cast<ofPolyline&>(getPolyline());
		}
		void update(ofPolyline& polyline);
		
		string getName() const { return "Planar Gesture Recognizer"; }
		
	protected:
		cv::RotatedRect ellipse, rect;
		ofVec2f linePoint, lineDirection;
		ofPolyline idealized;
		float lineRatio, fitError;
		GestureType gestureType;
	};

}

typedef rdtk::PlanarGestureRecognizer RAMDEPRECATED(ramPlanarGestureRecognizer);
