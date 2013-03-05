#pragma once

#include "ramControllable.h"
#include "ramGlobal.h"

class ramUnit : public ramControllable, public ramGlobalShortcut
{
public:	
	virtual ~ramUnit() {}
};