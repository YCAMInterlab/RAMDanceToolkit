// 
// ActorManager.h - RAMDanceToolkit
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

#include "ofMain.h"

#include "ofxOsc.h"
#include "ofxInteractivePrimitives.h"

#include "ramConstants.h"
#include "ramUtils.h"
#include "ramActor.h"
#include "ramOscManager.h"
#include "ramNodeIdentifer.h"

namespace rdtk{

	class ActorManager
	{
	public:
		
		ofEvent<Actor> actorSetup;
		ofEvent<Actor> actorExit;
		ofEvent<RigidBody> rigidSetup;
		ofEvent<RigidBody> rigidExit;
		
		ofEvent<NodeIdentifer> selectStateChanged;
		
		// singleton
		inline static ActorManager& instance()
		{
			if (_instance == NULL)
				_instance = new ActorManager;
			return *_instance;
		}
		
		void setup();
		void update();
		void draw();
		
		// node
		
		inline size_t getNumNodeArray() const { return nodearrays.size(); }
		
		inline vector<NodeArray> getAllNodeArrays() const
		{
			vector<NodeArray> r;
			for (int i = 0; i < getNumNodeArray(); i++)
				r.push_back(getNodeArray(i));
			return r;
		}
		
		inline const vector<string>& getNodeArrayNames() const { return nodearrays.keys(); }
		inline NodeArray& getNodeArray(int index) { return *nodearrays[index].get(); }
		inline const NodeArray& getNodeArray(int index) const { return const_cast<NodeArray&>(*nodearrays[index].get()); }
		inline NodeArray& getNodeArray(const string& name) { return *nodearrays[name].get(); }
		inline const NodeArray& getNodeArray(const string& name) const { return const_cast<NodeArray&>(*nodearrays[name].get()); }
		inline bool hasNodeArray(const string &key) const { return nodearrays.hasKey(key); }
		inline void removeNodeArray(const string& name) { nodearrays.erase(name); }
		
		// test
		void setNodeArray(const NodeArray& NA) { nodearrays.set(NA.getName(), make_shared<NodeArray>(NA)); }
		void setNodeArray(ofPtr<NodeArray> NA) { nodearrays.set(NA->getName(), NA); }
		
		
		// for mouse picked node
		
		const NodeIdentifer& getLastSelectedNodeIdentifer() const;
		const Node* getLastSelectedNode() const;
		const NodeArray* getLastSelectedNodeArray() const;
		void clearSelected();
		
		// Freeze all actor
		
		inline bool isFreezed() { return freeze; }
		inline void setFreezed(bool v) { freeze = v; }
		inline void toggleFreeze() { freeze ^= true; }
		
		// bus
		
		bool hasBus(const string& bus_name) const { return bus.find(bus_name) != bus.end(); }
		void setBus(const string& bus_name, const NodeArray &arr) { bus[bus_name] = arr; }
		const NodeArray& getBus(const string& bus_name) const { return bus.at(bus_name); }
		map<string, NodeArray>& getAllBus() { return bus; };
		const map<string, NodeArray>& getAllBus() const { return bus; };
		inline size_t getNumBus() { return bus.size(); };
		inline size_t eraseFromBus(const string& bus_name) { return bus.erase(bus_name); };
		
		
		// for internal use
		
		void updateWithOscMessage(const ofxOscMessage &m);
		void setupOscReceiver(OscManager* oscMan) {
			
			oscReceiver.addAddress(RAM_OSC_ADDR_ACTOR);
			oscReceiver.addAddress(RAM_OSC_ADDR_RIGID_BODY);
			
			oscMan->addReceiverTag(&oscReceiver);
			
		}
		void setEnableOnMouseRelease(bool v);
		
	private:
		
		static ActorManager *_instance;
		
		OscReceiveTag oscReceiver;
		
		// noncopyable
		ActorManager() {};
		ActorManager(const ActorManager&) {}
		ActorManager& operator=(const ActorManager&) { return *this; }
		~ActorManager() {};
		
		CompoundContainer<ofPtr<NodeArray> > nodearrays;
		
		ofxInteractivePrimitives::RootNode rootNode;
		
		bool freeze;
		
		class NodeSelector;
		friend class NodeSelector;
		
		NodeSelector *nodeSelector;
		
		map<string, NodeArray> bus;
		
		void onSelectStateChanged(NodeIdentifer &e);
		void onMouseReleased(ofMouseEventArgs &e);
	};
	
}

typedef rdtk::ActorManager RAMDEPRECATED(ramActorManager);
