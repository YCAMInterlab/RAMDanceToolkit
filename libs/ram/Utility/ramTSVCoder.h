#pragma once

#include "ramBaseCoder.h"
#include "ramSession.h"

class ramTSVCoder : public ramBaseCoder
{
protected:
	ramSession decode(ofBuffer buffer);
	const bool encode(const ramSession &src);
};

