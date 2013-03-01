#pragma once

class UpsideDownDonuts : public ramBaseScene
{
    
    ofVec3f mEuler;
    ofQuaternion mRotation;
    ramUpsideDown mUpsideDown;
    float mOffset;
    
    ofVec3f mAutoRotateSpeed;
    struct { bool x, y, z; } mAutoRotate;
	
	
	//-------------------
	// donuts
	ofxUIToggle *mToggles[ramActor::NUM_JOINTS];
	bool mNodeVisibility[ramActor::NUM_JOINTS];
	
	float mNumDuplicate;
	float mRadius;
	float mScale;
	float mBoxSize;
	ofVec3f mTranslate;
	bool mShowActor;
	
	int mNumDuplicates;
	float mRadian;

	
public:
	
	UpsideDownDonuts() {}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		ofAddListener(panel->newGUIEvent, this, &UpsideDownDonuts::onValueChanged);
        
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
		
		
		
		// --
		// donuts
		panel->addSpacer(gui.kLength, 2);
		panel->addSlider("Scale", 0.1, 10.0, &mScale, gui.kLength, gui.kDim);
		panel->addSlider("Position: X", -300.0, 300.0, &mTranslate.x, gui.kLength, gui.kDim);
		panel->addSlider("Position: Z", -300.0, 300.0, &mTranslate.z, gui.kLength, gui.kDim);
		
		panel->addSpacer(gui.kLength, 2);
		panel->addSlider("Duplicate", 1, 200, &mNumDuplicate, gui.kLength, gui.kDim);
		panel->addSlider("Radius", 10.0, 1000.0, &mRadius, gui.kLength, gui.kDim);
		panel->addSlider("Box Size", 1.0, 1000.0, &mBoxSize, gui.kLength, gui.kDim);
		
		panel->addSpacer(gui.kLength, 2);
		panel->addButton("Reset all settings", false, 20, 20);
		panel->addToggle("Show Actor", &mShowActor, 20, 20);
		panel->addToggle("Toggle All", false, 20, 20);
		
		for (int i=0; i<ramActor::NUM_JOINTS; i++)
		{
			mNodeVisibility[i] = (i == ramActor::JOINT_RIGHT_HAND || i == ramActor::JOINT_LEFT_HAND);
			mToggles[i] = panel->addToggle(ramActor::getJointName(i), &mNodeVisibility[i], 8, 8);
		}
		
		mNumDuplicate = floor(mNumDuplicate);
		mRadian = 2 * M_PI / mNumDuplicate;
		clearDonuts();
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
        
        mUpsideDown.setOffset(mOffset);
	}
	
	void draw()
	{
		if (getNumNodeArray() < 1) return;
		ramBeginCamera();
		
		int index = -1;
		for(int i=0; i<getNumNodeArray(); i++)
		{
			if(getNodeArray(i).isActor())
			{
				index = i;
				break;
			}
		}
		if (index < 0) return;
		
        ramActor tmpActor = getNodeArray(index);
        
        ofQuaternion base = tmpActor.getNode(ramActor::JOINT_HIPS).getOrientationQuat();
        ofQuaternion rotated = base * mRotation;
        tmpActor.getNode(ramActor::JOINT_HIPS).setOrientation(rotated);
        
        ramActor filterd = mUpsideDown.update(tmpActor);
        
		drawDonuts( filterd );
		ramEndCamera();
	}
	
	
	
	// --
	// donuts
	
	
	void drawDonuts(ramNodeArray &nodeArray)
	{
		ofPushStyle();
		ofNoFill();
		
		if (mShowActor)
		{
			for (int n=0; n<mNumDuplicate; n++)
			{
				ofColor c1 = ramColor::RED_DEEP;
				ofColor c2 = ramColor::BLUE_LIGHT;
				c1.g += n*3;
				c2.b += n*3;
				
				float x = cos( mRadian * n ) * mRadius;
				float z = sin( mRadian * n ) * mRadius;
				
				ofPushMatrix();
				ofTranslate(x, 0, z);
				if( n==0 ) ofTranslate(mTranslate.x, mTranslate.y, mTranslate.z);
				
				ofRotateY(360/mNumDuplicate * n);
				if( n==0 ) ofScale(mScale, mScale, mScale);
				
				ramDrawNodes(nodeArray);
				
				ofPopMatrix();
			}
		}
		
		else
		{
			for (int index=0; index<nodeArray.getNumNode(); index++)
			{
				if (mNodeVisibility[index] == false) continue;
				
				for (int n=0; n<mNumDuplicate; n++)
				{
					ramNode &node = nodeArray.getNode(index);
					
					ofPushMatrix();
					{
						ofColor c1 = ramColor::RED_DEEP;
						ofColor c2 = ramColor::BLUE_LIGHT;
						c1.g += n*3;
						c2.b += n*3;
						
						float x = cos( mRadian * n ) * mRadius;
						float z = sin( mRadian * n ) * mRadius;
						
						ofMatrix4x4 m = node.getOrientationQuat();
						glMultMatrixf(m.getPtr());
						
						ofTranslate(x, 0, z);
						if( n==0 ) ofTranslate(mTranslate.x, mTranslate.y, mTranslate.z);
						
						ofRotateY(360/mNumDuplicate * n);
						if( n==0 ) ofScale(mScale, mScale, mScale);
						
						node.beginTransform();
						ofSetColor(c2);
						ofBox(mBoxSize);
						node.endTransform();
					}
					ofPopMatrix();
				}
			}
		}
		
		
		ofPopStyle();
		
	}
	void setAllVisiblity(bool b)
	{
		for (int i=0; i<ramActor::NUM_JOINTS; i++)
			mToggles[i]->setValue(b);
	}
	
	void clearDonuts()
	{
		for (int i=0; i<ramActor::NUM_JOINTS; i++)
		{
			mNodeVisibility[i] = (i == ramActor::JOINT_RIGHT_HAND || i == ramActor::JOINT_LEFT_HAND);
			mToggles[i]->setValue(mNodeVisibility[i]);
		}
		
		mNumDuplicate = 20;
		mRadius = 110;
		mScale = 1.0;
		mBoxSize = 3.0;
		mTranslate = ofVec3f::zero();
		mShowActor = false;
	}
	
	// ---
	
	void onValueChanged(ofxUIEventArgs& e)
	{
		string name = e.widget->getName();
		
        if (e.widget->getName() == "RESET") {
            mAutoRotate.x = mAutoRotate.y = mAutoRotate.z = 0.0f;
            mEuler.set(0.0f);
            mAutoRotateSpeed.set(1.0f);
            mOffset = -3.0f;
        }
		
		// ---
		// Donuts
		if (name == "Toggle All")
		{
			ofxUIToggle *t = (ofxUIToggle *)e.widget;
			setAllVisiblity(t->getValue());
		}
		
		if (name == "Reset all settings")
		{
			clearDonuts();
		}
	}
	
	const string getName() { return "Upside Down Donuts"; }
};
