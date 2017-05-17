//
// ramBaseScene.h - RAMDanceToolkit
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
#include "ramConstants.h"

namespace rdtk{
	class BaseHasFbo {
	public:
		BaseHasFbo();
		virtual ~BaseHasFbo();
		
		virtual void allocateFbo(int w = 0, int h = 0);
		virtual void deallocateFbo();
		
		ofPtr<ofFbo> getFbo() { return mFbo; }
		ofFbo::Settings getFboSettings() { return mFboSettings; }
		const ofFbo::Settings getFboSettings() const { return mFboSettings; }
		
		bool isAllocatedFbo() const { return mAllocated; }
		
	private:
		ofPtr<ofFbo> mFbo;
		ofFbo::Settings mFboSettings;
		bool mAllocated;
		
	};

}

typedef rdtk::BaseHasFbo RAMDEPRECATED(ramBaseHasFbo);
