// 
// UpsideDown.h - RAMDanceToolkit
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

class UpsideDown : public rdtk::BaseScene
{
	
public:
    
    ofVec3f mEuler;
    ofQuaternion mRotation;
    ramFilterEach<rdtk::UpsideDown> mUpsideDown;
    
    float mOffset;
    
    ofVec3f mAutoRotateSpeed;
    struct { bool x, y, z; } mAutoRotate;
	
	void setupControlPanel()
	{
#ifdef RAM_GUI_SYSTEM_OFXUI
		
		
		ofxUICanvas* panel = rdtk::GetGUI().getCurrentUIContext();
		
		ofAddListener(panel->newGUIEvent, this, &UpsideDown::onValueChanged);
        
        const float dim = 16.0f;
        
        panel->addSlider("ANGLE X", 0.0f, 360.0f, &mEuler.x, 300.0f, dim);
        panel->addSlider("ANGLE Y", 0.0f, 360.0f, &mEuler.y, 300.0f, dim);
        panel->addSlider("ANGLE Z", 0.0f, 360.0f, &mEuler.z, 300.0f, dim);
        
        panel->addSpacer(300.0f, 1.0f);
        
        panel->addToggle("AUTO ROTATE X", &mAutoRotate.x, dim, dim);
        panel->addToggle("AUTO ROTATE Y", &mAutoRotate.y, dim, dim);
        panel->addToggle("AUTO ROTATE Z", &mAutoRotate.z, dim, dim);
        
        panel->addSlider("AUTO ROTATE SPEED X", -5.0f, 5.0f, &mAutoRotateSpeed.x, 300.0f, dim);
        panel->addSlider("AUTO ROTATE SPEED Y", -5.0f, 5.0f, &mAutoRotateSpeed.y, 300.0f, dim);
        panel->addSlider("AUTO ROTATE SPEED Z", -5.0f, 5.0f, &mAutoRotateSpeed.z, 300.0f, dim);
        
        panel->addSpacer(300.0f, 1.0f);
        
        panel->addSlider("OFFSET", -300.0f, 300.0f, &mOffset, 300.0f, dim);
        
        panel->addSpacer(300.0f, 1.0f);
        
        panel->addButton("RESET", false, dim, dim);
		
#endif
	}
	
	void drawImGui()
	{
		ImGui::DragFloat("Angle X", &mEuler.x, 1, 0.0, 360);
		ImGui::DragFloat("Angle Y", &mEuler.y, 1, 0.0, 360);
		ImGui::DragFloat("Angle Z", &mEuler.z, 1, 0.0, 360);
		
		ImGui::Checkbox("Auto rotate X", &mAutoRotate.x);
		ImGui::Checkbox("Auto rotate Y", &mAutoRotate.y);
		ImGui::Checkbox("Auto rotate Z", &mAutoRotate.z);
		
		ImGui::DragFloat("Offset", &mOffset, 1, -300, 300);
		
		if (ImGui::Button("Reset"))
		{
			mAutoRotate.x = mAutoRotate.y = mAutoRotate.z = 0.0f;
			mEuler.set(0.0f);
			mAutoRotateSpeed.set(1.0f);
			mOffset = -3.0f;
		}
		
	}
	
	void setup()
	{
        mAutoRotateSpeed.set(1.0f);
        mOffset = -3.0f;
	}
    
    void loopAngle(float &a)
    {
        if (a>=360.0f) a=0.0f;
        else if (a<0.0f) a=360.0f;
    }
	
	
	void update()
	{
        if (mAutoRotate.x) {
            mEuler.x += mAutoRotateSpeed.x;
            loopAngle(mEuler.x);
        }
        if (mAutoRotate.y) {
            mEuler.y += mAutoRotateSpeed.y;
            loopAngle(mEuler.y);
        }
        if (mAutoRotate.z) {
            mEuler.z += mAutoRotateSpeed.z;
            loopAngle(mEuler.z);
        }
        
        ofMatrix4x4 mat;
        mat.rotate(mEuler.z, 0.0f, 0.0f, 1.0f);
        mat.rotate(mEuler.y, 0.0f, 1.0f, 0.0f);
        mat.rotate(mEuler.x, 1.0f, 0.0f, 0.0f);
        
        mRotation = mat.getRotate();
        for (int i=0; i<mUpsideDown.getNumFilters(); i++)
        {
            mUpsideDown.getFilter(i).setOffset(mOffset);
        }
	}
	
	void draw()
	{
        vector<rdtk::NodeArray> NAs;
        for (int i=0; i<getNumNodeArray(); i++)
        {
            rdtk::NodeArray tmpActor = getNodeArray(i);
            ofQuaternion base = tmpActor.getNode(rdtk::Actor::JOINT_HIPS).getOrientationQuat();
            ofQuaternion rotated = base * mRotation;
            tmpActor.getNode(rdtk::Actor::JOINT_HIPS).setOrientation(rotated);
            
            NAs.push_back(tmpActor);
        }
        
        vector<rdtk::NodeArray> filterdNAs = mUpsideDown.update(NAs);
        
        rdtk::BeginCamera();
        
        ofPushStyle();
        for (int i=0; i<NAs.size(); i++)
        {
            rdtk::NodeArray &NA = filterdNAs[i];
            ofSetColor(rdtk::Color::RED_DEEP);
            rdtk::DrawBasicActor(NA);
        }
        ofPopStyle();
        
        rdtk::EndCamera();
	}
	
	
	void onValueChanged(ofxUIEventArgs& e)
	{
        if (e.widget->getName() == "RESET") {
            mAutoRotate.x = mAutoRotate.y = mAutoRotate.z = 0.0f;
            mEuler.set(0.0f);
            mAutoRotateSpeed.set(1.0f);
            mOffset = -3.0f;
        }
	}
	
	string getName() const { return "Upside Down"; }
};

