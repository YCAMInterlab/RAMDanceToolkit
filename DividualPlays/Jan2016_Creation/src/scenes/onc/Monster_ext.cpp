//
//  Monster_ext.cpp
//  RAMDanceToolkit
//
//  Created by YoshitoONISHI on 1/27/16.
//
//

#include "Monster_ext.h"
#include "ramCenteredActor.h"

void Monster_ext::setupControlPanel()
{
#ifdef RAM_GUI_SYSTEM_OFXUI
    
    needToReset = false;
    randomizeTopology = false;
    randomizeGeometry = false;
    randomLine = false;
    minScale = .5;
    maxScale = 2;
    randomizationAmount = .15;
    ofxUICanvas* panel = ramGetGUI().getCurrentUIContext();
    panel->addToggle("Reset", &needToReset, 20, 20);
    panel->addToggle("Random Line", &randomLine, 20, 20);
    panel->addToggle("Randomize Topology", &randomizeTopology, 20, 20);
    panel->addToggle("Randomize Geometry", &randomizeGeometry, 20, 20);
    panel->addSlider("Min scale", 0, 4, &minScale, 200, 20);
    panel->addSlider("Max scale", 0, 4, &maxScale, 200, 20);
    panel->addSlider("Randomization amount", 0, 1, &randomizationAmount, 200, 20);
    
    mex.setupControlPanel(this);
	
#endif
}

void Monster_ext::setup()
{
    int treeArray[] = {
        -1,
        ramActor::JOINT_HIPS,
        ramActor::JOINT_ABDOMEN,
        ramActor::JOINT_CHEST,
        ramActor::JOINT_NECK,
        ramActor::JOINT_HIPS,
        ramActor::JOINT_LEFT_HIP,
        ramActor::JOINT_LEFT_KNEE,
        ramActor::JOINT_LEFT_ANKLE,
        ramActor::JOINT_HIPS,
        ramActor::JOINT_RIGHT_HIP,
        ramActor::JOINT_RIGHT_KNEE,
        ramActor::JOINT_RIGHT_ANKLE,
        ramActor::JOINT_CHEST,
        ramActor::JOINT_LEFT_COLLAR,
        ramActor::JOINT_LEFT_SHOULDER,
        ramActor::JOINT_LEFT_ELBOW,
        ramActor::JOINT_LEFT_WRIST,
        ramActor::JOINT_CHEST,
        ramActor::JOINT_RIGHT_COLLAR,
        ramActor::JOINT_RIGHT_SHOULDER,
        ramActor::JOINT_RIGHT_ELBOW,
        ramActor::JOINT_RIGHT_WRIST
    };
    treeBase.resize(ramActor::NUM_JOINTS);
    lengthScale.resize(ramActor::NUM_JOINTS, 1);
    for(int i = 0; i < treeBase.size(); i++) {
        treeBase[i] = treeArray[i];
    }
    reset();
	randomizeTopology = true;
	int n = treeSwap.size();
	for(int i = 0; i < n; i++)
	{
		if(treeSwap[i] != -1 && ofRandom(1) < randomizationAmount)
		{
			treeSwap[i] = getRandomNonChild(i);
		}
	}
}

void Monster_ext::reset()
{
    treeSwap = treeBase;
    lengthScale.clear();
    lengthScale.resize(ramActor::NUM_JOINTS, 1);
}

bool Monster_ext::isAncestor(int ancestor, int child) {
    int cur = child;
    while(treeSwap[cur] != -1)
    {
        if(cur == ancestor)
        {
            return true;
        }
        cur = treeSwap[cur];
    }
    return false;
}

vector<int> Monster_ext::getNonChildren(int ancestor)
{
    vector<int> nonchildren;
    int n = ramActor::NUM_JOINTS;
    for(int child = 0; child < n; child++)
    {
        if(!isAncestor(ancestor, child))
        {
            nonchildren.push_back(child);
        }
    }
    return nonchildren;
}

int Monster_ext::getRandomNonChild(int i)
{
    vector<int> nonchildren = getNonChildren(i);
    int nonchild = nonchildren[ofRandom(0, nonchildren.size())];
    return nonchild;
}

