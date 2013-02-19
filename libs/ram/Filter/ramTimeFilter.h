#pragma once

#include "ramNodeArrayBuffer.h"
#include "ramBaseFilter.h"

class ramDelay : public ramBaseFilter
{
public:
	
	const string getName() { return "Delay"; }
	
	ramDelay(size_t delay_frame = 60) : delay_frame(delay_frame) {}
	
	void setFrame(size_t num) { delay_frame = num; };
	size_t getFrame() const { return delay_frame; }
	
	size_t getSize() const { return buffer.getSize(); }
	
protected:
	
	ramNodeArrayBuffer buffer;
	
	size_t delay_frame;
	
	const ramNodeArray& filter(const ramNodeArray& src)
	{
		buffer.add(src);
		return buffer.get(delay_frame);
	}
};


class ramTimeShifter : public ramBaseFilter
{
public:
	
	const string getName() { return "TimeShifter"; }
	
	ramTimeShifter(size_t buffer_frame = 300) : rate(1), play_head(0)
	{
		setNumBufferFrame(buffer_frame);
	}
	
	void setNumBufferFrame(size_t buffer_frame)
	{
		buffer.setCapacity(buffer_frame);
	}
	
	size_t getNumBufferFrame() const { return buffer.getCapacity(); }
	
	void setRate(float rate) { this->rate = rate; }
	float getRate() const { return rate; }

protected:

	ramNodeArrayBuffer buffer;
	
	float rate;
	float play_head;
	
	const ramNodeArray& filter(const ramNodeArray& src)
	{
		buffer.add(src);
		
		play_head += (60. / buffer.getSize()) / 60. * (-rate + 1);
		while (play_head > 1) play_head -= 1;
		while (play_head <= 0) play_head += 1;
		
		return buffer.get(((int)buffer.getSize() - 1) * play_head);
	}

};
