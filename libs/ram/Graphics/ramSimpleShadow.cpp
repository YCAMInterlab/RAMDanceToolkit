// 
// ramSimpleShadow.cpp - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "ramSimpleShadow.h"

#include "ramCameraManager.h"

#include <numeric>

using namespace rdtk;

void SimpleShadow::setup()
{
	enable = true;

	// defulat light position
	setLightPosition(ofVec3f(-100.0f, 500.0f, 200.0f));
	shadow_color.set(Color::SHADOW);

#define _S(src) # src

	const char *vs = _S(
			uniform vec4 shadow_color;
			uniform mat4 shadow_matrix;
			uniform mat4 modelview_matrix;
			uniform mat4 modelview_matrix_inv;

			void main()
			{
				mat4 model_matrix = modelview_matrix_inv * gl_ModelViewMatrix;

				gl_FrontColor = shadow_color;
				gl_Position = gl_ProjectionMatrix * modelview_matrix * shadow_matrix * model_matrix * gl_Vertex;
			}
			);

#undef _S

	shader.setupShaderFromSource(GL_VERTEX_SHADER, vs);
	shader.linkProgram();
}

void SimpleShadow::setLightPosition(ofVec3f pos)
{
	static const float groundplane[] = { 0.0, 1.0, 0.0, 1.0 };
	const float* lightpos = pos.getPtr();

	shadow_matrix.makeIdentityMatrix();
	float* shadowMatrix = shadow_matrix.getPtr();

	float dot = inner_product(groundplane, groundplane + 4, lightpos, 0.f);

	for (int y = 0; y < 4; ++y)
	{
		for (int x = 0; x < 4; ++x)
		{

			shadowMatrix[y * 4 + x] = -groundplane[y] * lightpos[x];
			if (x == y) shadowMatrix[y * 4 + x] += dot;
		}
	}
}

void SimpleShadow::setShadowColor(ofFloatColor color)
{
	shadow_color = color;
}

void SimpleShadow::begin()
{
	assert(enable);

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glEnable(GL_DEPTH_TEST);

	ofMatrix4x4 modelview_matrix = CameraManager::instance().getActiveCamera().getModelViewMatrix();
	ofMatrix4x4 modelview_matrix_inv = modelview_matrix.getInverse();

	shader.begin();
	shader.setUniform4f("shadow_color", shadow_color.r, shadow_color.g, shadow_color.b, shadow_color.a);
	shader.setUniformMatrix4f("shadow_matrix", shadow_matrix);
	shader.setUniformMatrix4f("modelview_matrix", modelview_matrix);
	shader.setUniformMatrix4f("modelview_matrix_inv", modelview_matrix_inv);
}

void SimpleShadow::end()
{
	shader.end();

	glPopAttrib();
}

void SimpleShadow::setShadowAlpha(float alpha)
{
	shadow_color = ofFloatColor(shadow_color, alpha);
}
