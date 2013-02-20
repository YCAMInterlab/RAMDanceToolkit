#pragma once
#include "ofMain.h"

#include "ramGlobalShortcut.h"

#include "ramControllable.h"

#include "ofxUI.h"



/** Empty scene sample code

#pragma once

class EmptyScene : public ramBaseScene
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




class ramBaseScene : public ramControllable, public ramGlobalShortcut
{
	friend class ramControlPanel;

public:
	ramBaseScene() : bEnabled(false) {}
	virtual ~ramBaseScene(){}

	virtual string getSceneName() { return "unnamed scene"; }

	virtual void setup() {}
	virtual void update() {}
	virtual void draw() {}
	virtual void drawActor(ramActor &actor) {}
	virtual void drawRigid(ramRigidBody &rigid) {}
	virtual void drawHUD() {}

	inline void enable(){ bEnabled = true; }
	inline void disable() { bEnabled = false; }
	inline void toggle() { setEnabled(!isEnabled()); }
	inline bool isEnabled() { return bEnabled; }
	
	inline void setEnabled(bool b)
	{
		if (bEnabled == b) return;
		bEnabled = b;
		
		if (b)
			onEnabled();
		else
			onDisabled();
	}
	
	virtual void onEnabled() { cout << "[Scene enabled] " << getName() << endl; }
	virtual void onDisabled() { cout << "[Scene disabled] " << getName() << endl; }

	ramBaseScene* getPtr() { return this; }
	
protected:

	ramControlPanel& gui() { return *guiPtr; }

private:

	bool bEnabled;
	ramControlPanel *guiPtr;

};

