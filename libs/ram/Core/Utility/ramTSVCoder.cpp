// 
// ramTSVCoder.cpp - RAMDanceToolkit
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

#include "ramTSVCoder.h"


void ramTSVCoder::decode(ofBuffer buffer)
{
	
	if (buffer.size())
	{
		clear();
		
		do
		{
			string frame = buffer.getNextLine();
			vector<string> values = ofSplitString(frame, "\t");
			
			if (values.size() < 2)
				throw std::exception();
				
			const string addr = values.at(0);
			const string name = values.at(1);
			
			ramNodeArray NA;
			
			if (addr == RAM_OSC_ADDR_ACTOR)
			{
				ramActor o;
				o.setType(RAM_NODEARRAY_TYPE_ACTOR);
				o.setName(name);
				NA = o;
			}
			else
			{
				ramRigidBody o;
				o.setType(RAM_NODEARRAY_TYPE_RIGIDBODY);
				o.setName(name);
				NA = o;
			}
			
			const int numNodes = ofToInt(values.at(2));
			
			for (int i=0; i<numNodes; i++)
			{
				if (values.size() < i*8+3+7)
					throw std::exception();
					
				const string nodeName = values.at(i*8 + 0 + 3);
				const float vx = ofToFloat( values.at(i*8 + 1 + 3) );
				const float vy = ofToFloat( values.at(i*8 + 2 + 3) );
				const float vz = ofToFloat( values.at(i*8 + 3 + 3) );
				const float qa = ofToFloat( values.at(i*8 + 4 + 3) );
				const float ax = ofToFloat( values.at(i*8 + 5 + 3) );
				const float ay = ofToFloat( values.at(i*8 + 6 + 3) );
				const float az = ofToFloat( values.at(i*8 + 7 + 3) );
				const ofVec3f vec(vx, vy, vz);
				const ofVec3f axis(ax, ay, az);
				const ofQuaternion quat(qa, axis);
				
				ramNode &node = NA.getNode(i);
				node.setID(i);
				node.setName(nodeName);
				node.setPosition(vec);
				node.setOrientation(quat);
				node.getAccelerometer().update(vec, quat);
			}
			
			NA.setTimestamp(ofToFloat( values.at(numNodes*8 + 0 + 3) ));
			
			mSession.appendFrame(NA);
			
		} while (!buffer.isLastLine());
		
//		cout << "[" << __FUNCTION__ << "] " << "File loaded! " << endl;
//		cout << "[" << __FUNCTION__ << "] " << "Actor: " << mSession.getNodeArrayName() << endl;
//		cout << "[" << __FUNCTION__ << "] " << "Duration: " << mSession.getDuration() << "sec"<< endl;
//		cout << "[" << __FUNCTION__ << "] " << "Frames: " << mSession.getNumFrames() << endl << endl;
	}


//	return mSession;
}

bool ramTSVCoder::encode(ramSession &src)
{
	if (src.getNumFrames() <= 0)
	{
		cout << "session seems not to have any data to save." << endl;
		return false;
	}
	
	src.prepareForPlay();
	
	ofBuffer buf;
	ramNodeArray& sample = src.getFrame(0);
	const string timestampStr = ofGetTimestampString("%Y.%m.%d_%H.%M.%S");
	const string address = sample.isActor() ? RAM_OSC_ADDR_ACTOR : RAM_OSC_ADDR_RIGID_BODY;
	const string entityName = sample.getName() + " " + timestampStr;
	const string numJoints = ofToString( sample.getNumNode() );
	
	for(int i=0; i<src.getNumFrames(); i++)
	{
		ramNodeArray &nodeArray = src.getFrame(i);
		stringstream frame;
		frame << address << "\t" << entityName << "\t" << numJoints << "\t";
		
		for(int j=0; j<nodeArray.getNumNode(); j++)
		{
			const ramNode& node = nodeArray.getNode(j);
			const string& name = node.getName();
			const ofVec3f &pos = node.getPosition();
			float qangle, qx, qy, qz;
			node.getOrientationQuat().getRotate(qangle, qx, qy, qz);
			
			frame
			<< name << "\t"
			<< ofToString(pos.x) << "\t"
			<< ofToString(pos.y) << "\t"
			<< ofToString(pos.z) << "\t"
			<< ofToString(qangle) << "\t"
			<< ofToString(qx) << "\t"
			<< ofToString(qy) << "\t"
			<< ofToString(qz) << "\t";
		}
		
		const float timestamp = nodeArray.getTimestamp();
		frame << ofToString(timestamp) << "\n";
		
		buf.append(frame.str().c_str(), frame.str().length());
	}
	
	const string fileName = mFileName.empty() ? timestampStr + "_" + entityName + ".tsv" : mFileName;

	const bool succeeded = ofBufferToFile(fileName, buf, true);
	
	if (succeeded)
	{
		cout << "[" << __FUNCTION__ << "] " << "Save succeeded! " << endl;
		cout << "[" << __FUNCTION__ << "] " << "File name:" << fileName << endl;
		cout << "[" << __FUNCTION__ << "] " << "Duration: " << src.getDuration() << "sec"<< endl;
		cout << "[" << __FUNCTION__ << "] " << "Frames: " << src.getNumFrames() << endl;
		cout << "[" << __FUNCTION__ << "] " << "Size: " << buf.size() * 0.001 << "KB" << endl << endl;
	}
	else
	{
		cout << "[" << __FUNCTION__ << "] " << "save failed.";
	}
	
	return succeeded;
}

