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

// NO texture coords or normals
// use ofGetIcoSphere(radius, 0) // 0 iterations will return Icosahedron //
//----------------------------------------------------------
ofMesh ofGetIcosahedronMesh(float radius) {
	ofMesh mesh;
	
	const float sqrt5 = sqrt(5.0f);
	const float phi = (1.0f + sqrt5) * 0.5f;
	
	/// Step 1 : Generate icosahedron
	float invnorm = 1/sqrt(phi*phi+1);
	
	mesh.addVertex(invnorm*ofVec3f(-1,  phi, 0));//0
	mesh.addVertex(invnorm*ofVec3f( 1,  phi, 0));//1
	mesh.addVertex(invnorm*ofVec3f(0,   1,  -phi));//2
	mesh.addVertex(invnorm*ofVec3f(0,   1,   phi));//3
	mesh.addVertex(invnorm*ofVec3f(-phi,0,  -1));//4
	mesh.addVertex(invnorm*ofVec3f(-phi,0,   1));//5
	mesh.addVertex(invnorm*ofVec3f( phi,0,  -1));//6
	mesh.addVertex(invnorm*ofVec3f( phi,0,   1));//7
	mesh.addVertex(invnorm*ofVec3f(0,   -1, -phi));//8
	mesh.addVertex(invnorm*ofVec3f(0,   -1,  phi));//9
	mesh.addVertex(invnorm*ofVec3f(-1,  -phi,0));//10
	mesh.addVertex(invnorm*ofVec3f( 1,  -phi,0));//11
	
	int firstFaces[] = {
		0,1,2,
		0,3,1,
		0,4,5,
		1,7,6,
		1,6,2,
		1,3,7,
		0,2,4,
		0,5,3,
		2,6,8,
		2,8,4,
		3,5,9,
		3,9,7,
		11,6,7,
		10,5,4,
		10,4,8,
		10,9,5,
		11,8,6,
		11,7,9,
		10,8,11,
		10,11,9
	};
	
	for(int i = 0; i < mesh.getNumVertices(); i++) {
		mesh.setVertex(i, mesh.getVertex(i) * radius);
	}
	
	for(int i = 0; i < 60; i+=3) {
		mesh.addTriangle(firstFaces[i], firstFaces[i+1], firstFaces[i+2]);
	}
	
	return mesh;
}

// based on code by Michael Broutin for the ogre-procedural library //
// http://code.google.com/p/ogre-procedural/source/browse/library/src/ProceduralIcoSphereGenerator.cpp
// For the latest info, see http://code.google.com/p/ogre-procedural/ //
//----------------------------------------------------------
ofMesh ofGetIcoSphereMesh(float radius, int iterations) {
	
	ofMesh icosahedron = ofGetIcosahedronMesh( 1.f );
	vector<ofVec3f> vertices = icosahedron.getVertices();
	vector<ofIndexType> faces = icosahedron.getIndices();
	
	int size = faces.size();
	
	/// Step 2 : tessellate
	for (unsigned short iteration = 0; iteration < iterations; iteration++)
	{
		size*=4;
		vector<ofIndexType> newFaces;
		newFaces.clear();
		//newFaces.resize(size);
		for (int i=0; i<size/12; i++)
		{
			int i1 = faces[i*3];
			int i2 = faces[i*3+1];
			int i3 = faces[i*3+2];
			int i12 = vertices.size();
			int i23 = i12+1;
			int i13 = i12+2;
			ofVec3f v1 = vertices[i1];
			ofVec3f v2 = vertices[i2];
			ofVec3f v3 = vertices[i3];
			//make 1 vertice at the center of each edge and project it onto the sphere
			vertices.push_back((v1+v2).normalized());
			vertices.push_back((v2+v3).normalized());
			vertices.push_back((v1+v3).normalized());
			//now recreate indices
			newFaces.push_back(i1);
			newFaces.push_back(i12);
			newFaces.push_back(i13);
			newFaces.push_back(i2);
			newFaces.push_back(i23);
			newFaces.push_back(i12);
			newFaces.push_back(i3);
			newFaces.push_back(i13);
			newFaces.push_back(i23);
			newFaces.push_back(i12);
			newFaces.push_back(i23);
			newFaces.push_back(i13);
		}
		faces.swap(newFaces);
	}
	
	/// Step 3 : generate texcoords
	vector<ofVec2f> texCoords;
	for (unsigned short i=0;i<vertices.size();i++)
	{
		const ofVec3f& vec = vertices[i];
		float u, v;
		float r0 = sqrtf(vec.x*vec.x+vec.z*vec.z);
		float alpha;
		alpha = atan2f(vec.z,vec.x);
		u = alpha/TWO_PI+.5f;
		v = atan2f(vec.y, r0)/PI + .5f;
		// reverse the u coord, so the default is texture mapped left to
		// right on the outside of a sphere //
		texCoords.push_back(ofVec2f(1.0-u,v));
	}
	
	/// Step 4 : fix texcoords
	// find vertices to split
	std::vector<int> indexToSplit;
	
	for (unsigned int i=0;i<faces.size()/3;i++)
	{
		ofVec2f& t0 = texCoords[faces[i*3+0]];
		ofVec2f& t1 = texCoords[faces[i*3+1]];
		ofVec2f& t2 = texCoords[faces[i*3+2]];
		
		if (abs(t2.x-t0.x)>0.5)
		{
			if (t0.x<0.5)
				indexToSplit.push_back(faces[i*3]);
			else
				indexToSplit.push_back(faces[i*3+2]);
		}
		if (abs(t1.x-t0.x)>0.5)
		{
			if (t0.x<0.5)
				indexToSplit.push_back(faces[i*3]);
			else
				indexToSplit.push_back(faces[i*3+1]);
		}
		if (abs(t2.x-t1.x)>0.5)
		{
			if (t1.x<0.5)
				indexToSplit.push_back(faces[i*3+1]);
			else
				indexToSplit.push_back(faces[i*3+2]);
		}
	}
	
	//split vertices
	for (unsigned short i=0;i<indexToSplit.size();i++)
	{
		int index = indexToSplit[i];
		//duplicate vertex
		ofVec3f v = vertices[index];
		ofVec2f t = texCoords[index] + ofVec2f(1.f, 0.f);
		vertices.push_back(v);
		texCoords.push_back(t);
		int newIndex = vertices.size()-1;
		//reassign indices
		for (unsigned short j=0;j<faces.size();j++)
		{
			if (faces[j]==index)
			{
				int index1 = faces[(j+1)%3+(j/3)*3];
				int index2 = faces[(j+2)%3+(j/3)*3];
				if ((texCoords[index1].x>0.5) || (texCoords[index2].x>0.5))
				{
					faces[j] = newIndex;
				}
			}
		}
	}
	
	
	ofMesh sphere;
	
	sphere.addIndices( faces );
	//sphere.addNormals( vertices );
	//sphere.addTexCoords( texCoords );
	
	for(int i = 0; i < vertices.size(); i++ ) {
		vertices[i] *= radius;
	}
	sphere.addVertices( vertices );
	
	return  sphere;
}

ofMesh icosphere;
void _ofSetIcoSphereResolution(int iterations) {
	icosphere = ofGetIcoSphereMesh(1, iterations);
}

void ofIcoSphere(ofVec3f center, float radius) {
	ofPushMatrix();
	ofTranslate(center);
	ofScale(radius, radius, radius);
	icosphere.drawVertices();
	icosphere.drawWireframe();
	ofPopMatrix();
}