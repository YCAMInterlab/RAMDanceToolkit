#pragma once

#include "ramGlobal.h"
#include "ramControllable.h"

class ramUnit : public ramControllable, public ramGlobalShortcut
{
	friend class ramControlPanel;
	
public:
	
	virtual ~ramUnit() {}

};