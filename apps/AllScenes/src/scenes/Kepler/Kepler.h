//
//  Kepler.h
//  Kepler
//
//  Created by Onishi Yoshito on 2/18/13.
//
//

#ifndef Kepler_Kepler_h
#define Kepler_Kepler_h

#include "ramBaseScene.h"

#include "btPicker.h"
#include "KeplerBtDynamics.h"
#include "KeplerCube.h"
#include "btBulletDynamicsCommon.h"

class Kepler : public ramBaseScene
{
public:
    // ------------------------
    void setupControlPanel();
	void setup();
	void update();
	void draw();
    
	// ------------------------
	void drawActor(ramActor &actor);
    
    const string getName() { return "Kepler scene"; }
    
private:
    void onValueChanged(ofxUIEventArgs& e);
    
    // ------------------------
    void spawnKepler();
    void attach();
        
    KeplerBtDynamics mKeplerDynamics;
    
    btPicker mPickerA;
    btPicker mPickerB;
    
    int mNodeA,mNodeB;
    int mEdgeA,mEdgeB;
    
    map<string, int> mNamesNodeA;
    map<string, int> mNamesNodeB;
    map<string, int> mNamesEdgeA;
    map<string, int> mNamesEdgeB;
};

#endif
