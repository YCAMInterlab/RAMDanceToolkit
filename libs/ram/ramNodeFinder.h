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
    
    vector<ramNode> findNode(int jointId)
    {
        vector<ramNode> nodes;
        for (int i=0; i<getActorManager().getNumActor(); i++)
        {
            ramNode &node = getActorManager().getActor(i).getNode(jointId);
            nodes.push_back(node);
        }
        return nodes;
    }
    
    ramNode& findNode(const string actoreName, int jointId)
    {
        return getActorManager().getActor(actoreName).getNode(jointId);
    }
    
private:
    
    inline ramActorManager& getActorManager() { return ramActorManager::instance(); }
};


#endif
