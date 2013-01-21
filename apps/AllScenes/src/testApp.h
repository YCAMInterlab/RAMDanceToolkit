#pragma once

#include "ofMain.h"
#include "ramMain.h"
#include <numeric>

static const string myActorName = "Ando_2012-09-01_18-49-10";
//static const string myActorName = "default";



class testApp : public ramBaseApp
{
public:

	// of methods
	// ------------------------
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	// ram methods
	// ------------------------
	void drawFloor();
    void drawActor(ramActor &actor);
    void drawRigid(ramRigidBody &rigid);

    
	// ...
	// ------------------------
	ramOscReceiver oscReceiver;
	
};


namespace gl
{
	//ax + by + cz + d = 0;
	static const float kGroundPlaneXUp[] = { 1.0, 0.0, 0.0, 1.0 };
	static const float kGroundPlaneYUp[] = { 0.0, 1.0, 0.0, 1.0 };
	static const float kGroundPlaneZUp[] = { 0.0, 0.0, 1.0, 1.0 };
	
	inline void calcShadowMatrix(const float groundplane[4],
								 const float lightpos[3],
								 float shadowMatrix[16]);
	
	
	void calcShadowMatrix(const float groundplane[],
						  const float lightpos[],
						  float shadowMatrix[])
	{
		float dot = inner_product(groundplane, groundplane+4, lightpos, 0.f);
		
		for(int y = 0; y < 4;++y) {
			for(int x = 0; x < 4; ++x) {
				shadowMatrix[y*4+x] = - groundplane[y]*lightpos[x];
				
				if (x == y) shadowMatrix[y*4+x] += dot;
			}
		}
	}
}
