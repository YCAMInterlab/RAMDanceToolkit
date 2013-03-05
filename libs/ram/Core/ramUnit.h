#pragma once

#include "ramControllable.h"
#include "ramGlobal.h"

class ramUnit : public ramControllable, public ramGlobalShortcut
{
public:	
	ramUnit() : bEnabled(false) {} 
	virtual ~ramUnit() {}
	
	void enable()
	{
		bEnabled = true;
	}
	
	void disable()
	{
		bEnabled = false;
	}
	
	void toggle()
	{
		setEnabled(!isEnabled());
	}
	
	bool isEnabled()
	{
		return bEnabled;
	}
	
	void setEnabled(bool enabled)
	{
		if (bEnabled == enabled) return;
		
		bEnabled = enabled;
		
		if (enabled)
			onEnabled();
		else
			onDisabled();
	}
	
	
	
	void onEnabled()
	{
		cout << "[Unit enabled] " << getName() << endl;
	}
	
	void onDisabled()
	{
		cout << "[Unit disabled] " << getName() << endl;
	}
	
protected:
	
	bool bEnabled;
};