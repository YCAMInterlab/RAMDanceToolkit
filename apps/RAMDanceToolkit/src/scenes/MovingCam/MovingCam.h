#pragma once

#include "ofxXmlSettings.h"
#include "ramBaseScene.h"

class MovingCam : public ramBaseScene
{
    
    enum MovingState
    {
        STATE_UNKNOWN = -1000,
        STATE_STOP = 0,
        STATE_MOVING_UP = 1,
        STATE_MOVING_DOWN = 2
    };
    
    struct CameraSetting
    {
        ofVec3f pos;
        ofVec3f lookAt;
        float fov;
        float angle;
    };
    
    CameraSetting base, opposite, current, moveFrom, moveTo;
    MovingState state;
    inline float getElapsedTime() { return ofGetElapsedTimef() - movingStartTime; }
    float movingStartTime;
    float movingDuration;
    
    float movingRadius;
    float movingAngle;
    
public:
	
	void setupControlPanel()
	{
		ofxUICanvas* panel = ramGetGUI().getCurrentUIContext();
        
        panel->addLabel("Press [u] to move up", OFX_UI_FONT_SMALL);
        panel->addLabel("Press [d] to move down", OFX_UI_FONT_SMALL);
        panel->addSpacer();
        panel->addButton("Start move up", false);
        panel->addButton("Start move down", false);
        panel->addButton("Reset & Reload XML", false);
        
        ofAddListener(panel->newGUIEvent, this, &MovingCam::onValuChanged);
	}
	
	void setup()
	{
        resetCamera();
	}
	
	void update()
	{
        if (state != STATE_STOP)
        {
            assert(state == STATE_MOVING_UP || state == STATE_MOVING_DOWN);
            
            const float direction =
                (state == STATE_MOVING_UP) ? 1 :
                (state == STATE_MOVING_DOWN) ? -1 : STATE_UNKNOWN;
            
            const float progress = getElapsedTime() / movingDuration;
            
            const float deg = (abs(moveFrom.angle)*2 * progress) * direction;
            
            ofVec3f axis, normal;
            normal = moveFrom.pos;
            normal.y = 0;
            axis = normal.getPerpendicular(ofVec3f(0, 1, 0));
            
            current.pos = moveFrom.pos.getRotated(deg, axis);
            
            getCameraManager().getActiveCamera().setPosition(current.pos);
            getCameraManager().getActiveCamera().lookAt(current.lookAt);
            
            if (progress >= 1.0)
            {
                current.pos = moveTo.pos;
                resetCamera();
            }
        }
	}
	
	void draw()
	{
        ramBeginCamera();
        
        ofPushStyle();
        
        ofSetColor(255,0,0);
        ofLine(ofVec3f::zero(), current.pos);
        
        // from
        ofVec3f strPos1 = base.pos;
        strPos1.y += 20;
        ofDrawBitmapString("Pos1", strPos1);
        ofBox(base.pos, 10);
        
        // to
        ofVec3f strPos2 = opposite.pos;
        strPos2.y += 20;
        ofDrawBitmapString("Pos2", strPos2);
        ofBox(opposite.pos, 10);
        
        // current
        ofSetColor(0, 0, 255);
        ofVec3f curStrPos = current.pos;
        curStrPos.y += 20;;
        ofDrawBitmapString("Cur", curStrPos);
        ofBox(current.pos, 10);
        
        ofPopStyle();
        
        ramEndCamera();
	}
    
    void onValuChanged(ofxUIEventArgs &e)
    {
        const string name = e.widget->getName();
        
        if (name == "Reset & Reload XML")
            resetCamera();
        
        else if (name == "Start move up")
            moveUp();
        
        else if (name == "Start move down")
            moveDown();
    }
    
    void reloadXML()
    {
        
#define _S(src) #src
		
		string default_xml = _S(
                                <cam>
                                    <duration>104</duration>
                                    <fov>27</fov>
                                    <pos><x>-620</x><y>300</y><z>620</z></pos>
                                    <look_at><x>0</x><y>50</y><z>0</z></look_at>
                                </cam>
                                );
        
#undef _S
		
		const string filePath = "MovingCam.xml";
		
		if (!ofFile::doesFileExist(filePath))
		{
			ofBuffer buf(default_xml);
			ofBufferToFile(filePath, buf);
		}
		
		ofxXmlSettings XML;
		XML.loadFile(filePath);
        
        movingDuration = XML.getValue("cam:duration", 100);
        
        base.pos.x = XML.getValue("cam:pos:x", 100);
        base.pos.y = XML.getValue("cam:pos:y", 100);
        base.pos.z = XML.getValue("cam:pos:z", 100);
        base.lookAt.x = XML.getValue("cam:look_at:x", 0);
        base.lookAt.y = XML.getValue("cam:look_at:y", 0);
        base.lookAt.z = XML.getValue("cam:look_at:z", 0);
        base.fov = XML.getValue("cam:fov", 60);
        
        movingRadius = base.pos.distance(ofVec3f::zero());
        const float triHeight = base.pos.y;
        const float triBase = sqrt(pow(movingRadius, 2) - pow(triHeight, 2));
        base.angle = ofRadToDeg( atan(triHeight/triBase) );
        
        opposite = base;
        opposite.pos.y *= -1;
        opposite.angle *= -1;
//        
//        cout << "loaded" << endl;
//        cout << movingDuration << endl;
//        cout << base.pos << endl;
//        cout << base.lookAt << endl;
//        cout << base.fov << endl;
//        cout << opposite.pos << endl;
//        cout << opposite.lookAt << endl;
//        cout << opposite.fov << endl;
//        
//        cout << "movingRadius:" << movingRadius << endl;
//        cout << "tribase, triheight:" << triBase << ", " << triHeight << endl;
//        cout << "movingAngle:"  << movingAngle << endl;
    }
    
    void resetCamera()
    {
        reloadXML();
        movingStartTime = ofGetElapsedTimef();
        state = STATE_STOP;
    }
    
    void moveUp()
    {
        resetCamera();
        state = STATE_MOVING_UP;
        moveFrom = (base.pos.y > 0) ? opposite : base;
        moveTo = (base.pos.y > 0) ? base : opposite;
    }
    
    void moveDown()
    {
        resetCamera();
        state = STATE_MOVING_DOWN;
        moveFrom = (base.pos.y > 0) ? base : opposite;
        moveTo = (base.pos.y > 0) ? opposite : base;
    }
    
    void onKeyPressed(ofKeyEventArgs &e)
    {
        switch (e.key)
        {
            case 'u': moveUp(); break;
            case 'd': moveDown(); break;
            default: break;
        }
    }
    
    void onEnabled()
    {
		cout << "[Unit enabled] " << getName() << endl;
        ofAddListener(ofEvents().keyPressed, this, &MovingCam::onKeyPressed);
    }
	
	void onDisabled()
	{
		cout << "[Unit disabled] " << getName() << endl;
        ofRemoveListener(ofEvents().keyPressed, this, &MovingCam::onKeyPressed);
        
	}
	
	string getName() const { return "MovingCam"; }
};
