#include "MarchingCubes.h"

float fLevel = .5;

void MarchingCubes::BuildTables()
{
	// Build CubeNeighbors table
	
	// Cube neighbors
	//
	// bit 0 : x + 1
	// bit 1 : x - 1
	// bit 2 : y + 1
	// bit 3 : y - 1
	// bit 4 : z + 1
	// bit 5 : z - 1
	
	for( int i = 0; i < 256; i++ )
	{
		int c = 0;
		if(	(i & 0x66) != 0 && (i & 0x66) != 0x66 ) c |= (1<<0);
		if(	(i & 0x99) != 0 && (i & 0x99) != 0x99 ) c |= (1<<1);
		if(	(i & 0xF0) != 0 && (i & 0xF0) != 0xF0 ) c |= (1<<2);
		if(	(i & 0x0F) != 0 && (i & 0x0F) != 0x0F ) c |= (1<<3);
		if(	(i & 0xCC) != 0 && (i & 0xCC) != 0xCC ) c |= (1<<4);
		if(	(i & 0x33) != 0 && (i & 0x33) != 0x33 ) c |= (1<<5);
		
		m_CubeNeighbors[i] = c;
	}
}

// Static data

//        +----------+
//       /|7        /|6
//      / |        / |
//     +----------+  |
//    4|  |       |5 |
//     |  |       |  |
//     |  |       |  |
//     |  +-------|--+
//     | / 3      | / 2
//     |/         |/
//     +----------+
//    0            1       

//        +----------+
//      7/|    6    /|
//      / |        /5|
//     +----------+  |
//     |  |  4    |  |11
//     |10|       |  |
//    8|  |       |9 |
//     |  +-------|--+
//     | /     2  | /
//     |/3        |/1
//     +----------+
//           0           

// Cube vertices
float MarchingCubes::m_CubeVertices[8][3] = 
{
	{0,0,0},
	{1,0,0},
	{1,0,1},
	{0,0,1},
	{0,1,0},
	{1,1,0},
	{1,1,1},
	{0,1,1}
};

// This is the edges and the direction on them. They are designed so
// that edges of neighboring cubes are in the same direction.
char MarchingCubes::m_CubeEdges[12][2] = 
{
	{0,1}, {1,2}, {3,2}, {0,3},
	{4,5}, {5,6}, {7,6}, {4,7},
	{0,4}, {1,5}, {3,7}, {2,6}
};

