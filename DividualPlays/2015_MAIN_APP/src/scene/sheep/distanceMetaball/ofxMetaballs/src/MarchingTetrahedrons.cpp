#include "MarchingTetrahedrons.h"

ofVec3f getNormal(const ofVec3f& v1, const ofVec3f& v2, const ofVec3f& v3) {
	ofVec3f a = v1 - v2;
	ofVec3f b = v3 - v2;
	ofVec3f normal = b.cross(a);
	normal.normalize();
	return normal;
}

ofMesh convertFromIndices(const ofMesh& mesh) {
	ofMesh result;
	// have to do a const_cast because ofMesh::get*() is not const correct
	ofMesh& cmesh = const_cast<ofMesh&>(mesh);
	int vertices = mesh.getNumVertices();
	int colors = mesh.getNumColors();
	int normals = mesh.getNumNormals();
	int texcoords = mesh.getNumTexCoords();
	int indices = mesh.getNumIndices();
	for(int i = 0; i < indices; i++) {
		int cur = cmesh.getIndex(i);
		if(vertices > 0) {
			result.addVertex(cmesh.getVertex(cur));
		}
		if(colors > 0) {
			result.addColor(cmesh.getColor(cur));
		}
		if(normals > 0) {
			result.addNormal(cmesh.getNormal(cur));
		}
		if(texcoords > 0) {
			result.addTexCoord(cmesh.getTexCoord(cur));
		}
	}
	return result;
}

void buildNormalsFaces(ofMesh& mesh) {
	for(int i = 0; i < mesh.getNumVertices(); i += 3) {
		int i0 = i + 0, i1 = i + 1, i2 = i + 2;
		ofVec3f normal;
		normal = getNormal(mesh.getVertices()[i0], mesh.getVertices()[i1], mesh.getVertices()[i2]);

		for(int j = 0; j < 3; j++) {
			mesh.addNormal(normal);
		}
	}
}

// assumes indexed vertices and triangles
void buildNormalsSingle(ofMesh& mesh) {
	vector<ofIndexType>& indices = mesh.getIndices();
	vector<bool> ready(mesh.getNumVertices());
	vector<ofVec3f> normals(mesh.getNumVertices());
	for(int i = 0; i < indices.size(); i += 3) {
		int i0 = indices[i + 0], i1 = indices[i + 1], i2 = indices[i + 2];
		ofVec3f normal = getNormal(mesh.getVertices()[i0], mesh.getVertices()[i1], mesh.getVertices()[i2]);
		if(!ready[i0]) {
			normals[i0] = normal;
			ready[i0] = true;
		}
		if(!ready[i1]) {
			normals[i1] = normal;
			ready[i1] = true;
		}
		if(!ready[i2]) {
			normals[i2] = normal;
			ready[i2] = true;
		}
	}
	mesh.addNormals(normals);
}

void buildNormalsAverage(ofMesh& mesh) {
	vector<ofIndexType>& indices = mesh.getIndices();
	vector<ofVec3f> normals(mesh.getNumVertices());
	for(int i = 0; i < indices.size(); i += 3) {
		int i0 = indices[i + 0], i1 = indices[i + 1], i2 = indices[i + 2];
		ofVec3f normal = getNormal(mesh.getVertices()[i0], mesh.getVertices()[i1], mesh.getVertices()[i2]);
		normals[i0] += normal;
		normals[i1] += normal;
		normals[i2] += normal;
	}
	for(int i = 0; i < normals.size(); i++) {
		normals[i].normalize();
	}
	mesh.addNormals(normals);
}

static int verticesLeft[] = {0,0,0,1,2,3};
static int verticesRight[] = {1,2,3,2,3,1};
static int intermediateIndicesCount[] = {0,3,3,4,3,4,4,3};
static int intermediateIndices[] = {
	//0000
	0,1,2,//0001 c
	0,5,3,//0010 c
	2,5,3,1,//0011 c
	1,3,4,//0100 c
	2,0,3,4,//0101 c
	5,4,1,0,//0110 c
	5,4,2//0111 c
};
static int tetrahedronIndices[] = {
	0,1,2,6, // c
	1,3,2,6, // c
	6,4,0,1, // c
	3,1,7,6, // c
	1,5,7,6, // c
	4,6,5,1 // c
};

void setBit(unsigned char& state, int position, bool value) {
	if(value) {
		state |= (1 << position);
	} else {
		state &= ~(1 << position);
	}
}

bool getBit(unsigned char state, int position) {
	return (state >> position) & 1;
}

