// 
// ramBaseEvent.h - RAMDanceToolkit
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

class ramBaseEvent
{
public:

	ramBaseEvent() : enabled(true), last_updated_frame(0), fired(false) {}
	virtual ~ramBaseEvent() {}

	ofEvent<ofEventArgs> fire;

	inline void setEnabled(bool v) { enabled = v; }
	inline bool isEnabled() const { return enabled; }

	inline bool isFired() const { return fired; }

	bool update()
	{
		if (!enabled) return false;

		if (ofGetFrameNum() == last_updated_frame) return fired;

		last_updated_frame = ofGetFrameNum();

		fired = tick();
		return fired;
	}

protected:

	bool fired;

	bool enabled;
	int last_updated_frame;

	virtual bool tick() = 0;
};