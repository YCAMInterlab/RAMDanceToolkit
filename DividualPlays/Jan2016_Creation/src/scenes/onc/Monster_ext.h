//
//  Monster_ext.h
//  RAMDanceToolkit
//
//  Created by YoshitoONISHI on 1/27/16.
//
//

#ifndef Monster_ext_h
#define Monster_ext_h

#include "ramMain.h"
#include "ramMotionExtractor.h"

class Monster_ext : public ramBaseScene
{
public:
    void setupControlPanel();
    
    void setup();
    
    void reset();
    
    bool isAncestor(int ancestor, int child);
    
    vector<int> getNonChildren(int ancestor);
    
    int getRandomNonChild(int i);
    
    void attach(int child, int parent);
    
    void update();
    
    void draw();
    
    void drawActor(const ramActor &actor);
    
    void drawRigid(const ramRigidBody &rigid)
    {
    }
	
	void onValueChanged(ofxUIEventArgs &e)
	{
		ofxUIWidget* w = e.widget;
		if (w->getName() == "Randomize Topology")
			randomizeTopology = true;
		
		if (w->getName() == "Randomize Geometry")
			randomizeGeometry = true;
	}
	
    string getName() const { return "Monster_ext"; }
    
    ramMotionExtractor mex;
    vector<int> treeBase, treeSwap;
    vector<float> lengthScale;
    ramNodeArray monsterArray;
    float minScale, maxScale, randomizationAmount;
    bool needToReset, randomLine, randomizeTopology, randomizeGeometry;
};


#endif /* Monster_ext_h */
