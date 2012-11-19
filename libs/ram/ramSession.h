#pragma once

#include "ramActor.h"

#pragma mark - ramBuffer

class ramSession
{
public:

	enum EntityType
	{
		RAM_UNDEFINED_TYPE = -1,
		RAM_ACTOR,
		RAM_RIGID_BODY
	};
	
	ramSession() : sessionName(""), sessionType(RAM_UNDEFINED_TYPE), loop(true), recording(false), playing(false), rate(1.0) {}
	~ramSession() {}
	
	void clear()
	{
		actors.clear();
		rigids.clear();
		
		playhead = 0;
		duration = 0;
		frame_time = 0;
		play_start_time = 0;
		rec_start_time = 0;
		
		frame_index = 0;
		num_frames = 0;
	}
	
//  !!!: TODO
//	void load(string filename);
//	void save(const string filename);
	
	int getFrame()
	{
		return floor(playhead / frame_time);
	}
	
	void setup(const string mame, int type )
	{
		clear();
		sessionName = mame;
		sessionType = type;
	}
	
	void update()
	{
		if (isRecording())
		{
			if (sessionType == RAM_ACTOR)
				appendFrame( getActorManager().getActor(sessionName) );
			
			else if (sessionType == RAM_RIGID_BODY)
				appendFrame( getActorManager().getRigidBody(sessionName) );
		}
		
		if (isPlaying())
		{
			playhead = (ofGetElapsedTimef() - play_start_time) * rate;
			frame_index = getFrame();
			
			if(frame_index >= num_frames)
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
		if (sessionName == "") return;
		
		recording = true;
		
		clear();
		rec_start_time = ofGetElapsedTimef();
	}
	
	void stopRecording()
	{
		recording = false;
		
		duration = ofGetElapsedTimef() - rec_start_time;
		num_frames = sessionType==RAM_ACTOR ? actors.size() : sessionType==RAM_RIGID_BODY ? rigids.size() : RAM_UNDEFINED_TYPE;
		frame_time = duration / num_frames;
	}
	
	void play()
	{
		if (num_frames <= 0) return;
		
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
	
	inline ramActor& getNextFrameActor() {return actors.at(frame_index);}
	inline ramRigidBody& getNextFrameRigid() {return rigids.at(frame_index);}
	
protected:
	
	vector<ramActor> actors;
	vector<ramRigidBody> rigids;
	
	string sessionName;
	int sessionType;
	bool loop;
	float rate;
	
	bool recording;
	bool playing;
	
	float playhead;
	float duration;
	float frame_time;
	float play_start_time;
	float rec_start_time;
	
	int frame_index;
	int num_frames;
	inline ramActorManager& getActorManager() { return ramActorManager::instance(); }
};



