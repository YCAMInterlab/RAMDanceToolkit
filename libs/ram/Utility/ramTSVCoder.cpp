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
			cout << "numNodes:" << numNodes << endl;
			
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
				
				cout << "nodeName:" << nodeName << " nodeName:" << nodeName << " vec:" << vec << endl;
				
				ramNode &node = NA.getNode(i);
				node.setID(i);
				node.setName(nodeName);
				node.setPosition(vec);
				node.setOrientation(quat);
				node.getAccelerometer().update(vec, quat);
			}
			
			cout << "--" << endl;
			
			NA.setTimestamp(ofToFloat( values.at(numNodes*8 + 0 + 3) ));
			
			mSession.appendFrame(NA);
			
		} while (!buffer.isLastLine());
		
		cout << "File loaded! " << endl;
		cout << "Actor: " << mSession.getNodeArrayName() << endl;
		cout << "Duration: " << mSession.getDuration() << "sec"<< endl;
		cout << "Frames: " << mSession.getNumFrames() << endl << endl;
	}


//	return mSession;
}

const bool ramTSVCoder::encode(const ramSession &src)
{
	if (src.getNumFrames() <= 0)
	{
		cout << "session seems not to have any data to save." << endl;
		return false;
	}
	
	ofBuffer buf;
	
	const string address = src.get().isActor() ? RAM_OSC_ADDR_ACTOR : RAM_OSC_ADDR_RIGID_BODY;
	const string entityName = src.get().getName();
	const string numJoints = ofToString( src.get().getNumNode() );
	
	for(int i=0; i<src.getNumFrames(); i++)
	{
		const ramNodeArray &nodeArray = src.getFrame(i);
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
	
	const string fileName = mFileName.empty() ? ofGetTimestampString("%Y.%m.%d %H.%M.%S ") + entityName + ".tsv" : mFileName;

	const bool succeeded = ofBufferToFile(fileName, buf, true);
	
	if (succeeded)
	{
		cout << "Save succeeded! " << endl;
		cout << "File name:" << fileName << endl;
		cout << "Duration: " << src.getDuration() << "sec"<< endl;
		cout << "Frames: " << src.getNumFrames() << endl;
		cout << "Size: " << buf.size() * 0.001 << "KB" << endl << endl;
	}
	else
	{
		cout << "save failed.";
	}
	
	return succeeded;
}

