#pragma once

#include "ramNodeArrayBuffer.h"
#include "ramSession.h"
/*
 # format of each frame sent from motioner
 s: string
 i: int
 f: float
  ssisfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsfffffffsffffffff
 
 "{s} osc route", "{s} Actor name", {i} numJoints,
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
	
	
	/// load tsv, and convert to ramSession
	// -----------------------------
	ramSession load(const string filePath)
	{
		cout
		<< "start loading file..."
		<< "File name: " << filePath << endl;
		
		if ( !ofFile::doesFileExist(filePath) )
		{
			cout << filePath << " load failed. No such file or directory." << endl;
			return ramSession();
		}
		
		ofFile file;
		file.open(filePath);
		
		return decode(file.readToBuffer());
	}
	
	
	/// save ramSession to filePath
	// -----------------------------
	const bool save(const ramSession &src)
	{
		return encode(src);
	}
	
	const bool save(const ramSession &src, const string filePath)
	{
		setFileName(filePath);
		return encode(src);
	}
	
	const bool save(const ramNodeArrayBuffer &src)
	{
		ramSession session(src);
		return encode(session);
	}
	
	const bool save(const ramNodeArrayBuffer &src, const string filePath)
	{
		ramSession session(src);
		setFileName(filePath);
		return encode(session);
	}
	
	
	/// clear all members
	// -----------------------------
	void clear()
	{
		mFileName.clear();
		mSession.clear();
	}
	
	
	/// setters
	// -----------------------------
	inline void setFileName(const string fileName) { mFileName = fileName; }
	
	
protected:
	
	virtual ramSession decode(const ofBuffer buffer) = 0;
	virtual const bool encode(const ramSession &src) = 0;
	
	string mFileName;
	ramSession mSession;
};