void MarchingTetrahedrons::buildTetrahedrons() {
	int j = 0;
	tetrahedrons.resize(16);
	for(int i = 0; i < 8; i++) {
		int indexCount = intermediateIndicesCount[i];
		int lastIndex = j + indexCount;
		IndexedSamples tetra;
		for(; j < lastIndex; j++) {
			int curIndex = intermediateIndices[j];
			pair<int, int> curSample(verticesLeft[curIndex], verticesRight[curIndex]);
			tetra.samples.push_back(curSample);
		}
		if(indexCount > 0) {
			tetra.indices.push_back(0);
			tetra.indices.push_back(1);
			tetra.indices.push_back(2);
			if(indexCount > 3) {
				tetra.indices.push_back(0);
				tetra.indices.push_back(2);
				tetra.indices.push_back(3);
			}
		}
		tetrahedrons[i] = tetra;
		int inverted = 15 - i;
		reverse(tetra.indices.begin(), tetra.indices.end());
		tetrahedrons[inverted] = tetra;
	}
}

void MarchingTetrahedrons::buildCubes() {
	buildTetrahedrons();
	cubes.resize(256);
	for(int i = 0; i < 256; i++) {
		// compile all indices and samples
		vector<unsigned int> indices;
		vector< pair<int, int> > samples;
		for(int j = 0; j < 6; j++) {
			unsigned char state = 0;
			vector<int> tetraIndices;
			for(int k = 0; k < 4; k++) {
				int curIndex = tetrahedronIndices[j * 4 + k];
				setBit(state, k, getBit(i, curIndex));
				tetraIndices.push_back(curIndex);
			}
			IndexedSamples tetra = tetrahedrons[state];
			int base = samples.size();
			for(int k = 0; k < tetra.indices.size(); k++) {
				indices.push_back(tetra.indices[k] + base);
			}
			for(int k = 0; k < tetra.samples.size(); k++) {
				pair<int, int> cur = tetra.samples[k];
				cur.first = tetraIndices[cur.first];
				cur.second = tetraIndices[cur.second];
				samples.push_back(cur);
			}
		}
		// reduce all samples and load into cube
		IndexedSamples cube;
		for(int j = 0; j < indices.size(); j++) {
			int curIndex = indices[j];
			pair<int, int>& curSample = samples[curIndex];
			bool exists = false;
			for(int k = 0; k < cube.samples.size(); k++) {
				if(cube.samples[k] == curSample) {
					cube.indices.push_back(k);
					exists = true;
					break;
				}
			}
			if(!exists) {
				cube.indices.push_back(cube.samples.size());
				cube.samples.push_back(curSample);
			}
		}
		cubes[i] = cube;
	}
}

void MarchingTetrahedrons::setCenters(const vector<ofVec3f>& centers) {
	this->centers = centers;
}

void MarchingTetrahedrons::setRadius(float minRadius, float maxRadius) {
	this->minRadius = ofClamp(minRadius, 0, 1);
	this->maxRadius = ofClamp(maxRadius, 0, 1);
}

// modified from ken perlin http://www.geisswerks.com/ryan/BLOBS/blobs.html
inline float getEnergy(const ofVec3f& vertex, const ofVec3f& center, float min, float max) {
	float r = vertex.distance(center);
	if(r >= max) {
		return 0;
	}
	if(r <= min) {
		return 1;
	}
	r = (r - min) / (max - min);
	return 1 - (r * r * r * (r * (r * 6 - 15) + 10));
}

void MarchingTetrahedrons::buildCells() {
	// zero all the elements
	fill(cells.begin(), cells.end(), 0);
	
	unsigned long start = ofGetSystemTimeMicros();
	float actualMin = ofLerp(minRadius, maxRadius, -1);

	int maxOff = 1 + maxRadius * resolution;
	unsigned long totalCells = 0;
	for(int k = 0; k < centers.size(); k++) {
		ofVec3f& center = centers[k];
		ofVec3f cur = center * resolution;
		for(int zoff = -maxOff; zoff <= maxOff; zoff++) {
			int z = cur.z + zoff;
			if(z >= 0 && z < resolution) {
				for(int yoff = -maxOff; yoff <= maxOff; yoff++) {
					int y = cur.y + yoff;
					if(y >= 0 && y < resolution) {
						for(int xoff = -maxOff; xoff <= maxOff; xoff++) {
							int x = cur.x + xoff;
							if(x >= 0 && x < resolution) {
								float energy = getEnergy(getVertex(x, y, z), center, actualMin, maxRadius);
								cells[getIndex(x, y, z)] += energy;
								if(energy > 0) {
									totalCells++;
								}
							}
						}
					}
				}
			}
		}
	}
	ofLogVerbose() << "total cells worth evaluating: " << totalCells;
	
	unsigned long stop = ofGetSystemTimeMicros();
	ofLogVerbose() << "cells took " << (stop - start) / 1000. << " ms";
	
	// zero all the sides
	for(int z = 0; z < resolution; z += resolution - 1) {
		for(int y = 0; y < resolution; y++) {
			for(int x = 0; x < resolution; x++) {
				cells[getIndex(x, y, z)] = 0;
			}
		}
	}
	
	for(int y = 0; y < resolution; y += resolution - 1) {
		for(int z = 0; z < resolution; z++) {
			for(int x = 0; x < resolution; x++) {
				cells[getIndex(x, y, z)] = 0;
			}
		}
	}
	
	for(int x = 0; x < resolution; x += resolution - 1) {
		for(int z = 0; z < resolution; z++) {
			for(int y = 0; y < resolution; y++) {
				cells[getIndex(x, y, z)] = 0;
			}
		}
	}
}

