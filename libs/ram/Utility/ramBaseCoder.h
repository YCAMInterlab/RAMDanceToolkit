#pragma once

#include "ramNodeArrayBuffer.h"
#include "ramSession.h"
/*
 # format of each frame sent from motioner
 s: string
 i: int
 f: float
  sisfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsffffffff
 
 "{s} osc route", {i} numJoints,
 "{s} joint name", {f}x, {f}y, {f}z, {f}quat angle, {f}quat x, {f}quat y, {f}quat z,
 "{s} joint name", {f}x, {f}y, {f}z, {f}quat angle, {f}quat x, {f}quat y, {f}quat z,
 "{s} joint name", {f}x, {f}y, {f}z, {f}quat angle, {f}quat x, {f}quat y, {f}quat z,
 "{s} joint name", {f}x, {f}y, {f}z, {f}quat angle, {f}quat x, {f}quat y, {f}quat z,
 ...
 ...
 ...
 {f} timestamp
 */


class ramBaseCoder
{
	
public:
	ramBaseCoder() {}
	ramBaseCoder(const string fileName) : mFileName(fileName) {}
	virtual ~ramBaseCoder() {}
	
	
	/// save csv file to filePath
	// -----------------------------
	bool save(const ramSession &src)
	{
		clear();
		return encode(src);
	}
	bool save(const ramSession &src, const string filePath)
	{
		clear();
		setFileName(filePath);
		return encode(src);
	}
	
	bool save(const ramNodeArrayBuffer &src)
	{
		ramSession session(src);
		
		clear();
		return encode(session);
	}
	
	bool save(const ramNodeArrayBuffer &src, const string filePath)
	{
		ramSession session(src);
		
		clear();
		setFileName(filePath);
		return encode(session);
	}
	
	
	/// clear all members
	// -----------------------------
	void clear()
	{
		mBuffer.clear();
		mFileName.clear();
		mSession.clear();
	}
	
	
	/// result
	// -----------------------------
	ramSession get() { return mSession; };
	
	
	/// setters
	// -----------------------------
	inline void setFileName(const string fileName) { mFileName = fileName; }
	
	
protected:
	
	virtual bool encode(const ramSession &src) = 0;
	
	ofBuffer mBuffer;
	
	string mFileName;
	ramSession mSession;
};
