#pragma once

#include "ramActorManager.h"
#include "ramActor.h"
#include "ramNodeIdentifer.h"

class ramNodeFinder : public ramNodeIdentifer
{
public:
	
    ramNodeFinder() : ramNodeIdentifer() {}
	ramNodeFinder(const ramNodeIdentifer& copy) : ramNodeIdentifer(copy) {}
	
	void setActorName(string name_) { name = name_; }
	void setJointID(int index_) { index = index_; }
	
	inline bool found()
	{
		if (!getActorManager().hasNodeArray(name)) return false;
		return ofInRange(index, 0, getActorManager().getNodeArray(name).getNumNode());
	}
	
	// safety API
	bool get(ramNode &node)
    {
		if (!getActorManager().hasNodeArray(name))
		{
			if (getActorManager().getNumNodeArray() > 0)
			{
				node = getActorManager().getNodeArray(0).getNode(index);
				return true;
			}
			return false;
		}
		else
		{
			node = getActorManager().getNodeArray(name).getNode(index);
			return true;
		}
    }
	
	//
	
	vector<ramNode> get()
	{
		vector<ramNode> nodes;
		
		bool has_target_actor = !name.empty();
		bool has_target_node = index != -1;
		
		for (int i = 0; i < getActorManager().getNumNodeArray(); i++)
        {
			ramNodeArray &actor = getActorManager().getNodeArray(i);
			
			if (has_target_actor && name != actor.getName()) continue;
			
			for (int n = 0; n < actor.getNumNode(); n++)
			{
				if (has_target_node && index != n) continue;
				
				ramNode &node = actor.getNode(n);
				nodes.push_back(node);
			}
        }
		
		return nodes;
	}
	
	vector<ramNode*> getPtr()
	{
		vector<ramNode*> nodes;
		
		bool has_target_actor = !name.empty();
		bool has_target_node = index != -1;
		
		for (int i = 0; i < getActorManager().getNumNodeArray(); i++)
        {
			ramNodeArray &actor = getActorManager().getNodeArray(i);
			
			if (has_target_actor && name != actor.getName()) continue;
			
			for (int n = 0; n < actor.getNumNode(); n++)
			{
				if (has_target_node && index != n) continue;
				
				ramNode &node = actor.getNode(n);
				nodes.push_back(&node);
			}
        }
		
		return nodes;
	}
	
	//
	
    static vector<ramNode> findNodes(int jointId)
    {
        vector<ramNode> nodes;
		
        for (int i=0; i<getActorManager().getNumNodeArray(); i++)
        {
            ramNode &node = getActorManager().getNodeArray(i).getNode(jointId);
            nodes.push_back(node);
        }
		
        return nodes;
    }
	
	static vector<ramNode> findNodes(const string& actoreName)
    {
        vector<ramNode> nodes;
		ramNodeArray &actor = getActorManager().getNodeArray(actoreName);
		
        for (int i=0; i<actor.getNumNode(); i++)
        {
            ramNode &node = actor.getNode(i);
            nodes.push_back(node);
        }
		
        return nodes;
    }
    
	
    static const ramNode& findNode(const string& actoreName, int jointId)
    {
        return getActorManager().getNodeArray(actoreName).getNode(jointId);
    }
	
	static const ramNode& findNode(const ramNodeIdentifer &identifr)
	{
		return findNode(identifr.name, identifr.index);
	}
	
	// safety API
	static bool findNode(const string& actoreName, int jointId, ramNode &node)
    {
		if (!getActorManager().hasNodeArray(actoreName)) return false;
        node = getActorManager().getNodeArray(actoreName).getNode(jointId);
		return true;
    }
	
	static bool findNode(const ramNodeIdentifer &identifr, ramNode &node)
	{
		if (!getActorManager().hasNodeArray(identifr.name)) return false;
		node = getActorManager().getNodeArray(identifr.name).getNode(identifr.index);
		return true;
	}
    
private:
	
    inline static ramActorManager& getActorManager() { return ramActorManager::instance(); }
};
