#pragma once

const ofIndexType faces[] =
{
	// bottom
	2,1,0,
	3,5,6,
	3,4,5,
	
	// top
	7,8,9,
	10,12,13,
	10,11,12,
	
	// z minus side
	0,7,9,
	0,2,9,
	
	3,4,10,
	4,10,11,
	
	2,3,9,
	3,9,10,
	
	// z plus side
	0,7,8,
	0,1,8,
	
	1,6,8,
	6,8,13,
	
	12,13,5,
	5,6,13,
	
	// hip
	4,5,11,
	5,11,12,
};

const float verts[] =
{
	4.0f, 0.0f, 0.0f,
	2.0f, 0.0f, -2.0f,
	2.0f, 0.0f, 2.0f,
	2.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, -1.0f,
	2.0f, 0.0f, -1.0f,
	
	4.0f, 1.0f, 0.0f,
	2.0f, 1.0f, -2.0f,
	2.0f, 1.0f, 2.0f,
	2.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, -1.0f,
	2.0f, 1.0f, -1.0f,
};

const int kNumFaces = sizeof(faces) / sizeof(ofIndexType);
const int kNumVerts = sizeof(verts) / sizeof(float);

ofVec3f v[kNumVerts];
ofVec3f n[kNumVerts];
ofFloatColor c[kNumVerts];


class Arrow : public ofVbo
{
public:
	Arrow(){}
	void setup()
	{
		int i, j = 0;
		for ( i = 0; i < kNumVerts; i++ )
		{
			c[i].r = 3*i;
			c[i].g = 3*i;
			c[i].b = 3*i;
			
			v[i][0] = verts[j];
			j++;
			v[i][1] = verts[j];
			j++;
			v[i][2] = verts[j];
			j++;
		}
		
		setVertexData( &v[0], kNumVerts, GL_STATIC_DRAW );
		setColorData( &c[0], kNumVerts, GL_STATIC_DRAW );
		setIndexData( &faces[0], kNumFaces, GL_STATIC_DRAW );
	}
	
};