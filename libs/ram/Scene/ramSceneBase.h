#pragma once
#include "ofMain.h"

#include "ramActorManager.h"
#include "ramCameraManager.h"

#include "ramControllable.h"

#include "ofxUI.h"



/** Empty scene sample code
 
#pragma once
 
class EmptyScene : public ramSceneBase
{
	
public:
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		 
		panel->addWidgetDown(new ofxUILabel(getName(), OFX_UI_FONT_LARGE));
		panel->addSpacer(gui.kLength, 2);
		panel->addSlider("Font Color", 0.0, 255.0, &fontColor, gui.kLength, gui.kDim);
	
		ofAddListener(panel->newGUIEvent, this, &BigBox::onPanelChanged);
	}
	
	void setup()
	{
		
	}
	
	
	void update()
	{
		
	}
	
	void draw()
	{
	
	}
	
	void drawActor( ramActor& actor )
	{
		
	}
	
	void drawRigid(ramRigidBody &rigid)
	{
		
	}
	 
	void onPanelChanged(ofxUIEventArgs& e)
	{
		string name = e.widget->getName();
	}
	
	const string getName() { return "My scene"; }
};
*/
 
 

class ramControlPanel;

class ramSceneBase : public ramControllable
{
	friend class ramControlPanel;
	
public:
	ramSceneBase() : bEnabled(false) {}
	virtual ~ramSceneBase(){}
	
	virtual string getSceneName() { return "unnamed scene"; }

	inline ramActorManager& getActorManager() { return ramActorManager::instance(); }
	inline ramActor& getActor(string name) { return ramActorManager::instance().getActor(name); }
	inline ramRigidBody& getRigidBody(string name) { return ramActorManager::instance().getRigidBody(name); }
	inline ofCamera& getActiveCamera() { return ramCameraManager::instance().getActiveCamera(); }
	
	virtual void setup() {}
	virtual void update() {}
	virtual void draw() {}
	virtual void drawActor(ramActor &actor) {}
	virtual void drawRigid(ramRigidBody &rigid) {}
	
	inline void enable(){ bEnabled = true; }
	inline void disable() { bEnabled = false; }
	inline void toggle() { bEnabled ^= true; }
	inline void setEnabled(bool b) { bEnabled = b; }
	inline bool isEnabled() { return bEnabled; }
	
	ramSceneBase* getPtr() { return this; }
	
protected:
	
	ramControlPanel& gui() { return *guiPtr; }
	
private:
	
	bool bEnabled;
	ramControlPanel *guiPtr;
	
};

