// 
// ramNodeFinder.h - RAMDanceToolkit
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

#pragma once

#include "ramActorManager.h"
#include "ramActor.h"
#include "ramNodeIdentifer.h"
#include "ramGlobal.h"

namespace rdtk{
	class NodeFinder : public NodeIdentifer, public GlobalShortcut
	{
	public:
		
		NodeFinder() : NodeIdentifer() {}
		NodeFinder(const NodeIdentifer& copy) : NodeIdentifer(copy) {}
		
		void setTargetName(const string& name_) { name = name_; }
		void setJointID(int index_) { index = index_; }
		
		inline bool found() const
		{
			if (!hasNodeArray(name))
				return false;
			
			return index == -1
			|| (index >= 0 && index < getNodeArray(name).getNumNode());
		}
		
		//
		
		bool findOne(Node &node) const
		{
			if (!isValid()) return false;
			
			if (!hasNodeArray(name))
			{
				if (getNumNodeArray() > 0)
				{
					node = getNodeArray(0).getNode(index);
					return true;
				}
				return false;
			}
			else
			{
				node = getNodeArray(name).getNode(index);
				return true;
			}
		}
		
		//
		
		vector<Node> findAll() const
		{
			vector<Node> nodes;
			
			bool has_target_actor = !name.empty();
			bool has_target_node = index != -1;
			
			for (int i = 0; i < getNumNodeArray(); i++)
			{
				const NodeArray &actor = getNodeArray(i);
				
				if (has_target_actor && name != actor.getName()) continue;
				
				for (int n = 0; n < actor.getNumNode(); n++)
				{
					if (has_target_node && index != n) continue;
					
					const Node &node = actor.getNode(n);
					nodes.push_back(node);
				}
			}
			
			map<string, NodeArray>::const_iterator it = getActorManager().getAllBus().begin();
			
			while (it != getActorManager().getAllBus().end())
			{
				const NodeArray &o = (*it).second;
				
				if (has_target_actor && name != o.getName()) continue;
				
				for (int n = 0; n < o.getNumNode(); n++)
				{
					if (has_target_node && index != n) continue;
					
					const Node &node = o.getNode(n);
					nodes.push_back(node);
				}
				
				++it;
			}
			
			
			return nodes;
		}
		
	};
}

typedef rdtk::NodeFinder RAMDEPRECATED(ramNodeFinder);
