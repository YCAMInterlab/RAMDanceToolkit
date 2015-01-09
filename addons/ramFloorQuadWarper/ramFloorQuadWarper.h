//
//  ramFloorQuadWarp.h
//  RAMDanceToolkit
//
//  Created by kezzardrix2 on 2015/01/09.
//
//

#ifndef __RAMDanceToolkit__ramFloorQuadWarp__
#define __RAMDanceToolkit__ramFloorQuadWarp__

#include "ofxQuadWarp.h"
#include "ramMain.h"

class ramFloorQuadWarper{
public:
    
    static const int FBO_WIDTH = 1920;
    static const int FBO_HEIGHT = 1200;
    
    static const int FLOOR_WIDTH = 1000;
    static const int FLOOR_HEIGHT = 1000;
    
    inline static ramFloorQuadWarper& instance()
	{
		if (__instance == NULL)
			__instance = new ramFloorQuadWarper;
		return* __instance;
	};
    
    void setupContolPanel(ramBaseScene *scene_, ofVec2f canvasPos = ofVec2f(440,30));
    void guiEvent(ofxUIEventArgs &e);
    
    void setup();
    void begin();
    void end();
    void draw();
    void drawDebug();
    
    ofMatrix4x4 getWarpedMatrix(){return mWarper.getMatrix();};
    
private:
    
    static ramFloorQuadWarper *__instance;
    
    ramFloorQuadWarper() {};
	ramFloorQuadWarper(const ramFloorQuadWarper&){}
	ramFloorQuadWarper& operator=(const ramFloorQuadWarper&) {return *this; }
	~ramFloorQuadWarper() {};
    
    ofxQuadWarp mWarper;
    ofFbo mFbo;
    
    bool isDebug = false;
    
    ofxUICanvasPlus*		parentGui;
	ofxUICanvas*			mGui;
    
    void enableControls();
    void disableControls();
    
    void load();
    void save();
    void reset();
};

#endif /* defined(__RAMDanceToolkit__ramFloorQuadWarp__) */
