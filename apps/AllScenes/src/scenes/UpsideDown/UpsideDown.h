#pragma once

class UpsideDown : public ramBaseScene
{
	
public:
    
    ofVec3f mEuler;
    ofQuaternion mRotation;
    ramUpsideDown mUpsideDown;
    float mOffset;
    
    ofVec3f mAutoRotateSpeed;
    struct { bool x, y, z; } mAutoRotate;
	
	void setupControlPanel()
	{
#ifdef RAM_GUI_SYSTEM_OFXUI
		
		ramControlPanel &gui = ramGetGUI();
		
		ofxUICanvas* panel = gui.getCurrentUIContext();
		
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
        if (e.widget->getName() == "RESET") {
            mAutoRotate.x = mAutoRotate.y = mAutoRotate.z = 0.0f;
            mEuler.set(0.0f);
            mAutoRotateSpeed.set(1.0f);
            mOffset = -3.0f;
        }
	}
	
	const string getName() { return "UpsideDown scene"; }
};

