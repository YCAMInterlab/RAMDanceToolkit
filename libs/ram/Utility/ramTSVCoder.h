#pragma once

#include "ramBaseCoder.h"
#include "ramSession.h"

class ramTSVCoder : public ramBaseCoder
{
	
public:
//	void test()
//	{
//		ofBuffer buf;
//		
//		{
//			/// アクターの動き１フレーム分、全関節の位置と回転を文字列としてbufferに追加
//			/// 各floatの間は後でパース出来る様に任意の文字(この場合は"/")で区切る
//			string str = ofToString(0.01f) + "/" + ofToString(0.02f) + "/" + ofToString(0.03f) + "\n";
//			buf.append(str.c_str(), str.length());
//		}
//		
//		{
//			/// buffer が空じゃなければ
//			if (buf.size()) {
//				/// 最後の行まで来たら頭に戻る(ループ)
//				if (buf.isLastLine())
//					buf.resetLineReader();
//				
//				/// 次の行(アクターの動き１フレーム分、全関節の位置と回転を文字列として読む)
//				string str = buf.getNextLine();
//				/// 書き込みの時に指定した文字で分割
//				vector<string> values = ofSplitString(str, "/");
//				vector<float> data;
//				data.clear();
//				/// floatに変換
//				for (int i=0; i<values.size(); i++)
//					data.push_back(ofToFloat(values.at(i)));
//				
//				/// ダンプ
//				for (int i=0; i<data.size(); i++)
//					cout << data.at(i) << ", ";
//				cout << endl;
//			}
//		}
//	}
	
protected:
	
//	bool decode(const string filePath)
//	{
//		ofBu
//	}
	
	bool encode(const ramSession &src)
	{
		if (src.getSize() <= 0) return false;
		
		const string address = src.get().isActor() ? RAM_OSC_ADDR_ACTOR : RAM_OSC_ADDR_RIGID_BODY;
		const string entityName = src.get().getName();
		const string numJoints = ofToString( src.get().getNumNode() );
		
		for(int i=0; i<src.getNumFrames(); i++)
		{
			const ramNodeArray &nodeArray = src.get(i);
			
			stringstream frame;
			frame << address << "\t" << entityName << "\t" << numJoints << "\t";
			
			for(int j=0; j<nodeArray.getNumNode(); j++)
			{
				const ramNode &node = nodeArray.getNode(j);
				
//				const string& name = node.getName();
				const ofVec3f &pos = node.getPosition();
				float qangle, qx, qy, qz;
				node.getOrientationQuat().getRotate(qangle, qx, qy, qz);
				
				frame
//				<< name << "\t"
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
			
			mBuffer.append(frame.str().c_str(), frame.str().length());
		}
		
		const string fileName = mFileName.empty() ? ofGetTimestampString("%Y.%m.%d %H.%M.%S ") + entityName + ".tsv" : mFileName;

		const bool succeeded = ofBufferToFile(fileName, mBuffer, true);
		
		if (succeeded)
		{
			cout << "save succeeded! " << fileName << " was created." << endl;
			cout << "Duration: " << src.getDuration() << "sec"<< endl;
			cout << "Frames: " << src.getNumFrames() << endl;
			cout << "Size: " << mBuffer.size() * 0.001 << "KB" << endl;
		}
		else
		{
			cout << "save failed.";
		}
		
		mBuffer.clear();
		return succeeded;
	}
};

