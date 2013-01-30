#include "ramShadow.h"

#include <numeric>

namespace gl
{
	//ax + by + cz + d = 0;
	static const float kGroundPlaneXUp[] = { 1.0, 0.0, 0.0, 1.0 };
	static const float kGroundPlaneYUp[] = { 0.0, 1.0, 0.0, 1.0 };
	static const float kGroundPlaneZUp[] = { 0.0, 0.0, 1.0, 1.0 };
	
	inline void calcShadowMatrix(const float groundplane[4],
								 const float lightpos[3],
								 float shadowMatrix[16]);
	
	
	void calcShadowMatrix(const float groundplane[],
						  const float lightpos[],
						  float shadowMatrix[])
	{
		float dot = inner_product(groundplane, groundplane+4, lightpos, 0.f);
		
		for(int y = 0; y < 4;++y) {
			for(int x = 0; x < 4; ++x) {
				
				shadowMatrix[y*4+x] = - groundplane[y]*lightpos[x];
				if (x == y) shadowMatrix[y*4+x] += dot;
			}
		}
	}
}

void ramShadow::setup()
{

}

void ramShadow::setLightPosition(ofVec3f pos)
{
	gl::calcShadowMatrix(gl::kGroundPlaneYUp, pos.getPtr(), shadow_matrix.getPtr());
}

void ramShadow::begin()
{
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	
	glPushMatrix();
	glMultMatrixf(shadow_matrix.getPtr());

	shader.begin();
}

void ramShadow::end()
{
	shader.end();
	
	glPopMatrix();
	glPopAttrib();
}