// This list gives the edges that the triangles in each case intersect.
char MarchingCubes::m_CubeTriangles[256][16] = 
{ 
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 0  
	{ 3,  0,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 1  
	{ 9,  0,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 2  
	{ 3,  1,  8,  1,  9,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 3  
	{11,  1,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 4  
	{ 3,  0,  8, 11,  1,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 5  
	{11,  9,  2,  9,  0,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 6  
	{ 3,  2,  8,  8,  2, 11,  8, 11,  9, -1, -1, -1, -1, -1, -1, -1}, // 7  
	{ 2,  3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 8 
	{ 2,  0, 10,  0,  8, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 9 
	{ 0,  1,  9, 10,  2,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 10  
	{ 2,  1, 10, 10,  1,  9, 10,  9,  8, -1, -1, -1, -1, -1, -1, -1}, // 11  
	{ 1,  3, 11,  3, 10, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 12  
	{ 1,  0, 11, 11,  0,  8, 11,  8, 10, -1, -1, -1, -1, -1, -1, -1}, // 13  
	{ 0,  3,  9,  9,  3, 10,  9, 10, 11, -1, -1, -1, -1, -1, -1, -1}, // 14  
	{11,  9,  8, 10, 11,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 15  
	{ 8,  4,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 16  
	{ 0,  4,  3,  4,  7,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 17  
	{ 9,  0,  1,  7,  8,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 18  
	{ 9,  4,  1,  1,  4,  7,  1,  7,  3, -1, -1, -1, -1, -1, -1, -1}, // 19  
	{11,  1,  2,  7,  8,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 20  
	{ 7,  3,  4,  4,  3,  0, 11,  1,  2, -1, -1, -1, -1, -1, -1, -1}, // 21  
	{11,  9,  2,  2,  9,  0,  7,  8,  4, -1, -1, -1, -1, -1, -1, -1}, // 22  
	{ 9,  2, 11,  7,  2,  9,  3,  2,  7,  4,  7,  9, -1, -1, -1, -1}, // 23  
	{ 7,  8,  4,  2,  3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 24  
	{ 7, 10,  4,  4, 10,  2,  4,  2,  0, -1, -1, -1, -1, -1, -1, -1}, // 25  
	{ 1,  9,  0,  7,  8,  4, 10,  2,  3, -1, -1, -1, -1, -1, -1, -1}, // 26  
	{10,  4,  7, 10,  9,  4,  2,  9, 10,  1,  9,  2, -1, -1, -1, -1}, // 27  
	{ 1,  3, 11, 11,  3, 10,  4,  7,  8, -1, -1, -1, -1, -1, -1, -1}, // 28  
	{11,  1, 10, 10,  1,  4,  4,  1,  0,  4,  7, 10, -1, -1, -1, -1}, // 29  
	{ 8,  4,  7, 10,  9,  0, 11,  9, 10,  3, 10,  0, -1, -1, -1, -1}, // 30  
	{10,  4,  7,  9,  4, 10, 11,  9, 10, -1, -1, -1, -1, -1, -1, -1}, // 31  
	{ 4,  9,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 32  
	{ 4,  9,  5,  3,  0,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 33  
	{ 4,  0,  5,  0,  1,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 34  
	{ 4,  8,  5,  5,  8,  3,  5,  3,  1, -1, -1, -1, -1, -1, -1, -1}, // 35  
	{11,  1,  2,  4,  9,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 36  
	{ 8,  3,  0, 11,  1,  2,  5,  4,  9, -1, -1, -1, -1, -1, -1, -1}, // 37  
	{11,  5,  2,  2,  5,  4,  2,  4,  0, -1, -1, -1, -1, -1, -1, -1}, // 38  
	{ 5,  2, 11,  5,  3,  2,  4,  3,  5,  8,  3,  4, -1, -1, -1, -1}, // 39  
	{ 4,  9,  5, 10,  2,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 40  
	{ 2,  0, 10, 10,  0,  8,  5,  4,  9, -1, -1, -1, -1, -1, -1, -1}, // 41  
	{ 4,  0,  5,  5,  0,  1, 10,  2,  3, -1, -1, -1, -1, -1, -1, -1}, // 42  
	{ 5,  2,  1,  8,  2,  5, 10,  2,  8,  5,  4,  8, -1, -1, -1, -1}, // 43  
	{10, 11,  3,  3, 11,  1,  4,  9,  5, -1, -1, -1, -1, -1, -1, -1}, // 44  
	{ 5,  4,  9,  1,  0,  8,  1,  8, 11, 11,  8, 10, -1, -1, -1, -1}, // 45  
	{ 0,  5,  4, 10,  5,  0, 11,  5, 10,  3, 10,  0, -1, -1, -1, -1}, // 46  
	{ 8,  5,  4, 11,  5,  8, 10, 11,  8, -1, -1, -1, -1, -1, -1, -1}, // 47  
	{ 8,  9,  7,  9,  5,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 48  
	{ 0,  9,  3,  3,  9,  5,  3,  5,  7, -1, -1, -1, -1, -1, -1, -1}, // 49  
	{ 8,  0,  7,  7,  0,  1,  7,  1,  5, -1, -1, -1, -1, -1, -1, -1}, // 50  
	{ 3,  1,  5,  7,  3,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 51  
	{ 8,  9,  7,  7,  9,  5,  2, 11,  1, -1, -1, -1, -1, -1, -1, -1}, // 52  
	{ 2, 11,  1,  0,  9,  5,  0,  5,  3,  3,  5,  7, -1, -1, -1, -1}, // 53  
	{ 2,  8,  0,  5,  8,  2,  7,  8,  5,  2, 11,  5, -1, -1, -1, -1}, // 54  
	{ 5,  2, 11,  3,  2,  5,  7,  3,  5, -1, -1, -1, -1, -1, -1, -1}, // 55  
	{ 5,  7,  9,  9,  7,  8,  2,  3, 10, -1, -1, -1, -1, -1, -1, -1}, // 56  
	{ 7,  9,  5,  2,  9,  7,  0,  9,  2, 10,  2,  7, -1, -1, -1, -1}, // 57  
	{10,  2,  3,  8,  0,  1,  8,  1,  7,  7,  1,  5, -1, -1, -1, -1}, // 58  
	{ 1, 10,  2,  7, 10,  1,  5,  7,  1, -1, -1, -1, -1, -1, -1, -1}, // 59  
	{ 8,  9,  5,  7,  8,  5,  3, 11,  1, 10, 11,  3, -1, -1, -1, -1}, // 60  
	{ 0,  5,  7,  9,  5,  0,  0,  7, 10, 11,  1,  0,  0, 10, 11, -1}, // 61  
	{ 0, 10, 11,  3, 10,  0,  0, 11,  5,  7,  8,  0,  0,  5,  7, -1}, // 62  
	{ 5, 10, 11,  5,  7, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 63  
	{ 5, 11,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 64  
	{ 3,  0,  8,  6,  5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 65  
	{ 1,  9,  0,  6,  5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 66  
	{ 3,  1,  8,  8,  1,  9,  6,  5, 11, -1, -1, -1, -1, -1, -1, -1}, // 67  
	{ 5,  1,  6,  1,  2,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 68  
	{ 5,  1,  6,  6,  1,  2,  8,  3,  0, -1, -1, -1, -1, -1, -1, -1}, // 69  
	{ 5,  9,  6,  6,  9,  0,  6,  0,  2, -1, -1, -1, -1, -1, -1, -1}, // 70  
	{ 8,  5,  9,  2,  5,  8,  6,  5,  2,  8,  3,  2, -1, -1, -1, -1}, // 71  
	{10,  2,  3,  5, 11,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 72  
	{ 8, 10,  0,  0, 10,  2,  5, 11,  6, -1, -1, -1, -1, -1, -1, -1}, // 73  
	{ 9,  0,  1, 10,  2,  3,  6,  5, 11, -1, -1, -1, -1, -1, -1, -1}, // 74  
	{ 6,  5, 11,  2,  1,  9,  2,  9, 10, 10,  9,  8, -1, -1, -1, -1}, // 75  
	{10,  6,  3,  3,  6,  5,  3,  5,  1, -1, -1, -1, -1, -1, -1, -1}, // 76  
	{10,  0,  8,  5,  0, 10,  1,  0,  5,  6,  5, 10, -1, -1, -1, -1}, // 77  
	{ 6,  3, 10,  6,  0,  3,  5,  0,  6,  9,  0,  5, -1, -1, -1, -1}, // 78  
	{ 9,  6,  5, 10,  6,  9,  8, 10,  9, -1, -1, -1, -1, -1, -1, -1}, // 79  
	{ 6,  5, 11,  8,  4,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 80  
	{ 0,  4,  3,  3,  4,  7, 11,  6,  5, -1, -1, -1, -1, -1, -1, -1}, // 81  
	{ 0,  1,  9,  6,  5, 11,  7,  8,  4, -1, -1, -1, -1, -1, -1, -1}, // 82  
	{ 5, 11,  6,  7,  1,  9,  3,  1,  7,  4,  7,  9, -1, -1, -1, -1}, // 83  
	{ 2,  6,  1,  1,  6,  5,  8,  4,  7, -1, -1, -1, -1, -1, -1, -1}, // 84  
	{ 5,  1,  2,  6,  5,  2,  4,  3,  0,  7,  3,  4, -1, -1, -1, -1}, // 85  
	{ 7,  8,  4,  5,  9,  0,  5,  0,  6,  6,  0,  2, -1, -1, -1, -1}, // 86  
	{ 9,  7,  3,  4,  7,  9,  9,  3,  2,  6,  5,  9,  9,  2,  6, -1}, // 87  
	{ 2,  3, 10,  4,  7,  8,  5, 11,  6, -1, -1, -1, -1, -1, -1, -1}, // 88 
	{ 6,  5, 11,  2,  4,  7,  0,  4,  2, 10,  2,  7, -1, -1, -1, -1}, // 89  
	{ 9,  0,  1,  8,  4,  7, 10,  2,  3,  6,  5, 11, -1, -1, -1, -1}, // 90  
	{ 1,  9,  2,  2,  9, 10, 10,  9,  4,  4,  7, 10,  6,  5, 11, -1}, // 91  
	{ 7,  8,  4,  5,  3, 10,  1,  3,  5,  6,  5, 10, -1, -1, -1, -1}, // 92  
	{10,  5,  1,  6,  5, 10, 10,  1,  0,  4,  7, 10, 10,  0,  4, -1}, // 93 
	{ 9,  0,  5,  5,  0,  6,  6,  0,  3,  3, 10,  6,  7,  8,  4, -1}, // 94  
	{ 9,  6,  5, 10,  6,  9,  9,  4,  7,  9,  7, 10, -1, -1, -1, -1}, // 95  
	{ 9, 11,  4, 11,  6,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 96  
	{ 6,  4, 11, 11,  4,  9,  3,  0,  8, -1, -1, -1, -1, -1, -1, -1}, // 97  
	{ 1, 11,  0,  0, 11,  6,  0,  6,  4, -1, -1, -1, -1, -1, -1, -1}, // 98 
	{ 1,  8,  3,  6,  8,  1,  4,  8,  6, 11,  6,  1, -1, -1, -1, -1}, // 99 
	{ 9,  1,  4,  4,  1,  2,  4,  2,  6, -1, -1, -1, -1, -1, -1, -1}, // 100  
	{ 8,  3,  0,  9,  1,  2,  9,  2,  4,  4,  2,  6, -1, -1, -1, -1}, // 101  
	{ 4,  0,  2,  6,  4,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 102  
	{ 2,  8,  3,  4,  8,  2,  6,  4,  2, -1, -1, -1, -1, -1, -1, -1}, // 103  
	{ 9, 11,  4,  4, 11,  6,  3, 10,  2, -1, -1, -1, -1, -1, -1, -1}, // 104  
	{ 2,  0,  8, 10,  2,  8, 11,  4,  9,  6,  4, 11, -1, -1, -1, -1}, // 105  
	{ 2,  3, 10,  6,  0,  1,  4,  0,  6, 11,  6,  1, -1, -1, -1, -1}, // 106  
	{ 1,  6,  4, 11,  6,  1,  1,  4,  8, 10,  2,  1,  1,  8, 10, -1}, // 107  
	{ 4,  9,  6,  6,  9,  3,  3,  9,  1,  3, 10,  6, -1, -1, -1, -1}, // 108  
	{ 1,  8, 10,  0,  8,  1,  1, 10,  6,  4,  9,  1,  1,  6,  4, -1}, // 109  
	{ 6,  3, 10,  0,  3,  6,  4,  0,  6, -1, -1, -1, -1, -1, -1, -1}, // 110  
	{ 8,  6,  4,  8, 10,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 111  
	{ 6,  7, 11, 11,  7,  8, 11,  8,  9, -1, -1, -1, -1, -1, -1, -1}, // 112  
	{ 3,  0,  7,  7,  0, 11, 11,  0,  9, 11,  6,  7, -1, -1, -1, -1}, // 113  
	{ 7, 11,  6,  7,  1, 11,  8,  1,  7,  0,  1,  8, -1, -1, -1, -1}, // 114  
	{ 7, 11,  6,  1, 11,  7,  3,  1,  7, -1, -1, -1, -1, -1, -1, -1}, // 115 
	{ 6,  1,  2,  8,  1,  6,  9,  1,  8,  7,  8,  6, -1, -1, -1, -1}, // 116  
	{ 9,  2,  6,  1,  2,  9,  9,  6,  7,  3,  0,  9,  9,  7,  3, -1}, // 117  
	{ 0,  7,  8,  6,  7,  0,  2,  6,  0, -1, -1, -1, -1, -1, -1, -1}, // 118  
	{ 2,  7,  3,  2,  6,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 119  
	{10,  2,  3,  8, 11,  6,  9, 11,  8,  7,  8,  6, -1, -1, -1, -1}, // 120  
	{ 7,  2,  0, 10,  2,  7,  7,  0,  9, 11,  6,  7,  7,  9, 11, -1}, // 121 
	{ 0,  1,  8,  8,  1,  7,  7,  1, 11, 11,  6,  7, 10,  2,  3, -1}, // 122  
	{ 1, 10,  2,  7, 10,  1,  1, 11,  6,  1,  6,  7, -1, -1, -1, -1}, // 123  
	{ 6,  8,  9,  7,  8,  6,  6,  9,  1,  3, 10,  6,  6,  1,  3, -1}, // 124  
	{ 1,  0,  9,  7, 10,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 125  
	{ 0,  7,  8,  6,  7,  0,  0,  3, 10,  0, 10,  6, -1, -1, -1, -1}, // 126  
	{ 6,  7, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 127  
	{10,  7,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 128  
	{ 8,  3,  0,  6, 10,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 129  
	{ 9,  0,  1,  6, 10,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 130  
	{ 9,  8,  1,  1,  8,  3,  6, 10,  7, -1, -1, -1, -1, -1, -1, -1}, // 131  
	{ 2, 11,  1,  7,  6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 132  
	{11,  1,  2,  8,  3,  0,  7,  6, 10, -1, -1, -1, -1, -1, -1, -1}, // 133  
	{ 0,  2,  9,  9,  2, 11,  7,  6, 10, -1, -1, -1, -1, -1, -1, -1}, // 134  
	{ 7,  6, 10,  3,  2, 11,  3, 11,  8,  8, 11,  9, -1, -1, -1, -1}, // 135  
	{ 3,  7,  2,  7,  6,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 136  
	{ 8,  7,  0,  0,  7,  6,  0,  6,  2, -1, -1, -1, -1, -1, -1, -1}, // 137  
	{ 6,  2,  7,  7,  2,  3,  9,  0,  1, -1, -1, -1, -1, -1, -1, -1}, // 138  
	{ 2,  1,  6,  6,  1,  8,  8,  1,  9,  6,  8,  7, -1, -1, -1, -1}, // 139  
	{ 6, 11,  7,  7, 11,  1,  7,  1,  3, -1, -1, -1, -1, -1, -1, -1}, // 140  
	{ 6, 11,  7, 11,  1,  7,  7,  1,  8,  8,  1,  0, -1, -1, -1, -1}, // 141  
	{ 7,  0,  3, 11,  0,  7,  9,  0, 11,  7,  6, 11, -1, -1, -1, -1}, // 142  
	{11,  7,  6,  8,  7, 11,  9,  8, 11, -1, -1, -1, -1, -1, -1, -1}, // 143  
	{ 4,  6,  8,  6, 10,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 144  
	{10,  3,  6,  6,  3,  0,  6,  0,  4, -1, -1, -1, -1, -1, -1, -1}, // 145  
	{10,  8,  6,  6,  8,  4,  1,  9,  0, -1, -1, -1, -1, -1, -1, -1}, // 146  
	{ 6,  9,  4,  3,  9,  6,  1,  9,  3,  6, 10,  3, -1, -1, -1, -1}, // 147  
	{ 4,  6,  8,  8,  6, 10,  1,  2, 11, -1, -1, -1, -1, -1, -1, -1}, // 148  
	{11,  1,  2, 10,  3,  0, 10,  0,  6,  6,  0,  4, -1, -1, -1, -1}, // 149  
	{ 8,  4, 10, 10,  4,  6,  9,  0,  2,  9,  2, 11, -1, -1, -1, -1}, // 150  
	{ 3, 11,  9,  2, 11,  3,  3,  9,  4,  6, 10,  3,  3,  4,  6, -1}, // 151  
	{ 3,  8,  2,  2,  8,  4,  2,  4,  6, -1, -1, -1, -1, -1, -1, -1}, // 152  
	{ 2,  0,  4,  2,  4,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 153  
	{ 0,  1,  9,  4,  2,  3,  6,  2,  4,  8,  4,  3, -1, -1, -1, -1}, // 154  
	{ 4,  1,  9,  2,  1,  4,  6,  2,  4, -1, -1, -1, -1, -1, -1, -1}, // 155  
	{ 3,  8,  1,  1,  8,  6,  6,  8,  4,  1,  6, 11, -1, -1, -1, -1}, // 156  
	{ 0, 11,  1,  6, 11,  0,  4,  6,  0, -1, -1, -1, -1, -1, -1, -1}, // 157  
	{ 3,  4,  6,  8,  4,  3,  3,  6, 11,  9,  0,  3,  3, 11,  9, -1}, // 158  
	{ 4, 11,  9,  4,  6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 159  
	{ 5,  4,  9, 10,  7,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 160  
	{ 3,  0,  8,  5,  4,  9,  6, 10,  7, -1, -1, -1, -1, -1, -1, -1}, // 161  
	{ 1,  5,  0,  0,  5,  4, 10,  7,  6, -1, -1, -1, -1, -1, -1, -1}, // 162  
	{ 6, 10,  7,  4,  8,  3,  4,  3,  5,  5,  3,  1, -1, -1, -1, -1}, // 163  
	{ 4,  9,  5,  2, 11,  1, 10,  7,  6, -1, -1, -1, -1, -1, -1, -1}, // 164  
	{ 7,  6, 10, 11,  1,  2,  3,  0,  8,  5,  4,  9, -1, -1, -1, -1}, // 165  
	{10,  7,  6, 11,  5,  4, 11,  4,  2,  2,  4,  0, -1, -1, -1, -1}, // 166  
	{ 8,  3,  4,  4,  3,  5,  5,  3,  2,  2, 11,  5,  6, 10,  7, -1}, // 167  
	{ 3,  7,  2,  2,  7,  6,  9,  5,  4, -1, -1, -1, -1, -1, -1, -1}, // 168  
	{ 4,  9,  5,  6,  0,  8,  2,  0,  6,  7,  6,  8, -1, -1, -1, -1}, // 169  
	{ 2,  3,  6,  6,  3,  7,  0,  1,  5,  0,  5,  4, -1, -1, -1, -1}, // 170  
	{ 8,  6,  2,  7,  6,  8,  8,  2,  1,  5,  4,  8,  8,  1,  5, -1}, // 171 
	{ 4,  9,  5,  6, 11,  1,  6,  1,  7,  7,  1,  3, -1, -1, -1, -1}, // 172  
	{11,  1,  6,  6,  1,  7,  7,  1,  0,  0,  8,  7,  4,  9,  5, -1}, // 173  
	{11,  4,  0,  5,  4, 11, 11,  0,  3,  7,  6, 11, 11,  3,  7, -1}, // 174  
	{11,  7,  6,  8,  7, 11, 11,  5,  4, 11,  4,  8, -1, -1, -1, -1}, // 175  
	{ 5,  6,  9,  9,  6, 10,  9, 10,  8, -1, -1, -1, -1, -1, -1, -1}, // 176  
	{10,  3,  6,  3,  0,  6,  6,  0,  5,  5,  0,  9, -1, -1, -1, -1}, // 177  
	{ 8,  0, 10, 10,  0,  5,  5,  0,  1, 10,  5,  6, -1, -1, -1, -1}, // 178  
	{ 3,  6, 10,  5,  6,  3,  1,  5,  3, -1, -1, -1, -1, -1, -1, -1}, // 179  
	{11,  1,  2, 10,  9,  5,  8,  9, 10,  6, 10,  5, -1, -1, -1, -1}, // 180  
	{ 3,  0, 10, 10,  0,  6,  6,  0,  9,  9,  5,  6, 11,  1,  2, -1}, // 181  
	{ 5, 10,  8,  6, 10,  5,  5,  8,  0,  2, 11,  5,  5,  0,  2, -1}, // 182  
	{ 3,  6, 10,  5,  6,  3,  3,  2, 11,  3, 11,  5, -1, -1, -1, -1}, // 183  
	{ 9,  5,  8,  8,  5,  2,  2,  5,  6,  2,  3,  8, -1, -1, -1, -1}, // 184  
	{ 6,  9,  5,  0,  9,  6,  2,  0,  6, -1, -1, -1, -1, -1, -1, -1}, // 185  
	{ 8,  1,  5,  0,  1,  8,  8,  5,  6,  2,  3,  8,  8,  6,  2, -1}, // 186  
	{ 6,  1,  5,  6,  2,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 187  
	{ 6,  1,  3, 11,  1,  6,  6,  3,  8,  9,  5,  6,  6,  8,  9, -1}, // 188  
	{ 0, 11,  1,  6, 11,  0,  0,  9,  5,  0,  5,  6, -1, -1, -1, -1}, // 189  
	{ 8,  0,  3, 11,  5,  6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 190  
	{ 6, 11,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 191  
	{11, 10,  5, 10,  7,  5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 192  
	{11, 10,  5,  5, 10,  7,  0,  8,  3, -1, -1, -1, -1, -1, -1, -1}, // 193  
	{ 7,  5, 10, 10,  5, 11,  0,  1,  9, -1, -1, -1, -1, -1, -1, -1}, // 194  
	{ 5, 11,  7,  7, 11, 10,  1,  9,  8,  1,  8,  3, -1, -1, -1, -1}, // 195  
	{ 2, 10,  1,  1, 10,  7,  1,  7,  5, -1, -1, -1, -1, -1, -1, -1}, // 196  
	{ 3,  0,  8,  7,  1,  2,  5,  1,  7, 10,  7,  2, -1, -1, -1, -1}, // 197  
	{ 5,  9,  7,  7,  9,  2,  2,  9,  0,  7,  2, 10, -1, -1, -1, -1}, // 198  
	{ 2,  7,  5, 10,  7,  2,  2,  5,  9,  8,  3,  2,  2,  9,  8, -1}, // 199  
	{11,  2,  5,  5,  2,  3,  5,  3,  7, -1, -1, -1, -1, -1, -1, -1}, // 200  
	{ 0,  8,  2,  2,  8,  5,  5,  8,  7,  5, 11,  2, -1, -1, -1, -1}, // 201  
	{ 1,  9,  0,  3,  5, 11,  7,  5,  3,  2,  3, 11, -1, -1, -1, -1}, // 202  
	{ 2,  9,  8,  1,  9,  2,  2,  8,  7,  5, 11,  2,  2,  7,  5, -1}, // 203  
	{ 5,  1,  3,  5,  3,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 204  
	{ 7,  0,  8,  1,  0,  7,  5,  1,  7, -1, -1, -1, -1, -1, -1, -1}, // 205  
	{ 3,  9,  0,  5,  9,  3,  7,  5,  3, -1, -1, -1, -1, -1, -1, -1}, // 206  
	{ 7,  9,  8,  7,  5,  9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 207  
	{ 4,  5,  8,  8,  5, 11,  8, 11, 10, -1, -1, -1, -1, -1, -1, -1}, // 208  
	{ 4,  5,  0,  0,  5, 10, 10,  5, 11,  0, 10,  3, -1, -1, -1, -1}, // 209  
	{ 9,  0,  1, 11,  8,  4, 10,  8, 11,  5, 11,  4, -1, -1, -1, -1}, // 210  
	{ 4, 11, 10,  5, 11,  4,  4, 10,  3,  1,  9,  4,  4,  3,  1, -1}, // 211  
	{ 1,  2,  5,  5,  2,  8,  8,  2, 10,  8,  4,  5, -1, -1, -1, -1}, // 212  
	{10,  0,  4,  3,  0, 10, 10,  4,  5,  1,  2, 10, 10,  5,  1, -1}, // 213  
	{ 5,  0,  2,  9,  0,  5,  5,  2, 10,  8,  4,  5,  5, 10,  8, -1}, // 214  
	{ 5,  9,  4,  3,  2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 215  
	{11,  2,  5,  2,  3,  5,  5,  3,  4,  4,  3,  8, -1, -1, -1, -1}, // 216  
	{ 2,  5, 11,  4,  5,  2,  0,  4,  2, -1, -1, -1, -1, -1, -1, -1}, // 217  
	{ 2,  3, 11, 11,  3,  5,  5,  3,  8,  8,  4,  5,  9,  0,  1, -1}, // 218  
	{ 2,  5, 11,  4,  5,  2,  2,  1,  9,  2,  9,  4, -1, -1, -1, -1}, // 219  
	{ 5,  8,  4,  3,  8,  5,  1,  3,  5, -1, -1, -1, -1, -1, -1, -1}, // 220  
	{ 5,  0,  4,  5,  1,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 221  
	{ 5,  8,  4,  3,  8,  5,  5,  9,  0,  5,  0,  3, -1, -1, -1, -1}, // 222  
	{ 5,  9,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 223  
	{ 7,  4, 10, 10,  4,  9, 10,  9, 11, -1, -1, -1, -1, -1, -1, -1}, // 224  
	{ 3,  0,  8,  7,  4,  9,  7,  9, 10, 10,  9, 11, -1, -1, -1, -1}, // 225  
	{10,  1, 11,  4,  1, 10,  0,  1,  4, 10,  7,  4, -1, -1, -1, -1}, // 226  
	{ 4,  3,  1,  8,  3,  4,  4,  1, 11, 10,  7,  4,  4, 11, 10, -1}, // 227  
	{ 7,  4, 10,  4,  9, 10, 10,  9,  2,  2,  9,  1, -1, -1, -1, -1}, // 228  
	{ 4,  9,  7,  7,  9, 10, 10,  9,  1,  1,  2, 10,  3,  0,  8, -1}, // 229  
	{ 4, 10,  7,  2, 10,  4,  0,  2,  4, -1, -1, -1, -1, -1, -1, -1}, // 230  
	{ 4, 10,  7,  2, 10,  4,  4,  8,  3,  4,  3,  2, -1, -1, -1, -1}, // 231  
	{11,  2,  9,  9,  2,  7,  7,  2,  3,  9,  7,  4, -1, -1, -1, -1}, // 232  
	{ 7,  9, 11,  4,  9,  7,  7, 11,  2,  0,  8,  7,  7,  2,  0, -1}, // 233  
	{11,  3,  7,  2,  3, 11, 11,  7,  4,  0,  1, 11, 11,  4,  0, -1}, // 234  
	{ 2,  1, 11,  4,  8,  7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 235  
	{ 1,  4,  9,  7,  4,  1,  3,  7,  1, -1, -1, -1, -1, -1, -1, -1}, // 236  
	{ 1,  4,  9,  7,  4,  1,  1,  0,  8,  1,  8,  7, -1, -1, -1, -1}, // 237  
	{ 3,  4,  0,  3,  7,  4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 238  
	{ 7,  4,  8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 239  
	{ 8,  9, 11,  8, 11, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 240  
	{ 9,  3,  0, 10,  3,  9, 11, 10,  9, -1, -1, -1, -1, -1, -1, -1}, // 241  
	{11,  0,  1,  8,  0, 11, 10,  8, 11, -1, -1, -1, -1, -1, -1, -1}, // 242  
	{11,  3,  1, 11, 10,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 243  
	{10,  1,  2,  9,  1, 10,  8,  9, 10, -1, -1, -1, -1, -1, -1, -1}, // 244  
	{ 9,  3,  0, 10,  3,  9,  9,  1,  2,  9,  2, 10, -1, -1, -1, -1}, // 245  
	{10,  0,  2, 10,  8,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 246  
	{10,  3,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 247  
	{ 8,  2,  3, 11,  2,  8,  9, 11,  8, -1, -1, -1, -1, -1, -1, -1}, // 248  
	{ 2,  9, 11,  2,  0,  9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 249  
	{ 8,  2,  3, 11,  2,  8,  8,  0,  1,  8,  1, 11, -1, -1, -1, -1}, // 250  
	{ 2,  1, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 251  
	{ 8,  1,  3,  8,  9,  1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 252  
	{ 1,  0,  9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 253  
	{ 8,  0,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}, // 254  
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}  // 255  
};

char MarchingCubes::m_CubeNeighbors[256];
bool MarchingCubes::tablesBuilt = false;

MarchingCubes::MarchingCubes ()
{	
	m_nGridSize = 0;
	m_nGridSizep1 = 0;
	
	m_nMaxOpenVoxels = 32;
	m_pOpenVoxels    = new int[m_nMaxOpenVoxels*3];
	
	m_nNumOpenVoxels    = 0;
	m_pfGridEnergy      = 0;
	m_pnGridPointStatus = 0;
	m_pnGridVoxelStatus = 0;
	
	m_nNumVertices = 0;
	m_nNumIndices  = 0;
	m_pVertices    = 0;
	m_pIndices     = 0;
	
	if(!tablesBuilt) {
		BuildTables();
		tablesBuilt = true;
	}
}

void MarchingCubes::setup(int resolution)
{
	m_fVoxelSize = 2.0/float(resolution);
	m_nGridSize   = resolution;
	m_nGridSizep1 = m_nGridSize+1;
	
	if(minRadius == 0 && maxRadius == 0) {
		setRadius(1. / resolution, 3. / resolution);
	}
	
	m_pfGridEnergy      = new float[(resolution+1)*(resolution+1)*(resolution+1)];
	m_pnGridPointStatus = new char [(resolution+1)*(resolution+1)*(resolution+1)];
	m_pnGridVoxelStatus = new char [ resolution*    resolution*    resolution];
	
	m_nMaxNumVertices = MAX_VERTICES;
	m_nMaxNumIndices  = MAX_INDICES;
	
	m_pVertices = new SVertex[m_nMaxNumVertices];
	for (int i=0; i<m_nMaxNumVertices; i++){
		for (int j=0; j<3; j++){
			m_pVertices[i].v[j] = 0;
			m_pVertices[i].n[j] = 0;
		}
	}
	
	m_pIndices = new unsigned short[m_nMaxNumIndices];
	for (int i=0; i<m_nMaxNumIndices; i++){
		m_pIndices[i] = 0;
	}
}

void MarchingCubes::setCenters(const vector<ofVec3f>& centers){
	m_Balls.clear();
	for(int i = 0; i < centers.size(); i++) {
		m_Balls.push_back(SBall());
		m_Balls.back().p = centers[i] * 2 - 1;
		m_Balls.back().m = 1;
	}
}

void MarchingCubes::setMasses(const vector<float>& masses) {
	for(int i = 0; i < masses.size(); i++) {
		m_Balls[i].m = masses[i];
	}
}

void MarchingCubes::setRadius(float minRadius, float maxRadius) {
	this->minRadius = ofClamp(minRadius, 0, 1);
	this->maxRadius = ofClamp(maxRadius, minRadius, 1);
}

void MarchingCubes::update()
{
	mesh.clear();
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);

	int nCase,x,y,z;
	bool bComputed;
	
	m_nNumIndices  = 0;
	m_nNumVertices = 0;
	
	// Clear status grids
	memset(m_pnGridPointStatus, 0, (m_nGridSizep1)*(m_nGridSizep1)*(m_nGridSizep1));
	memset(m_pnGridVoxelStatus, 0, (m_nGridSize  )*(m_nGridSize  )*(m_nGridSize  ));
	
	for( int i = 0; i < m_Balls.size(); i++ ){
		if (m_Balls[i].m > 0){
			
			x = ConvertWorldCoordinateToGridPoint(m_Balls[i].p[0]);
			y = ConvertWorldCoordinateToGridPoint(m_Balls[i].p[1]);
			z = ConvertWorldCoordinateToGridPoint(m_Balls[i].p[2]);
			
			// Work our way out from the center of the ball until the surface is
			// reached. If the voxel at the surface is already computed then this
			// ball share surface with a previous ball.
			bComputed = false;
			while(1){
				if( IsGridVoxelComputed(x,y,z) ){
					bComputed = true;
					break;
				}
				
				nCase = ComputeGridVoxel(x,y,z);
				if( nCase < 255 )
					break;
				
				z--;
			}
			
			if( bComputed )
				continue;
			
			// Compute all voxels on the surface by computing neighbouring voxels
			// if the surface goes into them.
			AddNeighborsToList(nCase,x,y,z);
			
			while( m_nNumOpenVoxels ){
				m_nNumOpenVoxels--;
				x = m_pOpenVoxels[m_nNumOpenVoxels*3    ];
				y = m_pOpenVoxels[m_nNumOpenVoxels*3 + 1];
				z = m_pOpenVoxels[m_nNumOpenVoxels*3 + 2];
				
				nCase = ComputeGridVoxel(x,y,z);
				AddNeighborsToList(nCase,x,y,z);
			}
		}
	}		
}

void MarchingCubes::AddNeighborsToList(int nCase, int x, int y, int z)
{
	if( MarchingCubes::m_CubeNeighbors[nCase] & (1<<0) )
		AddNeighbor(x+1, y, z);
	
	if( MarchingCubes::m_CubeNeighbors[nCase] & (1<<1) )
		AddNeighbor(x-1, y, z);
	
	if( MarchingCubes::m_CubeNeighbors[nCase] & (1<<2) )
		AddNeighbor(x, y+1, z);
	
	if( MarchingCubes::m_CubeNeighbors[nCase] & (1<<3) )
		AddNeighbor(x, y-1, z);
	
	if( MarchingCubes::m_CubeNeighbors[nCase] & (1<<4) )
		AddNeighbor(x, y, z+1);
	
	if( MarchingCubes::m_CubeNeighbors[nCase] & (1<<5) )
		AddNeighbor(x, y, z-1);
}

void MarchingCubes::AddNeighbor(int x, int y, int z)
{
	if( IsGridVoxelComputed(x,y,z) || IsGridVoxelInList(x,y,z) )
		return;
	
	// Make sure the array is large enough
	if( m_nMaxOpenVoxels == m_nNumOpenVoxels ){
		m_nMaxOpenVoxels *= 2;
		int *pTmp = new int[m_nMaxOpenVoxels*3];
		memcpy(pTmp, m_pOpenVoxels, m_nNumOpenVoxels*3*sizeof(int));
		delete[] m_pOpenVoxels;
		m_pOpenVoxels = pTmp;
	}
	
	int n3 = m_nNumOpenVoxels*3;
	m_pOpenVoxels[n3  ] = x;
	m_pOpenVoxels[n3+1] = y;
	m_pOpenVoxels[n3+2] = z;
	
	SetGridVoxelInList(x,y,z);
	m_nNumOpenVoxels++;
}

// modified from ken perlin http://www.geisswerks.com/ryan/BLOBS/blobs.html
inline float getEnergy(const ofVec3f& vertex, const ofVec3f& center, float min, float max) {
	float r = vertex.distance(center) / 2; // / 2 because the space is double size. but this isn't quite right
	if(r >= max) {
		return 0;
	}
	if(r <= min) {
		return 1;
	}
	r = (r - min) / (max - min);
	return 1 - (r * r * r * (r * (r * 6 - 15) + 10));
}

float MarchingCubes::ComputeEnergy(float x, float y, float z)
{
	float fEnergy = 0;
	float fSqDist;
	float dx,dy,dz;
	
	ofVec3f vertex(x, y, z);
	
	for( int i = 0; i < m_Balls.size(); i++ ){	
		ofVec3f& center = m_Balls[i].p;
		float& mass = m_Balls[i].m;
		float cur = getEnergy(vertex, center, minRadius * mass, maxRadius * mass);
		fEnergy += cur;
	}
	
	return fEnergy;
}

void MarchingCubes::ComputeNormal(SVertex *pVertex)
{
	float fSqDist;
	float K;
	
	for( int i = 0; i < m_Balls.size(); i ++ ){
		// To compute the normal we derive the energy formula and get
		//   n += 2 * mass * vector / distance^4
		ofVec3f& p = m_Balls[i].p;
		ofVec3f& v = pVertex->v;
		float x = v[0] - p[0];
		float y = v[1] - p[1];
		float z = v[2] - p[2];
		
		fSqDist = x*x + y*y + z*z;
		K = 2.0 * m_Balls[i].m / (fSqDist * fSqDist);
		pVertex->n[0] += K * x;
		pVertex->n[1] += K * y;
		pVertex->n[2] += K * z;
	}
	
	float vx = pVertex->n[0];
	float vy = pVertex->n[1];
	float vz = pVertex->n[2];
	float vh = sqrt(vx*vx + vy*vy + vz*vz);
	if (vh > 0){
		pVertex->n[0] /= vh;
		pVertex->n[1] /= vh;
		pVertex->n[2] /= vh;
	}
		
}

inline float MarchingCubes::ComputeGridPointEnergy(int x, int y, int z)
{
	if( IsGridPointComputed(x,y,z) )
		return m_pfGridEnergy[x +
		                      y*(m_nGridSizep1) +
		                      z*(m_nGridSizep1)*(m_nGridSizep1)];
	
	// The energy on the edges are always zero to make sure the isosurface is
	// always closed.
	if( x <= 1 || y <= 1 || z <= 1 ||
		 x >= m_nGridSize || y >= m_nGridSize || z >= m_nGridSize ){
		m_pfGridEnergy[x +
		               y*(m_nGridSizep1) +
		               z*(m_nGridSizep1)*(m_nGridSizep1)] = 0;
		SetGridPointComputed(x,y,z);
		return 0;
	}
	
	float fx = ConvertGridPointToWorldCoordinate(x);
	float fy = ConvertGridPointToWorldCoordinate(y);
	float fz = ConvertGridPointToWorldCoordinate(z);
	
	float out = m_pfGridEnergy[x +
														 y*(m_nGridSizep1) +
														 z*(m_nGridSizep1)*(m_nGridSizep1)] = ComputeEnergy(fx,fy,fz);
	
	SetGridPointComputed(x,y,z);
	return out;
}

int MarchingCubes::ComputeGridVoxel(int x, int y, int z)
{
	float b[8];
	
	b[0] = ComputeGridPointEnergy(x  , y  , z  );
	b[1] = ComputeGridPointEnergy(x+1, y  , z  );
	b[2] = ComputeGridPointEnergy(x+1, y  , z+1);
	b[3] = ComputeGridPointEnergy(x  , y  , z+1);
	b[4] = ComputeGridPointEnergy(x  , y+1, z  );
	b[5] = ComputeGridPointEnergy(x+1, y+1, z  );
	b[6] = ComputeGridPointEnergy(x+1, y+1, z+1);
	b[7] = ComputeGridPointEnergy(x  , y+1, z+1);
	
	float fx = ConvertGridPointToWorldCoordinate(x) + m_fVoxelSize/2;
	float fy = ConvertGridPointToWorldCoordinate(y) + m_fVoxelSize/2;
	float fz = ConvertGridPointToWorldCoordinate(z) + m_fVoxelSize/2;
	
	int c = 0;
	c |= b[0] > fLevel ?   1 : 0;
	c |= b[1] > fLevel ?   2 : 0;
	c |= b[2] > fLevel ?   4 : 0;
	c |= b[3] > fLevel ?   8 : 0;
	c |= b[4] > fLevel ?  16 : 0;
	c |= b[5] > fLevel ?  32 : 0;
	c |= b[6] > fLevel ?  64 : 0;
	c |= b[7] > fLevel ? 128 : 0;
	
	// Compute vertices from marching pyramid case
	fx = ConvertGridPointToWorldCoordinate(x);
	fy = ConvertGridPointToWorldCoordinate(y);
	fz = ConvertGridPointToWorldCoordinate(z);
	
	int i = 0;
	float t, omt;
	unsigned short EdgeIndices[12];
	memset(EdgeIndices, 0xFF, 12*sizeof(unsigned short));
	while(1){
		int nEdge =	MarchingCubes::m_CubeTriangles[c][i];
		if( nEdge == -1 )
			break;
		
		if( EdgeIndices[nEdge] == 0xFFFF )
		{
			EdgeIndices[nEdge] = m_nNumVertices;
			if (m_nNumVertices < m_nMaxNumVertices){
				
				// Optimization: It's possible that the non-interior edges
				// have been computed already in neighbouring voxels
				
				// Compute the vertex by interpolating between the two points
				int nIndex0 = MarchingCubes::m_CubeEdges[nEdge][0];
				int nIndex1 = MarchingCubes::m_CubeEdges[nEdge][1];
				
				float *mcvn0 = MarchingCubes::m_CubeVertices[nIndex0];
				float *mcvn1 = MarchingCubes::m_CubeVertices[nIndex1];
				
				t = (fLevel - b[nIndex0])/(b[nIndex1] - b[nIndex0]);
				omt = 1.0f - t;
				
				m_pVertices[m_nNumVertices].v[0] = mcvn0[0]*omt + mcvn1[0]*t;
				m_pVertices[m_nNumVertices].v[1] = mcvn0[1]*omt + mcvn1[1]*t;
				m_pVertices[m_nNumVertices].v[2] = mcvn0[2]*omt + mcvn1[2]*t;
				
				m_pVertices[m_nNumVertices].v[0] = fx +
				m_pVertices[m_nNumVertices].v[0]*m_fVoxelSize;
				m_pVertices[m_nNumVertices].v[1] = fy +
				m_pVertices[m_nNumVertices].v[1]*m_fVoxelSize;
				m_pVertices[m_nNumVertices].v[2] = fz +
				m_pVertices[m_nNumVertices].v[2]*m_fVoxelSize;
				
				// Compute the normal at the vertex
				ComputeNormal(&m_pVertices[m_nNumVertices]);
				
				m_nNumVertices++;
			}
		}
		
		// Add the edge's vertex index to the index list
		if (m_nNumIndices < MAX_INDICES){
			m_pIndices[m_nNumIndices] = EdgeIndices[nEdge];
			m_nNumIndices++;
		}
		i++;
	}
	
	SetGridVoxelComputed (x,y,z);
	
	for (int i=0; i<m_nNumIndices; i++){
		SVertex V = m_pVertices[m_pIndices[i]];
		mesh.addNormal(V.n);
		mesh.addVertex((V.v + 1) / 2);
	}
	
	m_nNumVertices = 0;
	m_nNumIndices = 0;
	
	return c;
}




inline float MarchingCubes::ConvertGridPointToWorldCoordinate(int x)
{
	return float(x)*m_fVoxelSize - 1.0f;
}

int MarchingCubes::ConvertWorldCoordinateToGridPoint(float x)
{
	return int((x + 1.0f)/m_fVoxelSize + 0.5f);
}


inline bool MarchingCubes::IsGridPointComputed(int x, int y, int z)
{
	if( m_pnGridPointStatus[x +
	                        y*(m_nGridSize+1) +
	                        z*(m_nGridSize+1)*(m_nGridSize+1)] == 1 )
		return true;
	else
		return false;
}

inline bool MarchingCubes::IsGridVoxelComputed(int x, int y, int z)
{
	if( m_pnGridVoxelStatus[x +
	                        y*m_nGridSize +
	                        z*m_nGridSize*m_nGridSize] == 1 )
		return true;
	else
		return false;
}

inline bool MarchingCubes::IsGridVoxelInList(int x, int y, int z)
{
	if( m_pnGridVoxelStatus[x +
	                        y*m_nGridSize +
	                        z*m_nGridSize*m_nGridSize] == 2 )
		return true;
	else
		return false;
}

inline void MarchingCubes::SetGridPointComputed(int x, int y, int z)
{
	m_pnGridPointStatus[x +
	                    y*(m_nGridSizep1) +
	                    z*(m_nGridSizep1)*(m_nGridSizep1)] = 1;
}

inline void MarchingCubes::SetGridVoxelComputed(int x, int y, int z)
{
	m_pnGridVoxelStatus[x +
	                    y*m_nGridSize +
	                    z*m_nGridSize*m_nGridSize] = 1;
}

inline void MarchingCubes::SetGridVoxelInList(int x, int y, int z)
{
	m_pnGridVoxelStatus[x +
	                    y*m_nGridSize +
	                    z*m_nGridSize*m_nGridSize] = 2;
}

void MarchingCubes::draw() {
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

const ofMesh& MarchingCubes::getMesh() const {
	return mesh;
}
