// @author sadmb
// @date 10th,Feb,2014.
// modified from utils/DepthRemapToRange.h of ofxNI2 by @satoruhiga

#pragma once

#include "ofxKinect2.h"

namespace ofxKinect2
{
	class MeshGenerator;
} // namespace ofxKinect2

class ofxKinect2::MeshGenerator
{
public:
	
	MeshGenerator() : downsampling_level(1) {}
	
	void setup(DepthStream& depth_stream)
	{
		float fovH = depth_stream.getHorizontalFieldOfView();
		float fovV = depth_stream.getVerticalFieldOfView();
		
		xzFactor = tan(fovH * 0.5) * 2;
		yzFactor = tan(fovV * 0.5) * -2;
	}
	
	const ofMesh& update(const ofShortPixels& depth, const ofPixels& color = ofPixels())
	{
		assert(depth.getNumChannels() == 1);
		
		const int W = depth.getWidth();
		const int H = depth.getHeight();
		const float invW = 1. / W;
		const float invH = 1. / H;
		
		const unsigned short *depth_pix = depth.getPixels();
		
		bool has_color = color.isAllocated();
		const float inv_byte = 1. / 255.;
		
		mesh.setMode(OF_PRIMITIVE_POINTS);
		
		const int DS = downsampling_level;
		
		vector<ofVec3f>& verts = mesh.getVertices();
		verts.resize((W / DS) * (H / DS));
		
		int vert_index = 0;
		
		if (has_color)
		{
			const unsigned char *color_pix = color.getPixels();
			
			vector<ofFloatColor>& cols = mesh.getColors();
			cols.resize((W / DS) * (H / DS));
			
			if (color.getNumChannels() == 1)
			{
				for (int y = 0; y < H; y += DS)
				{
					for (int x = 0; x < W; x += DS)
					{
						const int idx = y * W + x;
						
						const float Z = depth_pix[idx];
						const float normX = x * invW - 0.5;
						const float normY = y * invH - 0.5;
						const float X = normX * xzFactor * Z;
						const float Y = normY * yzFactor * Z;
						verts[vert_index].set(X, Y, -Z);
						
						const unsigned char *C = &color_pix[idx];
						cols[vert_index].set(C[0] * inv_byte);
						
						vert_index++;
					}
				}
			}
			else if (color.getNumChannels() == 3)
			{
				for (int y = 0; y < H; y += DS)
				{
					for (int x = 0; x < W; x += DS)
					{
						const int idx = y * W + x;
						
						const float Z = depth_pix[idx];
						const float normX = x * invW - 0.5;
						const float normY = y * invH - 0.5;
						const float X = normX * xzFactor * Z;
						const float Y = normY * yzFactor * Z;
						verts[vert_index].set(X, Y, -Z);
						
						const unsigned char *C = &color_pix[idx * 3];
						cols[vert_index].set(C[0] * inv_byte,
											 C[1] * inv_byte,
											 C[2] * inv_byte);
						
						vert_index++;
					}
				}
			}
			else throw;
			
			mesh.addColors(cols);
		}
		else
		{
			for (int y = 0; y < H; y += DS)
			{
				for (int x = 0; x < W; x += DS)
				{
					int idx = y * W + x;
					
					float Z = depth_pix[idx];
					float X = (x * invW - 0.5) * xzFactor * Z;
					float Y = (y * invH - 0.5) * yzFactor * Z;
					verts[vert_index].set(X, Y, -Z);
					
					vert_index++;
				}
			}
		}
		
		return mesh;
	}
	
	void draw()
	{
		mesh.draw();
	}
	
	void setDownsamplingLevel(int level) { downsampling_level = level; }
	int getDownsamplingLevel() const { return downsampling_level; }
	
	ofMesh& getMesh() { return mesh; }
	
protected:
	
	int downsampling_level;
	
	ofMesh mesh;
	float xzFactor, yzFactor;
	
};
