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

class ramNodeFinder : public ramNodeIdentifer, public ramGlobalShortcut
{
public:

	ramNodeFinder() : ramNodeIdentifer() {}
	ramNodeFinder(const ramNodeIdentifer& copy) : ramNodeIdentifer(copy) {}

	void setTargetName(string name_) { name = name_; }
	void setJointID(int index_) { index = index_; }

	inline bool found()
	{
		if (!hasNodeArray(name))
			return false;

		return index == -1
			   || (index >= 0 && index < getNodeArray(name).getNumNode());
	}

	//

	bool findOne(ramNode &node)
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

	vector<ramNode> findAll()
	{
		vector<ramNode> nodes;

		bool has_target_actor = !name.empty();
		bool has_target_node = index != -1;

		for (int i = 0; i < getNumNodeArray(); i++)
		{
			ramNodeArray &actor = getNodeArray(i);

			if (has_target_actor && name != actor.getName()) continue;

			for (int n = 0; n < actor.getNumNode(); n++)
			{
				if (has_target_node && index != n) continue;

				ramNode &node = actor.getNode(n);
				nodes.push_back(node);
			}
		}
		
		map<string, ramNodeArray>::iterator it = getActorManager().getAllBus().begin();
		
		while (it != getActorManager().getAllBus().end())
		{
			ramNodeArray &o = (*it).second;
			
			if (has_target_actor && name != o.getName()) continue;
			
			for (int n = 0; n < o.getNumNode(); n++)
			{
				if (has_target_node && index != n) continue;
				
				ramNode &node = o.getNode(n);
				nodes.push_back(node);
			}
			
			++it;
		}


		return nodes;
	}

};