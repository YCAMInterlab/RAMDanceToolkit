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
	
	inline bool isFired() { return fired; }

	bool update()
	{
		if (!enabled) return false;
		if (ofGetFrameNum() == last_updated_frame) return false;
		
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