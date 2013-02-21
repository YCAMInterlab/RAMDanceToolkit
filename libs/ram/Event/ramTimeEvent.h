#pragma once

#include "ofMain.h"

class ramBaseTimerEvent
{
public:
	
	ofEvent<ofEventArgs> fire;
	
	ramBaseTimerEvent() : enabled(true), last_updated_frame(0), fired(false) {}

	inline void setEnabled(bool v) { enabled = v; }
	inline bool isEnabled() const { return enabled; }
	
	inline void reset() { current_time = 0; }
	
	inline bool isFired() { return fired; }
	
	bool update()
	{
		if (!enabled) return false;
		if (ofGetFrameNum() == last_updated_frame) return false;
		last_updated_frame = ofGetFrameNum();
		
		fired = tick();
		return fired;
	}

	inline float getDuration() const { return timer_duration; }
	inline float getCurrentTime() const { return current_time ;}
	inline float getPosition() const { return current_time / timer_duration; }
	
protected:

	bool enabled;
	int last_updated_frame;
	
	bool fired;
	
	float current_time;
	float timer_duration;
	
	virtual bool tick() = 0;
};

class ramScheduledTimerEvent : public ramBaseTimerEvent
{
public:
	
	void setTimer(float t) { timer_duration = t; }
	
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
	
	void setTimer(float min, float max)
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
