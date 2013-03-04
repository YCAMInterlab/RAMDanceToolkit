#pragma once

#include "ramNodeArrayBuffer.h"
#include "ramBaseFilter.h"

class ramDelay : public ramBaseFilter
{
public:

	const string getName() { return "ramDelay"; }

	ramDelay(size_t delay_frame = 60) : delay_frame(delay_frame) {}

	void setFrame(size_t num) { delay_frame = num; if (delay_frame < 1) delay_frame = 1; };
	size_t getFrame() const { return delay_frame; }

	size_t getSize() const { return buffer.getSize(); }

	void setupControlPanel()
	{
		ramGetGUI().addSection(getName());
		ramGetGUI().addSlider("Delay", 1, 1000, &delay_frame);
	}

protected:

	ramNodeArrayBuffer buffer;

	float delay_frame;

	const ramNodeArray& filter(const ramNodeArray& src)
	{
		buffer.setCapacity(delay_frame);
		buffer.add(src);
		return buffer.get(delay_frame);
	}
};

class ramTimeShifter : public ramBaseFilter
{
public:

	const string getName() { return "ramTimeShifter"; }

	ramTimeShifter(size_t buffer_frame = 300) : rate(1), play_head(0), buffer_frame(buffer_frame)
	{
		setNumBufferFrame(buffer_frame);
	}

	void setNumBufferFrame(size_t buffer_frame) { this->buffer_frame = buffer_frame; }
	size_t getNumBufferFrame() const { return buffer_frame; }

	void setRate(float rate) { this->rate = rate; }
	float getRate() const { return rate; }

	void clear() { buffer.clear(); }

	void setupControlPanel()
	{
		ramGetGUI().addSection(getName());
		ramGetGUI().addSlider("Rate", -3, 3, &rate);
		ramGetGUI().addSlider("Buffer Frame", 1, 1000, &buffer_frame);
	}

protected:

	ramNodeArrayBuffer buffer;

	float rate;
	float play_head;
	float buffer_frame;

	const ramNodeArray& filter(const ramNodeArray& src)
	{
		buffer.setCapacity(buffer_frame);

		buffer.add(src);

		play_head += (60. / buffer.getSize()) / 60. * (-rate + 1);
		while (play_head > 1) play_head -= 1;
		while (play_head <= 0) play_head += 1;

		return buffer.get(((int)buffer.getSize() - 1) * play_head);
	}

};