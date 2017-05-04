// 
// Chain.h - RAMDanceToolkit
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

//
//  Chain.h
//  Chain
//
//  Created by Onishi Yoshito on 2/19/13.
//
//

#ifndef __Chain__Chain__
#define __Chain__Chain__

#include "ramMain.h"

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
    
    void update(const rdtk::Actor &actor)
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

class Chain : public rdtk::BaseScene
{
public:
    // ------------------------
    void setupControlPanel();
	void setup();
	void update();
	void draw();
    
	// ------------------------
	void drawActor(const rdtk::Actor &actor);
    
    string getName() const { return "Chain"; }
    
private:
	void onKeyPressed(ofKeyEventArgs &e);
    void onValueChanged(ofxUIEventArgs& e);
    
    ChainBtDynamics mChainBtDynamics;
        
    vector<AttachableChain *> mChains;
    
    ofVec3f mGravity;
    
    float mNumEdges;
    float mAttachingEdge;
    float mEdgeLength;
    float mThickness;
};


#endif /* defined(__Chain__Chain__) */