ofVec3f MarchingTetrahedrons::getVertex(int i) {
	int x, y, z;
	getPosition(i, x, y, z);
	return getVertex(x, y, z);
}

ofVec3f MarchingTetrahedrons::getVertex(int x, int y, int z) {
	return ofVec3f((float) x / resolution, (float) y / resolution, (float) z / resolution);
}

int MarchingTetrahedrons::getIndex(int x, int y, int z) {
	return
		(x + 
		(y + 
		(z)
			* resolution)
			* resolution);
}

void MarchingTetrahedrons::getPosition(int i, int& x, int& y, int& z) {
	z = i / (resolution * resolution);
	i -= (z * resolution * resolution);
	y = i / (resolution);
	i -= (y * resolution);
	x = i;
}

void MarchingTetrahedrons::buildMesh() {
	mesh.clear();
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	int offsets[] = {
		getIndex(0, 0, 0),
		getIndex(1, 0, 0),
		getIndex(0, 1, 0),
		getIndex(1, 1, 0),
		getIndex(0, 0, 1),
		getIndex(1, 0, 1),
		getIndex(0, 1, 1),
		getIndex(1, 1, 1)};
	usageCount.resize(cubes.size(), 0);
	unsigned long start = ofGetSystemTimeMicros();
	for(int z = 0; z < resolution - 1; z++) {
		for(int y = 0; y < resolution - 1; y++) {
			for(int x = 0; x < resolution - 1; x++) {
				int i = getIndex(x, y, z);
				unsigned char state = 0;
				for(int j = 0; j < 8; j++) {
					int curIndex = i + offsets[j];
					setBit(state, j, cells[curIndex] > .5); // threshold
				}
				usageCount[state]++;
				IndexedSamples cube = cubes[state];
				vector<unsigned int> indices = cube.indices;
				for(int j = 0; j < indices.size(); j++) {
					indices[j] += mesh.getNumVertices();
				}
				mesh.addIndices(indices);
				vector< pair<int, int> >& samples = cube.samples;
				for(int j = 0; j < samples.size(); j++) {
					pair<int, int>& curSample = samples[j];
					int fromIndex = i + offsets[curSample.first];
					int toIndex = i + offsets[curSample.second];
					ofVec3f from = getVertex(fromIndex);
					ofVec3f to = getVertex(toIndex);
					float amt = ofNormalize(.5, cells[fromIndex], cells[toIndex]); // threshold
					ofVec3f center = from.getInterpolated(to, amt);
					mesh.addVertex(center); // normalized
				}
			}
		}
	}
	unsigned long stop = ofGetSystemTimeMicros();
	ofLogVerbose() << "meshing took " << (stop - start) / 1000. << " ms";
	ofLogVerbose() << "total vertices: " << mesh.getNumVertices();
	mesh = convertFromIndices(mesh);

	buildNormalsFaces(mesh);

	unsigned long total = resolution * resolution * resolution;
	unsigned long empty = (usageCount[0] + usageCount[255]);
	ofLogVerbose() << "full cells: " << (total - empty);
	ofLogVerbose() << "empty cells: " << empty << " (" << (100 * empty) / total << "%)";
}

MarchingTetrahedrons::MarchingTetrahedrons()
:minRadius(0)
,maxRadius(0) {
	buildCubes();
}

void MarchingTetrahedrons::setup(int resolution) {
	this->resolution = resolution;
	if(minRadius == 0 && maxRadius == 0) {
		setRadius(1. / resolution, 3. / resolution);
	}
	cells.resize(resolution * resolution * resolution);
}

void MarchingTetrahedrons::update() {
	buildCells();
	buildMesh();
}

void MarchingTetrahedrons::draw() {
	ofSetColor(255);
	glEnable(GL_DEPTH_TEST);
	ofNoFill();
	ofBox(1);
	ofTranslate(-.5, -.5, -.5);
	ofSetColor(0);
	mesh.draw();
	ofSetColor(255);
	glDepthFunc(GL_LEQUAL);
	mesh.drawWireframe();
}

const ofMesh& MarchingTetrahedrons::getMesh() const {
	return mesh;
}