// 
// SphereMesh.h - RAMDanceToolkit
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

#pragma once

#include "ramMain.h"

// NO texture coords or normals
// use ofGetIcoSphere(radius, 0) // 0 iterations will return Icosahedron //
//----------------------------------------------------------
ofMesh ofGetIcosahedronMesh(float radius);

// based on code by Michael Broutin for the ogre-procedural library //
// http://code.google.com/p/ogre-procedural/source/browse/library/src/ProceduralIcoSphereGenerator.cpp
// For the latest info, see http://code.google.com/p/ogre-procedural/ //
//----------------------------------------------------------
ofMesh ofGetIcoSphereMesh(float radius, int iterations);
void _ofSetIcoSphereResolution(int iterations);
void ofIcoSphere(ofVec3f center, float radius);