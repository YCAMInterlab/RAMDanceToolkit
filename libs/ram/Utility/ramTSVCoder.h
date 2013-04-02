#pragma once

#include "ramBaseCoder.h"
#include "ramSession.h"

class ramTSVCoder : public ramBaseCoder
{
protected:
	void decode(ofBuffer buffer);
	bool encode(ramSession &src);
};

