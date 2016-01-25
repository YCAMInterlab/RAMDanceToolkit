//
//  lineDrawing_ext.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2016/01/24.
//
//

#ifndef lineDrawing_ext_h
#define lineDrawing_ext_h

#include "ramNodeLine.h"
#include "ramMotionExtractor.h"

class LineDrawing_ext : public ramBaseScene
{

public:
	string getName() const {return "Line_ext";}
	struct LineContext
	{
		float curve, spiral_radius, spiral_num_rotate;
		float noise_scale, noise_freq;
		float extend_from, extend_to, line_width;
		
		ofFloatColor color;
		ramNodeLine nodeLine;

		bool active;
		int id;
		
		void setup()
		{
			active = true;
			curve = spiral_radius = spiral_num_rotate =
			noise_scale = noise_freq = extend_from = extend_to = 0.0;
			line_width = 2.0;
			color.set(1.0,1.0,1.0,1.0);
		}
		
		void draw()
		{
			if (!active) return;
			nodeLine.curve(curve);
			nodeLine.resampling(0.3);
			
			if (extend_from > 0) nodeLine.extendFrom(extend_from);
			if (extend_to > 0) nodeLine.extendTo(extend_to);
			if (spiral_radius > 0) nodeLine.spiral(spiral_radius, spiral_num_rotate);
			if (noise_scale > 0) nodeLine.noise(noise_scale, noise_freq, ofGetFrameNum() / 60.0f);
			
			ofSetColor(color);
			ofSetLineWidth(line_width);
			nodeLine.draw();
			
		}
		
		void randomize()
		{
			
			ramActorManager &AM = ramActorManager::instance();
			const vector<string>& names = AM.getNodeArrayNames();
			
			nodeLine.from.index = ofRandom(23);
			nodeLine.from.name = names[rand() % names.size()];
			
			nodeLine.control0.index = ofRandom(23);
			nodeLine.control0.name = names[rand() % names.size()];
			
			nodeLine.to.index = ofRandom(23);
			nodeLine.to.name = names[rand() % names.size()];
			
			nodeLine.control1.index = ofRandom(23);
			nodeLine.control1.name = names[rand() % names.size()];
		}
	};
	
	enum
	{
		NUM_LINE = 3
	};
	
	LineContext lines[NUM_LINE];
	
	ramMotionExtractor mex;
	
	void setupControlPanel();
	void onValueChanged(ofxUIEventArgs &e);
	void update();
	void draw();
	void exit();
	
};

#endif /* lineDrawing_ext_h */
