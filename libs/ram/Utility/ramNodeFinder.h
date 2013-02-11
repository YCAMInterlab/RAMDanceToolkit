#pragma once

#include "ramActorManager.h"
#include "ramActor.h"
#include "ramNodeIdentifer.h"

class ramNodeFinder
{
public:
    
    ramNodeFinder() : actor_name(""), joint_id(-1) {}
	ramNodeFinder(int joint_id) : actor_name(""), joint_id(joint_id) {}
	ramNodeFinder(const string& actor_name) : actor_name(actor_name), joint_id(-1) {}
	ramNodeFinder(const string& actor_name, int joint_id) : actor_name(actor_name), joint_id(joint_id) {}
	ramNodeFinder(const ramNodeIdentifer &identifr) : actor_name(identifr.name), joint_id(identifr.index) {}
    
	void setActorName(string actor_name_) { actor_name = actor_name_; }
	void setJointID(int joint_id_) { joint_id = joint_id_; }
	
	vector<ramNode> get()
	{
		vector<ramNode> nodes;
		
		bool has_target_actor = !actor_name.empty();
		bool has_target_node = joint_id != -1;
		
		for (int i = 0; i < getActorManager().getNumActor(); i++)
        {
			ramActor &actor = getActorManager().getActor(i);
			
			if (has_target_actor && actor_name != actor.getName()) continue;
			
			for (int n = 0; n < actor.getNumNode(); n++)
			{
				if (has_target_node && joint_id != n) continue;
				
				ramNode &node = actor.getNode(n);
				nodes.push_back(node);
			}
        }
		
		return nodes;
	}
	
	vector<ramNode*> getPtr()
	{
		vector<ramNode*> nodes;
		
		bool has_target_actor = !actor_name.empty();
		bool has_target_node = joint_id != -1;
		
		for (int i = 0; i < getActorManager().getNumActor(); i++)
        {
			ramActor &actor = getActorManager().getActor(i);
			
			if (has_target_actor && actor_name != actor.getName()) continue;
			
			for (int n = 0; n < actor.getNumNode(); n++)
			{
				if (has_target_node && joint_id != n) continue;
				
				ramNode &node = actor.getNode(n);
				nodes.push_back(&node);
			}
        }
		
		return nodes;
	}
	
    static vector<ramNode> findNodes(int jointId)
    {
        vector<ramNode> nodes;
		
        for (int i=0; i<getActorManager().getNumActor(); i++)
        {
            ramNode &node = getActorManager().getActor(i).getNode(jointId);
            nodes.push_back(node);
        }
		
        return nodes;
    }
	
	static vector<ramNode> findNodes(const string& actoreName)
    {
        vector<ramNode> nodes;
		ramActor &actor = getActorManager().getActor(actoreName);
		
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
	
	string actor_name;
	int joint_id;
    
    inline static ramActorManager& getActorManager() { return ramActorManager::instance(); }
};
