//
//  ramOscPlaybacker.h
//  RAMDanceToolkit
//
//  Created by motoishmz on 4/27/13.
//  Copyright (c) 2013 YCAMInterlab. All rights reserved.
//
#pragma once

#include "ramOscPlaybacker.h"
#include "ofxOscMessage.h"
#include "ofxXmlSettings.h"

class ramOscPlaybacker
{
	
public:
	
	enum ramPlayerState
	{
		PLAYER_STATE_PLAYING = 0,
		PLAYER_STATE_PAUSED = 1,
		PLAYER_STATE_STOPPED = 2,
		
		PLAYER_STATE_UNKNOWN = -1
	};
	
	
	ramOscPlaybacker(const string path);
	virtual ~ramOscPlaybacker();
	
	
	void update();
	void debugDraw(ofPoint drawPos);
	bool canPlayback();
	ofxOscMessage& getCurrentMessage();
	
	
	// player control interfaces
	void load(const string path);
	void play();
	void stop();
	void pause();
	void resume();
	void clearPlaybackData();
	
	
	// player properties getter/setter
	inline bool getLoop() const { return loop; }
	inline float getSpeed() const { return speed; }
	inline float getDuration() const { return duration; }
	inline float getElapsedTime() const { return elapsed_time; }
	inline float getPlayhead() const { return playhead; }
	
	inline void setLoop(const bool new_loop) { loop = new_loop; }
	inline void setSpeed(const float new_speed) { speed = new_speed; }
	inline void setDuration(const float new_duration) { duration = new_duration; }
	inline void setPlayhead(const float  new_playhead)
	{
		playhead = new_playhead > 1.0 ? 1.0 : new_playhead < 0.0 ? 0.0 : new_playhead; // 0 ~ 1
		elapsed_time = duration * playhead;
	}
	
	inline bool isPlaying() const { return (state == PLAYER_STATE_PLAYING); }
	inline bool isStopped() const { return (state == PLAYER_STATE_STOPPED); }
	inline bool isPaused() const { return (state == PLAYER_STATE_PAUSED); }
	inline bool isLoop() const { return getLoop(); }
		
protected:
	
	vector<ofxOscMessage> frames;
	static ofxOscMessage t;

	
	// helpers
	inline void setState(ramPlayerState new_state) { state = new_state; };
	void parseXmlToArgs(ofxOscMessage &m, ofxXmlSettings &xml);
	void updatePlayhead();
	size_t calcCurrentFrameIndex();
	
	
	// playback data properties
	string recording_date;
	string actor_name;
	size_t num_nodes;
	
	
	// player properties
	ramPlayerState state;
	bool loop;
	float speed;
	float duration;
	float playhead; // 0 ~ 1
	float elapsed_time;
	
	
	// xml tag names
	// ocs mesasge address
	static const string addrTagName;
	
	// session meta
	static const string sessionTagName; // root node <session>
	static const string recDateTagName;
	
	// converted from one osc message
	static const string frameTagName; // <frame>
	static const string entityTagName;
	static const string timestampTagName;
	static const string nodesTagName;
	static const string nodeTagName;
	static const string arg1TagName;
	static const string arg2TagName;
	static const string arg3TagName;
	static const string arg4TagName;
	static const string arg5TagName;
	static const string arg6TagName;
	static const string arg7TagName;
	static const string arg8TagName;
	
private:
	ramOscPlaybacker();
};