//
//  ramOscPlaybacker.cpp
//  RAMDanceToolkit
//
//  Created by motoishmz on 4/27/13.
//  Copyright (c) 2013 YCAMInterlab. All rights reserved.
//
#include "ramOscPlaybacker.h"


ramOscPlaybacker::ramOscPlaybacker(const string path)
{
	clearPlaybackData();
	load(path);
}

ramOscPlaybacker::~ramOscPlaybacker()
{
	
}

void ramOscPlaybacker::update()
{
	if (isStopped() || isPaused()) return;
	
	updatePlayhead();
}

void ramOscPlaybacker::debugDraw(ofPoint drawPos)
{
	ofPushStyle();
	ofSetColor(0);
	ofDrawBitmapString( "     [name] " + actor_name, drawPos.x, drawPos.y );
	ofDrawBitmapString( "[num nodes] " + ofToString(num_nodes), drawPos.x, drawPos.y+20 );
	ofDrawBitmapString( "     [time] " + ofToString(floor(getElapsedTime()*10)/10) + "/" + ofToString(floor(getDuration()*10)/10) + " sec", drawPos.x, drawPos.y+40 );
	ofDrawBitmapString( "   [frames] " + ofToString(calcCurrentFrameIndex()) + "/" + ofToString(frames.size()) + " frames", drawPos.x, drawPos.y+60 );
	ofPopStyle();
}

ofxOscMessage& ramOscPlaybacker::getCurrentMessage()
{
	if (isStopped())
		return t;
	else
		return frames.at( calcCurrentFrameIndex() );
}

bool ramOscPlaybacker::canPlayback()
{
	return frames.size() > 1;
}



#pragma mark - player control interfaces

void ramOscPlaybacker::load(const string path)
{
	if (!ofFile::doesFileExist(path)) return;
	
	clearPlaybackData();
	
	ofxXmlSettings xml(path);
	xml.pushTag(sessionTagName);
	
	const string date = xml.getValue( recDateTagName, "unknown_date_"+ofToString(ofRandom(100)) );
	const string address = xml.getValue( addrTagName, "/unknown_address" );
    const size_t num_frames = xml.getNumTags( frameTagName );
	
	recording_date = date;
	
    for(int frame_index=0; frame_index<num_frames; frame_index++)
    {
		// push to <frame>
        xml.pushTag(frameTagName, frame_index);
		{
			ofxOscMessage m;
			m.setAddress(address);
			parseXmlToArgs(m, xml);
			frames.push_back(m);
		}
        xml.popTag();
    }
    
    xml.popTag();
	
	if (frames.size() != 0)
	{
		// calc duration
		ofxOscMessage &firstFrame = frames.front();
		ofxOscMessage &lastFrame = frames.back();
		const float timestamp_front_frame = firstFrame.getArgAsFloat(firstFrame.getNumArgs()-1);
		const float timestamp_back_frame = lastFrame.getArgAsFloat(lastFrame.getNumArgs()-1);
		duration = timestamp_back_frame - timestamp_front_frame;
	}
}

void ramOscPlaybacker::play()
{
	if (frames.size() <= 1) return;
	
	setState(PLAYER_STATE_PLAYING);
}

void ramOscPlaybacker::stop()
{
	setState(PLAYER_STATE_STOPPED);
	setPlayhead(0.0);
}

void ramOscPlaybacker::pause()
{
	setState(PLAYER_STATE_PAUSED);
}

void ramOscPlaybacker::resume()
{
	setState(PLAYER_STATE_PLAYING);
}

void ramOscPlaybacker::clearPlaybackData()
{
	frames.clear();
	
	recording_date.empty();
	actor_name.empty();
	num_nodes = 0;
	
	setLoop(true);
	setSpeed(1.0);
	setPlayhead(0.0);
	duration = 0.0;
}



#pragma mark - helpers

void ramOscPlaybacker::parseXmlToArgs(ofxOscMessage &m, ofxXmlSettings &xml)
{
	actor_name = xml.getValue(entityTagName, "unknown") + "_" + recording_date;
	num_nodes = xml.getValue(nodesTagName, 0);
	
	m.addStringArg(actor_name);
	m.addIntArg(num_nodes);
	
	for (int node_index=0; node_index<num_nodes; node_index++)
	{
		// push to <node>
		xml.pushTag(nodeTagName, node_index);
		{
			const string node_name = xml.getValue(arg1TagName, "");
			const float vx = xml.getValue(arg2TagName, 0.0);
			const float vy = xml.getValue(arg3TagName, 0.0);
			const float vz = xml.getValue(arg4TagName, 0.0);
			
			const float angle = xml.getValue(arg5TagName, 0.0);
			const float ax = xml.getValue(arg6TagName, 0.0);
			const float ay = xml.getValue(arg7TagName, 0.0);
			const float az = xml.getValue(arg8TagName, 0.0);
			
			m.addStringArg( node_name );
			m.addFloatArg( vx );
			m.addFloatArg( vy );
			m.addFloatArg( vz );
			m.addFloatArg( angle );
			m.addFloatArg( ax );
			m.addFloatArg( ay );
			m.addFloatArg( az );
		}
		xml.popTag();
	}
	
	m.addFloatArg( xml.getValue(timestampTagName, 0.0) );
}

void ramOscPlaybacker::updatePlayhead()
{
	elapsed_time += ofGetLastFrameTime() * speed;
	
	bool wrapped = false;
	
	if (elapsed_time > duration)
	{
		elapsed_time = 0;
		wrapped = true;
	}
	
	if (elapsed_time < 0)
	{
		elapsed_time = duration;
		wrapped = true;
	}
	
	if (wrapped)
	{
		if (!isLoop())
		{
			stop();
		}
	}
	
	setPlayhead(elapsed_time / duration);
}

size_t ramOscPlaybacker::calcCurrentFrameIndex()
{
	size_t frameIndex = floor(frames.size() * playhead);
	
	if (frameIndex >= frames.size()) frameIndex = frames.size()-1;
	
	return frameIndex;
}



#pragma mark - xml tag names

const string ramOscPlaybacker::addrTagName = "address";
const string ramOscPlaybacker::sessionTagName = "session";
const string ramOscPlaybacker::recDateTagName = "recDate";
const string ramOscPlaybacker::frameTagName  = "frame";
const string ramOscPlaybacker::timestampTagName  = "timestamp";
const string ramOscPlaybacker::entityTagName = "entity";
const string ramOscPlaybacker::nodesTagName = "nodes";
const string ramOscPlaybacker::nodeTagName = "node";
const string ramOscPlaybacker::arg1TagName = "name";
const string ramOscPlaybacker::arg2TagName = "x";
const string ramOscPlaybacker::arg3TagName = "y";
const string ramOscPlaybacker::arg4TagName = "z";
const string ramOscPlaybacker::arg5TagName = "qa";
const string ramOscPlaybacker::arg6TagName = "qx";
const string ramOscPlaybacker::arg7TagName = "qy";
const string ramOscPlaybacker::arg8TagName = "qz";

ofxOscMessage ramOscPlaybacker::t;