#pragma once

#define MAX_VERTICES 60000
#define MAX_INDICES 60000

#include "ofMain.h"

struct SBall {
	ofVec3f p;
	float m;
};

struct SVertex {
	ofVec3f v, n;
};

class MarchingCubes {
public:
	MarchingCubes();
	void setup(int resolution);	
	void update();
	void draw();
	
	void setCenters(const vector<ofVec3f>& centers);
	void setMasses(const vector<float>& masses);
	void setRadius(float minRadius, float maxRadius);
	
	const ofMesh& getMesh() const;
	
protected:	
	float ComputeEnergy(float x, float y, float z);
	void ComputeNormal(SVertex *pVertex);
	inline float ComputeGridPointEnergy(int x, int y, int z);
	int ComputeGridVoxel(int x, int y, int z);
	
	bool IsGridPointComputed(int x, int y, int z);
	bool IsGridVoxelComputed(int x, int y, int z);
	bool IsGridVoxelInList(int x, int y, int z);
	void SetGridPointComputed(int x, int y, int z);
	void SetGridVoxelComputed(int x, int y, int z);
	void SetGridVoxelInList(int x, int y, int z);
	
	inline float ConvertGridPointToWorldCoordinate(int x);
	int ConvertWorldCoordinateToGridPoint(float x);
	void AddNeighborsToList(int nCase, int x, int y, int z);
	void AddNeighbor(int x, int y, int z);
	
	static void BuildTables();
	static bool tablesBuilt;
	static char m_CubeEdges[12][2];
	static char m_CubeTriangles[256][16];
	static char m_CubeNeighbors[256];
	static float m_CubeVertices[8][3];
	
	vector<SBall> m_Balls;
	ofMesh mesh;
	float minRadius, maxRadius;
	
	int m_nNumOpenVoxels;
	int m_nMaxOpenVoxels;
	int *m_pOpenVoxels;
	
	int m_nGridSize;
	int m_nGridSizep1;
	float m_fVoxelSize;
	
	float *m_pfGridEnergy;
	char *m_pnGridPointStatus;
	char *m_pnGridVoxelStatus;
	
	int m_nMaxNumVertices;
	int m_nMaxNumIndices;
	
	int m_nNumVertices;
	int m_nNumIndices;
	SVertex *m_pVertices;
	unsigned short *m_pIndices;
};