void Monster_ext::attach(int child, int parent)
{
    treeSwap[child] = parent;
}

void Monster_ext::update()
{
    mex.update();
    
    if(needToReset)
    {
        reset();
        needToReset = false;
    }
    if(randomizeTopology)
    {
        reset();
        int n = treeSwap.size();
        for(int i = 0; i < n; i++)
        {
            if(treeSwap[i] != -1 && ofRandom(1) < randomizationAmount)
            {
                treeSwap[i] = getRandomNonChild(i);
            }
        }
		randomizeTopology = false;
    }
    if(randomizeGeometry)
    {
        int n = lengthScale.size();
        for(int i = 0; i < n; i++)
        {
            if(ofRandom(1) < randomizationAmount)
            {
                lengthScale[i] = ofRandom(minScale, maxScale);
            } else {
                lengthScale[i] = 1;
            }
        }
		randomizeGeometry = false;
    }
    if(randomLine)
    {
        reset();
        vector<int> all;
        for(int i = 1; i < ramActor::NUM_JOINTS; i++)
        {
            all.push_back(i);
        }
        ofRandomize(all);
        attach(all[0], ramActor::JOINT_HIPS);
        for(int i = 1; i < all.size(); i++)
        {
            attach(all[i], all[i - 1]);
        }
        randomLine = false;
    }
}

void Monster_ext::draw()
{
}

void Monster_ext::drawActor(const ramActor &actor)
{
    bool bActEnable = false;
    for (int q = 0;q < mex.getNumPort();q++)
        if (mex.getActorNameAt(q) == actor.getName()) bActEnable = true;
    
    if (!bActEnable) return;
    
    monsterArray = actor;
    
    for (int i=0; i < treeSwap.size(); i++)
    {
        if(treeSwap[i] != -1)
        {
            monsterArray.getNode(i).setParent(monsterArray.getNode(treeSwap[i]));
            monsterArray.getNode(i).setScale(lengthScale[i]);
        }
    }
    
    ofPushStyle();
    ofNoFill();
    for (int i=0; i<monsterArray.getNumNode(); i++)
    {
        ramNode &node = monsterArray.getNode(i);
        ofSetColor(255);
        ofSetLineWidth(2);
        if(node.hasParent())
        {
            ofLine(node, *node.getParent());
        }
    }
    ofPopStyle();
}

void Monster_ext_2::setupControlPanel(){
    Monster_ext::setupControlPanel();
    
    ofxUICanvas* panel = ramGetGUI().getCurrentUIContext();
    panel->addSlider("x",-1000, 1000, &mTrans.x, 200, 20);
    panel->addSlider("y",-1000, 1000, &mTrans.y, 200, 20);
    panel->addSlider("z",0, 2000, &mTrans.z, 200, 20);
    panel->addToggle("Centered", &mIsCentered);
    
    mIsCentered = true;
    
   // mTrans.y = 140.0;
   // mTrans.z = 500;
}

void Monster_ext_2::drawActor(const ramActor &actor)
{
    bool bActEnable = false;
    for (int q = 0;q < mex.getNumPort();q++)
        if (mex.getActorNameAt(q) == actor.getName()) bActEnable = true;
    
    if (!bActEnable) return;
        
    if(mIsCentered)monsterArray = mCentered.update(actor);
    else monsterArray = actor;
    
    for (int i=0; i < treeSwap.size(); i++)
    {
        if(treeSwap[i] != -1)
        {
            monsterArray.getNode(i).setParent(monsterArray.getNode(treeSwap[i]));
            monsterArray.getNode(i).setScale(lengthScale[i]);
        }
    }
    
    ofPushMatrix();
    ofTranslate(mTrans);
    ofPushStyle();
    ofNoFill();
    for (int i=0; i<monsterArray.getNumNode(); i++)
    {
        ramNode &node = monsterArray.getNode(i);
        ofSetColor(255);
        ofSetLineWidth(2);
        if(node.hasParent())
        {
            ofLine(node, *node.getParent());
        }
    }
    ofPopStyle();
    ofPopMatrix();
}