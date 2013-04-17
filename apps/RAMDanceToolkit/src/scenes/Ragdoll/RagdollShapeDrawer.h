// 
// RagdollShapeDrawer.h - RAMDanceToolkit
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

/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/
#pragma once

class btCollisionShape;
class btShapeHull;
#include "LinearMath/btAlignedObjectArray.h"
#include "LinearMath/btVector3.h"

#include "BulletCollision/CollisionShapes/btShapeHull.h"

/// OpenGL shape drawing
class RagdollShapeDrawer
{
protected:
	struct ShapeCache
	{
	struct Edge { btVector3 n[2];int v[2]; };
	ShapeCache(btConvexShape* s) : m_shapehull(s) {}
	btShapeHull					m_shapehull;
	btAlignedObjectArray<Edge>	m_edges;
	};
	//clean-up memory of dynamically created shape hulls
	btAlignedObjectArray<ShapeCache*>	m_shapecaches;
	unsigned int						m_texturehandle;
	bool								m_textureenabled;
	bool								m_textureinitialized;
	

	ShapeCache*							cache(btConvexShape*);

public:
		RagdollShapeDrawer();

		virtual ~RagdollShapeDrawer();

		///drawOpenGL might allocate temporary memoty, stores pointer in shape userpointer
		virtual void		drawOpenGL(btScalar* m, const btCollisionShape* shape, const btVector3& color,int	debugMode,const btVector3& worldBoundsMin,const btVector3& worldBoundsMax);
		virtual void		drawShadow(btScalar* m, const btVector3& extrusion,const btCollisionShape* shape,const btVector3& worldBoundsMin,const btVector3& worldBoundsMax);
		
    virtual void		drawOpenGL(btScalar* m, const btCollisionShape* shape, const btVector3& color);
    virtual void		drawShadow(btScalar* m, const btVector3& extrusion,const btCollisionShape* shape);
    
    
		bool		enableTexture(bool enable) { bool p=m_textureenabled;m_textureenabled=enable;return(p); }
		bool		hasTextureEnabled() const
		{
			return m_textureenabled;
		}
		
		static void		drawCylinder(float radius,float halfHeight, int upAxis);
		void			drawSphere(btScalar r, int lats, int longs);
		static void		drawCoordSystem();
		
};

void OGL_displaylist_register_shape(btCollisionShape * shape);
void OGL_displaylist_clean();
