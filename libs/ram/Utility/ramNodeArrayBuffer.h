#pragma once

#include "ramActor.h"

class ramNodeArrayBuffer
{
public:

	ramNodeArrayBuffer() {}

	void add(const ramNodeArray& arr)
	{
		buffer.push_front(arr);
		if (buffer.size() > capacity)
			buffer.pop_back();
	}

	void clear()
	{
		buffer.clear();
	}

	void setCapacity(size_t capacity)
	{
		assert(capacity >= 0);
		this->capacity = capacity;
		while (buffer.size() > capacity)
			buffer.pop_back();
	}

	size_t getCapacity() const { return capacity; }

	// TODO: return linear interpolated nodeArray

	ramNodeArray& get(size_t index)
	{
		if (index >= buffer.size()) return buffer.back();

		return buffer[index];
	}

	const ramNodeArray& get(size_t index) const
	{
		if (index >= buffer.size()) return buffer.back();

		return buffer[index];
	}

	size_t getSize() const { return buffer.size(); }

private:

	size_t capacity;
	deque<ramNodeArray> buffer;
};