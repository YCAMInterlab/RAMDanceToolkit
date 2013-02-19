#pragma once

#include "ofMain.h"

#include "ramControllable.h"
#include "ramControlPanel.h"

#include "ramActor.h"

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
	
	const ramNodeArray& update(const ramNodeArray& src)
	{
		const int f = ofGetFrameNum();
		if (last_update_frame != f)
		{
			last_update_frame = f;
			cache = filter(src);
//			cout << "updated: " << getName() << " " << ofGetElapsedTimef() << endl;
		}
		else
		{
//			cout << "use cache: " << getName() << " " << ofGetElapsedTimef() << endl;
		}
		
		return cache;
	}
	
	virtual const ramNodeArray& get(size_t index = 0) const { return cache; }
	virtual size_t getSize() const { return 1; }
	
protected:
	
	ramNodeArray cache;
	
	virtual const ramNodeArray& filter(const ramNodeArray& src) = 0;
	
private:
	
	int last_update_frame;
};


class ramBusSend : public ramBaseFilter
{
public:
	
	const string getName() { return "BusSend"; }
	
	ramBusSend() {}
	ramBusSend(const string &bus_name) { setBusName(bus_name); }
	
	void setBusName(const string &bus_name) { this->bus_name = bus_name; }
	const string& getBusName() const { return bus_name; }
	
	const ramNodeArray& get(size_t index = 0) const { static ramNodeArray arr; return arr; }
	size_t getSize() const { return 1; }
	
protected:
	
	const ramNodeArray& filter(const ramNodeArray& src)
	{
		ramActorManager::instance().setBus(bus_name, src);
		return src;
	}

private:
	
	string bus_name;
};
