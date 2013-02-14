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



class ramControlPanel;

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
	
	virtual void onEnabled() { cout << "[Scene enabled] " << getName() << endl; }
	virtual void onDisabled() { cout << "[Scene disabled] " << getName() << endl; }

	inline void enable(){ bEnabled = true; }
	inline void disable() { bEnabled = false; }
	inline void toggle() { bEnabled ^= true; }
	inline void setEnabled(bool b) { bEnabled = b; }
	inline bool isEnabled() { return bEnabled; }
	
	/// !!!:
	// gwenに刺しかわったらscene.onEnabled / disabled 追加
//	void enable()
//	{
//		bEnabled = true;
//		onEnabled();
//	}
//	void disable()
//	{
//		bEnabled = false;
//		onDisabled();
//	}
//	void toggle()
//	{
//		bEnabled ^= true;
//		if (bEnabled)
//			onEnabled();
//		else
//			onDisabled();
//	}
//	void setEnabled(bool b)
//	{
//		bEnabled = b;
//		if (bEnabled)
//			onEnabled();
//		else
//			onDisabled();
//	}

	ramBaseScene* getPtr() { return this; }
	
	
protected:

	ramControlPanel& gui() { return *guiPtr; }

private:

	bool bEnabled;
	ramControlPanel *guiPtr;

};

