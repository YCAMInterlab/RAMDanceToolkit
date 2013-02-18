#pragma once

#include "ofMain.h"

#include "ramControllable.h"
#include "ramControlPanel.h"

/** sample code...

#pragma once

class ramXXXFilter : public ramBaseFilter
{
	
public:
	
	ramXXXFilter() {}
	
	void setupControlPanel(ofxUICanvas* panel)
	{
		ramControlPanel &gui = ramGetGUI();
		
		panel->addWidgetDown(new ofxUILabel(getName(), OFX_UI_FONT_LARGE));
		panel->addSpacer(gui.kLength, 2);
		
		ofAddListener(panel->newGUIEvent, this, &ramXXXFilter::onPanelChanged);
	}
	
	void setup()
	{
		
	}
	
	const ramNodeArray& update(ramNodeArray& src)
	{
		return src;
	}
	
	void onPanelChanged(ofxUIEventArgs& e)
	{

	}
	
	const string getName() { return "ramXXXFilter"; }
};
 */



class ramBaseFilter : public ramControllable, public ramGlobalShortcut
{
public:
	
	virtual ~ramBaseFilter() {}
	
	virtual void setupControlPanel(ofxUICanvas* panel) {}
	virtual void setup() {}
	
	virtual const ramNodeArray& update(const ramNodeArray& src) { return src; }
	
	// FIXME: remove pre vertual 
	virtual const ramNodeArray& get(size_t index = 0) const = 0; // { static ramNodeArray arr; return arr; }
	virtual size_t getSize() const = 0; // { return 1; }
};

class ramBusSend : public ramBaseFilter
{
public:
	
	ramBusSend() {}
	ramBusSend(const string &bus_name) { setBusName(bus_name); }
	
	void setBusName(const string &bus_name) { this->bus_name = bus_name; }
	const string& getBusName() const { return bus_name; }
	
	const ramNodeArray& update(const ramNodeArray& src)
	{
		ramActorManager::instance().setBus(bus_name, src);
		return src;
	}
	
	const ramNodeArray& get(size_t index = 0) const { static ramNodeArray arr; return arr; }
	size_t getSize() const { return 1; }
	
private:
	
	string bus_name;
};
