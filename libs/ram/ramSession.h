#pragma once


#include "ramActor.h"


#pragma mark - ramBuffer

class ramSession
{
public:

//  !!!: TODO
//	enum EntityType
//	{
//		RAM_ACTOR,
//		RAM_RIGID_BODY
//	};
	
	ramSession(const string name) : sessionName(name), loop(true), rate(1.0) { clear(); }
	~ramSession() {}
	
	void clear()
	{
		actors.clear();
		rigids.clear();
		
		playhead = 0;
		duration = 0;
		rec_start_time = 0;
		
		recording = false;
		playing = false;
	}
	
//  !!!: TODO
//	void load(string filename);
//	void save(const string filename);
	
	int getFrame()
	{
		return floor(playhead / frame_time);
	}
	
	void update()
	{
		if (isRecording())
		{
			appendFrame( getActorManager().getActor(sessionName) );
		}
		
		if (isPlaying())
		{
			playhead = (ofGetElapsedTimef() - play_start_time) * rate;
			frame_index = getFrame();
			
			if(frame_index >= actors.size())
			{
				frame_index = 0;
				
				if (loop)
					play_start_time = ofGetElapsedTimef();
				else
					playing = false;
			}
		}
	}

	void startRecording()
	{
		recording = true;
	
		clear();
		rec_start_time = ofGetElapsedTimef();
	}
	
	void stopRecording()
	{
		recording = false;
		
		duration = ofGetElapsedTimef() - rec_start_time;
		frame_time = duration / actors.size();
	}
	
	void play()
	{
		playing = true;
	
		recording = false;
		play_start_time = ofGetElapsedTimef();
	}
	
	void stop()
	{
		playing = false;
	}

	inline void appendFrame(ramActor& actor) {actors.push_back(actor);}
	inline void appendFrame(ramRigidBody& rigid) {rigids.push_back(rigid);}

	inline void setLoop(bool l) {loop = l;};
	inline void setRate(const float r) {rate = r;};
	inline void setPlayhead(const float t){ playhead = t; };
	
	inline bool isPlaying() {return playing;}
	inline bool isRecording() {return recording;}
	inline bool isLoop() {return loop;}
	inline float getDuration() {return duration;}
	inline float getPlayhead() {return playhead;}
	inline string getSessionName() {return sessionName;}

	inline ramActor& getNextFrame() {return actors.at(frame_index);}
	
protected:
	
	string sessionName;
	vector<ramActor> actors;
	vector<ramRigidBody> rigids;
	
	bool loop;
	float rate;
	
	float playhead;
	float duration;
	float frame_time;
	float play_start_time;
	float rec_start_time;
	
	bool recording;
	bool playing;
	
	int frame_index;
	inline ramActorManager& getActorManager() { return ramActorManager::instance(); }
};