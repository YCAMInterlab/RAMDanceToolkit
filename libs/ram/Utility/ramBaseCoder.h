// 
// ramBaseCoder.h - RAMDanceToolkit
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

#include "ramNodeArrayBuffer.h"
#include "ramSession.h"

class ramBaseCoder
{
	
public:
	ramBaseCoder() {}
	ramBaseCoder(const string fileName) : mFileName(fileName) {}
	virtual ~ramBaseCoder() {}
	
	
	/// load tsv, and convert to ramSession
	// -----------------------------
	void load(const string filePath)
	{
//		cout
//		<< "[" << __FUNCTION__ << "] " << "start loading file..."
//		<< "File name: " << filePath << endl;
		
		if ( !ofFile::doesFileExist(filePath) )
		{
			cout << "[" << __FUNCTION__ << "] " << filePath << " load failed. No such file or directory." << endl;
		}
		
		ofFile file;
		file.open(filePath);
		
		decode(file.readToBuffer());
	}
	
	
	/// save ramSession to filePath
	// -----------------------------
	const bool save(ramSession &src)
	{
		return encode(src);
	}
	
	const bool save(ramSession &src, const string filePath)
	{
		setFileName(filePath);
		return encode(src);
	}
	
	const bool save(ramNodeArrayBuffer &src)
	{
		ramSession session(src);
		return encode(session);
	}
	
	const bool save(ramNodeArrayBuffer &src, const string filePath)
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
	
	ramSession& get() { return mSession; }
	
protected:
	
	virtual void decode(ofBuffer buffer) = 0;
	virtual bool encode(ramSession &src) = 0;
	
	string mFileName;
	ramSession mSession;
};
