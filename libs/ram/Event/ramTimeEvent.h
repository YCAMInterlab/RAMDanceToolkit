// 
// ramTimeEvent.h - RAMDanceToolkit
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

#include "ramBaseEvent.h"

class ramBaseTimerEvent : public ramBaseEvent
{
public:

	ramBaseTimerEvent() : current_time(0), timer_duration(1) {}

	inline void reset() { current_time = 0; }

	inline float getDuration() const { return timer_duration; }
	inline float getCurrentTime() const { return current_time; }
	inline float getProgress() const { return current_time / timer_duration; }

protected:

	float current_time;
	float timer_duration;

};

class ramScheduledTimerEvent : public ramBaseTimerEvent
{
public:

	void setDuration(float t) { timer_duration = t; }

protected:

	bool tick()
	{
		current_time += ofGetLastFrameTime();

		bool will_fire = false;

		while (current_time > timer_duration)
		{
			current_time -= timer_duration;
			will_fire = true;
		}

		if (will_fire)
		{
			static ofEventArgs e;
			ofNotifyEvent(fire, e);
		}

		return will_fire;
	}

};

class ramRandomTimerEvent : public ramBaseTimerEvent
{
public:

	ramRandomTimerEvent() : min(1), max(5) {}

	void setDuration(float min, float max)
	{
		this->min = min;
		this->max = max;
	}

protected:

	float min, max;

	void randmize()
	{
		timer_duration = ofRandom(min, max);
	}

	bool tick()
	{
		current_time += ofGetLastFrameTime();

		bool will_fire = false;

		while (current_time > timer_duration)
		{
			current_time -= timer_duration;
			will_fire = true;
		}

		if (will_fire)
		{
			static ofEventArgs e;
			ofNotifyEvent(fire, e);

			randmize();
		}

		return will_fire;
	}
};