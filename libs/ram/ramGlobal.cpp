#include "ramGlobal.h"

#include <numeric>

static ramGlobalContext *ctx = NULL;

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

ramGlobalContext& ramGlobal()
{
	if (ctx == NULL)
	{
		ctx = new ramGlobalContext;
	}
	return *ctx;
}

string ramToResourcePath(string path)
{
	return ofFilePath::join(ofToDataPath("../../../../resources"), path);
}

ramControlPanel& ramGlobalContext::getGUI()
{
	return *gui;
}

void ramGlobalContext::init()
{
	gui = new ramControlPanel;
	gui->setup();
	
	gui->loadFont(ramToResourcePath("Fonts/din-webfont.ttf"), 10);
	gui->loadCameraSettings("settings.camera.xml");
	
	setLightPosition(ofVec3f(-100.0f, 500.0f, 200.0f));
}


void ramGlobalContext::beginShadowMatrix()
{
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	
	glPushMatrix();
	glMultMatrixf(shadow_matrix.getPtr());
}

void ramGlobalContext::endShadowMatrix()
{
	glPopMatrix();
	glPopAttrib();
}

void ramGlobalContext::setLightPosition(ofVec3f pos)
{
	gl::calcShadowMatrix(gl::kGroundPlaneYUp, pos.getPtr(), shadow_matrix.getPtr());
}
