//
//  nodeFinder.h
//  RAMDanceToolkit
//
//  Created by motoishmz on 2012/11/06.
//  Copyright (c) 2012年 YCAMInterlab. All rights reserved.
//

#ifndef RAMDanceToolkit_nodeFinder_h
#define RAMDanceToolkit_nodeFinder_h

#include "ramActorManager.h"
#include "ramActor.h"


class ramNodeFinder
{
  
public:
    ramNodeFinder(){}
    ~ramNodeFinder(){}
    
    void start()
    {
        lastFrameNodes.clear();
        ofAddListener(ofEvents().update, this, &ramNodeFinder::update);
    }
    void stop()
    {
        ofRemoveListener(ofEvents().update, this, &ramNodeFinder::update);
    }
    
    
    void update( ofEventArgs &args )
    {
        lastFrameNodes.clear();
        for (int i=0; i<getActorManager().getNumActor(); i++)
        {
            ramActor &o = getActorManager().getActor(i);
            lastFrameNodes.push_back(o);
        }
        for (int i=0; i<getActorManager().getNumRigidBody(); i++)
        {
            ramRigidBody &o = getActorManager().getRigidBody(i);
            lastFrameNodes.push_back(o);
        }
    }
    
    vector<ramNode&> getXXXXXXXNodes();
    vector<ramNode&> getInsideNodes(ofNode hitArea);
    
private:
    vector<ramNodeArray> lastFrameNodes;
    
    
    inline ramActorManager& getActorManager() { return ramActorManager::instance(); }
};


#endif
