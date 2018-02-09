// 
// ramActor.h - KinectV2OSC
// 
// Copyright 2018 nariakiiwatani
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


#ifndef ramActor_h
#define ramActor_h

struct ramActor
{
public:
	enum Joint
	{
		HIPS,
		ABDOMEN,
		CHEST,
		NECK,
		HEAD,
		
		L_HIP,
		L_KNEE,
		L_ANKLE,
		L_TOE,
		
		R_HIP,
		R_KNEE,
		R_ANKLE,
		R_TOE,
		
		L_COLLAR,
		L_SHOULDER,
		L_ELBOW,
		L_WRIST,
		L_HAND,
		
		R_COLLAR,
		R_SHOULDER,
		R_ELBOW,
		R_WRIST,
		R_HAND,
		
		NUM_JOINTS
	};
	ofNode nodes_[NUM_JOINTS];

	static string getJointName(int joint)
	{
		string joint_name[NUM_JOINTS] =
		{
			"HIPS",
			"ABDOMEN",
			"CHEST",
			"NECK",
			"HEAD",
			"LEFT_HIP",
			"LEFT_KNEE",
			"LEFT_ANKLE",
			"LEFT_TOE",
			"RIGHT_HIP",
			"RIGHT_KNEE",
			"RIGHT_ANKLE",
			"RIGHT_TOE",
			"LEFT_COLLAR",
			"LEFT_SHOULDER",
			"LEFT_ELBOW",
			"LEFT_WRIST",
			"LEFT_HAND",
			"RIGHT_COLLAR",
			"RIGHT_SHOULDER",
			"RIGHT_ELBOW",
			"RIGHT_WRIST",
			"RIGHT_HAND"
		};
		
		return joint < NUM_JOINTS ? joint_name[joint] : "";
	}
	
	void collection() {
		auto mid = [](ofNode &a, ofNode &b) {
			ofNode ret;
			ret.setGlobalPosition((a.getGlobalPosition()+b.getGlobalPosition())/2.f);
			ofQuaternion q = a.getGlobalOrientation();
			q.slerp(0.5f, q, b.getGlobalOrientation());
			ret.setGlobalOrientation(q);
			return ret;
		};
		nodes_[L_COLLAR] = mid(nodes_[CHEST], nodes_[L_SHOULDER]);
		nodes_[R_COLLAR] = mid(nodes_[CHEST], nodes_[R_SHOULDER]);
	}
};

#endif /* ramActor_h */
