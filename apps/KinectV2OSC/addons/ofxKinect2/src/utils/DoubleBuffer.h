// @author sadmb
// @date 10th,Feb,2014.
// modified from utils/DepthRemapToRange.h of ofxNI2 by @satoruhiga

#pragma once

#include "ofMain.h"

namespace ofxKinect2
{
	template <typename PixelType>
	struct DoubleBuffer;
} // namespace ofxKinect2

template <typename PixelType>
struct ofxKinect2::DoubleBuffer
{
public:
	
	DoubleBuffer() : front_buffer_index(0), back_buffer_index(1), allocated(false) {}
	
	void allocate(int w, int h, int channels)
	{
		if (allocated) return;
		allocated = true;
		
		pix[0].allocate(w, h, channels);
		pix[1].allocate(w, h, channels);
	}
	
	void deallocate()
	{
		if (!allocated) return;
		allocated = false;

		pix[0].clear();
		pix[1].clear();
	}

	PixelType& getFrontBuffer() { return pix[front_buffer_index]; }
	const PixelType& getFrontBuffer() const { return pix[front_buffer_index]; }
	
	PixelType& getBackBuffer() { return pix[back_buffer_index]; }
	const PixelType& getBackBuffer() const { return pix[back_buffer_index]; }
	
	void swap()
	{
		std::swap(front_buffer_index, back_buffer_index);
	}
	
private:
	
	PixelType pix[2];
	int front_buffer_index, back_buffer_index;
	bool allocated;
};
