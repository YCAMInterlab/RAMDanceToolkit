#include "ramGlobal.h"

#include <numeric>

static ramGlobalContext *ctx = NULL;


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

void ramGlobalContext::init()
{
	ramGetGUI().setup();
	
	ramGetGUI().loadFont(ramToResourcePath("Fonts/din-webfont.ttf"), 10);
	ramGetGUI().loadCameraSettings("settings.camera.xml");
	
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
//	gl::calcShadowMatrix(gl::kGroundPlaneYUp, pos.getPtr(), shadow_matrix.getPtr());
}
