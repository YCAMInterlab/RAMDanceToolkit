//
//  dpMarionette.h
//  RAMDanceToolkit
//
//  Created by Ovis aries on 2015/01/08.
//
//

#ifndef __RAMDanceToolkit__dpMarionette__
#define __RAMDanceToolkit__dpMarionette__

#include "ramMain.h"
#include "ramConstants.h"
#include "ramUtils.h"

#define DPM_AXIS_X 0
#define DPM_AXIS_Y 1
#define DPM_AXIS_Z 2

class dpMarionette{
public:
	inline static dpMarionette& instance()
	{
		if (__instance == NULL)
			__instance = new dpMarionette;
		return * __instance;
	}

	void drawActor(const ramActor& actor);

	void drawElbowPoint(ofTexture &tex,
						   const ramNode &nodeA,const ramNode &nodeB,
						   const ramNode &nodeC,
						   int width, int resolution,bool isLEFT = false);

	void drawChestPoint(ofTexture &tex,
						const ramNode &nodeA,const ramNode &nodeB,
						const ramNode &nodeC,
						int width, int resolution);
	void drawShoulderPoint(ofTexture &tex,
						   const ramNode &nodeA,const ramNode &nodeB,
						   const ramNode &nodeC,
						   int width, int resolution);
	void drawBasicRibbon(ofTexture &tex,
						 const ramNode &nodeA,const ramNode &nodeB,
						 float width, int resolution,
						 int A_Axis, int B_Axis,
						 bool wired,int beginOffset,int endOffset,
						 ofVec3f beginOffset3v = ofVec3f(0,0,0),
						 ofVec3f endOffset3v = ofVec3f(0,0,0));
	
private:
	static dpMarionette* __instance;
	dpMarionette();
	dpMarionette(const dpMarionette&){};
	dpMarionette& operator=(const dpMarionette&){return *this;};
	~dpMarionette() {};

	ofImage mMarioA_parts[20];
};

#endif /* defined(__RAMDanceToolkit__dpMarionette__) */
