#pragma once

class UpsideDown : public ramBaseScene
{
	
public:
    
    ofVec3f mEuler;
    ofQuaternion mRotation;
    ramUpsideDown mUpsideDown;
    
    ofVec3f mAutoRotateSpeed;
    struct { bool x, y, z; } mAutoRotate;
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ofAddListener(panel->newGUIEvent, this, &UpsideDown::onValueChanged);
        
        panel->addSlider("ANGLE X", 0.0f, 360.0f, &mEuler.x, 300.0f, 16.0f);
        panel->addSlider("ANGLE Y", 0.0f, 360.0f, &mEuler.y, 300.0f, 16.0f);
        panel->addSlider("ANGLE Z", 0.0f, 360.0f, &mEuler.z, 300.0f, 16.0f);
        
        panel->addSpacer(300.0f, 16.0f);
        
        panel->addToggle("AUTO ROTATE X", &mAutoRotate.x, 16.0f, 16.0f);
        panel->addToggle("AUTO ROTATE Y", &mAutoRotate.y, 16.0f, 16.0f);
        panel->addToggle("AUTO ROTATE Z", &mAutoRotate.z, 16.0f, 16.0f);
        
        panel->addSlider("AUTO ROTATE SPEED X", -5.0f, 5.0f, &mAutoRotateSpeed.x, 300.0f, 16.0f);
        panel->addSlider("AUTO ROTATE SPEED Y", -5.0f, 5.0f, &mAutoRotateSpeed.y, 300.0f, 16.0f);
        panel->addSlider("AUTO ROTATE SPEED Z", -5.0f, 5.0f, &mAutoRotateSpeed.z, 300.0f, 16.0f);
	}
    
	void setup()
	{
        mAutoRotateSpeed.set(1.0f);
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
	}
	
	void draw()
	{
		ramBeginCamera();

        
		ramEndCamera();
	}
	
	void drawActor(ramActor& actor)
	{

        ramActor tmpActor = actor;
        
        ofQuaternion base = tmpActor.getNode(ramActor::JOINT_HIPS).getOrientationQuat();
        ofQuaternion rotated = base * mRotation;
        tmpActor.getNode(ramActor::JOINT_HIPS).setOrientation(rotated);
        
        ramActor filterd = mUpsideDown.update(tmpActor);
        
        ramDrawBasicActor(filterd, ramColor::RED_DEEP, ramColor::RED_LIGHT);
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
		
	}
	
	void drawFloor()
	{
        
	}
	
	void onValueChanged(ofxUIEventArgs& e)
	{

	}
	
	const string getName() { return "UpsideDown scene"; }
};

