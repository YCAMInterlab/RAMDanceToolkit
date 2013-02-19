//
//  ChainScene.h
//  Chain
//
//  Created by Onishi Yoshito on 2/19/13.
//
//

#ifndef __Chain__ChainScene__
#define __Chain__ChainScene__

#include "ramBaseScene.h"

#include "btPicker.h"
#include "ChainBtDynamics.h"
#include "ChainObject.h"
#include "btBulletDynamicsCommon.h"

// ------------------------
class AttachableChain {
public:
    // ------------------------
    AttachableChain() :
    mDynamics(NULL), mChain(NULL), mNEdges(0), mActorName(""), mActorNodeId(0)
    {}
    
    void setup(ChainBtDynamics *dynamics)
    {
        mDynamics = dynamics;
        mPicker.setWorld(mDynamics->getDynamicsWorld());
    }
    
    void spawnChain(const btVector3 &origin, int nEdges, float length, float thickness)
    {
        if (!mDynamics) return;
        
        mNEdges = nEdges;
        mChain = mDynamics->spawnChain(origin, nEdges, length, thickness);
    }
    
    void attach(string actorName, int actorNodeId, int chainEdgeId)
    {
        if (!mDynamics) return;
        if (chainEdgeId<0) return;
        
        mActorName = actorName;
        mActorNodeId = actorNodeId;
        
        if (chainEdgeId < mNEdges) {
            btRigidBody *bd = mChain->m_bodies.at(chainEdgeId);
            mPicker.attatchRigidBody(bd);
        }
        else {
            btRigidBody *bd = mChain->m_bodies.at(mNEdges-1);
            mPicker.attatchRigidBody(bd);
        }
    }
    
    void update(const ramActor &actor)
    {
        if (actor.getName()==mActorName) {
            const ofVec3f pos = actor.getNode(mActorNodeId).getGlobalPosition();
            mPicker.updatePosition(btVector3(pos.x, pos.y, pos.z));
        }
    }
    
    void setActorInfo(const string &name, int nodeId)
    {
        mActorName = name;
        mActorNodeId = nodeId;
    }
    
    int getNEdges() const { return mNEdges; }
    
private:
    // ------------------------
    ChainBtDynamics *mDynamics;
    BaseConstrains *mChain;
    int mNEdges;
    btPicker mPicker;
    
    string mActorName;
    int mActorNodeId;
};
// ------------------------

class ChainScene : public ramBaseScene
{
public:
    // ------------------------
    void setupControlPanel(ofxUICanvas* panel);
	void setup();
	void update();
	void draw();
    
	// ------------------------
    void drawActor(ramActor &actor);
    
    const string getName() { return "Chain scene"; }
    
private:
    void onValueChanged(ofxUIEventArgs& e);
    
    ChainBtDynamics mChainBtDynamics;
        
    vector<AttachableChain *> mChains;
    
    ofVec3f mGravity;
    
    float mNumEdges;
    float mAttachingEdge;
    float mEdgeLength;
    float mThickness;
};


#endif /* defined(__Chain__ChainScene__) */
