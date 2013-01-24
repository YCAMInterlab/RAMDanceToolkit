
#pragma once
#include <algorithm>
#include <cfloat>
#include <cmath>
#include <map>
//
#include <numeric>
#include <set>


//namespace gl
//{
//	//ax + by + cz + d = 0;
//	static const float kGroundPlaneXUp[] = { 1.0, 0.0, 0.0, 1.0 };
//	static const float kGroundPlaneYUp[] = { 0.0, 1.0, 0.0, 1.0 };
//	static const float kGroundPlaneZUp[] = { 0.0, 0.0, 1.0, 1.0 };
//	
//	inline void calcShadowMatrix(const float groundplane[4],
//								 const float lightpos[3],
//								 float shadowMatrix[16]);
//	
//	
//	void calcShadowMatrix(const float groundplane[],
//						  const float lightpos[],
//						  float shadowMatrix[])
//	{
//		float dot = inner_product(groundplane, groundplane+4, lightpos, 0.f);
//		
//		for(int y = 0; y < 4;++y) {
//			for(int x = 0; x < 4; ++x) {
//				
//				shadowMatrix[y*4+x] = - groundplane[y]*lightpos[x];
//				
//				if (x == y) shadowMatrix[y*4+x] += dot;
//			}
//		}
//	}
//}


// ポインタの参照先を比較するためのファンクタ
class PtrComp
{
	public:
	template <class Type>
	bool operator()(const Type* a, const Type* b) const
	{
		return *a < *b;
	}
};


struct Vector
		{
	double x, y, z;
			float speed;
			ofColor color;
	// ======================================
	// 等価性の判定
	// ======================================
	bool operator==(const Vector& v) const
	{
		return x == v.x && y == v.y && z == v.z;
	}
	
	// ======================================
	// 大小判定（set / mapを構築する際に使用）
	// ======================================
	bool operator<(const Vector& v) const
	{
		return x != v.x ? x < v.x :
		y != v.y ? y < v.y :
		z < v.z;
	}
};


struct Circle
{
	Vector center;  // 中心座標
	double radius;  // 半径
};


class Triangle {
public:
	const static size_t NUM_VERTEX = 3;
	const Vector* p[NUM_VERTEX];  // 頂点座標
	
	// ======================================
	// 等価性の判定
	// ======================================
	bool operator==(const Triangle& t) const
	{
		const Vector* p1[NUM_VERTEX], *p2[NUM_VERTEX];
		for(int i = 0; i < NUM_VERTEX; ++i)
		{ // const Vector const * → const Vector *
			p1[i] = const_cast<const Vector*>(p[i]);
			p2[i] = const_cast<const Vector*>(t.p[i]);
		}
		std::sort(p1, p1 + NUM_VERTEX, PtrComp());
		std::sort(p2, p2 + NUM_VERTEX, PtrComp());
		
		for(int i = 0; i < NUM_VERTEX; ++i)
			if(!(p1[i] == p2[i])) return false;
		return true;
	}
	
	// ======================================
	// 大小判定（set / mapを構築する際に使用）
	// ======================================
	bool operator<(const Triangle& t) const
	{
		const Vector* p1[NUM_VERTEX], *p2[NUM_VERTEX];
		for(int i = 0; i < NUM_VERTEX; ++i)
		{ // const Vector const * → const Vector *
			p1[i] = const_cast<const Vector*>(p[i]);
			p2[i] = const_cast<const Vector*>(t.p[i]);
		}
		std::sort(p1, p1 + NUM_VERTEX, PtrComp());
		std::sort(p2, p2 + NUM_VERTEX, PtrComp());
		for(int i = 0; i < NUM_VERTEX - 1; ++i)
			if(!(*p1[i] == *p2[i])) return *p1[i] < *p2[i];
		return *p1[NUM_VERTEX - 1] < *p2[NUM_VERTEX - 1];
	}
};


class Tetrahedron {
public:
	const static size_t NUM_VERTEX = 4;
	const Vector* p[NUM_VERTEX];
	
	// ======================================
	// 等価性の判定
	// ======================================
	bool operator==(const Tetrahedron& t) const
	{
		const Vector* p1[NUM_VERTEX], *p2[NUM_VERTEX];
		for(int i = 0; i < NUM_VERTEX; ++i)
		{ // const Vector const * → const Vector *
			p1[i] = const_cast<const Vector*>(p[i]);
			p2[i] = const_cast<const Vector*>(t.p[i]);
		}
		std::sort(p1, p1 + NUM_VERTEX, PtrComp());
		std::sort(p2, p2 + NUM_VERTEX, PtrComp());
		for(int i = 0; i < NUM_VERTEX; ++i)
			if(!(p1[i] == p2[i])) return false;
		return true;
	}
	
	// ======================================
	// 大小判定（set / mapを構築する際に使用）
	// ======================================
	bool operator<(const Tetrahedron& t) const
	{
		const Vector* p1[NUM_VERTEX], *p2[NUM_VERTEX];
		for(int i = 0; i < NUM_VERTEX; ++i)
		{ // const Vector const * → const Vector *
			p1[i] = const_cast<const Vector*>( p[i] );
			p2[i] = const_cast<const Vector*>(t.p[i]);
		}
		std::sort(p1, p1 + NUM_VERTEX, PtrComp());
		std::sort(p2, p2 + NUM_VERTEX, PtrComp());
		
		for(int i = 0; i < NUM_VERTEX - 1; ++i)
			if(!(*p1[i] == *p2[i])) return *p1[i] < *p2[i];
		return *p1[NUM_VERTEX - 1] < *p2[NUM_VERTEX - 1];
	}
	
	// ======================================
	// 他の四面体と共有点を持つか
	// ======================================
	bool hasCommonPoints(const Tetrahedron& t) const
	{
		for(int i = 0; i < NUM_VERTEX; ++i)
			for(int j = 0; j < NUM_VERTEX; ++j)
				if(*p[i] == *t.p[j]) return true;
		return false;
	}
};

class Delaunay3d
{
	// ======================================
	// 型定義
	// ======================================
	typedef const std::set<Vector>         ConstVertexSet;
	typedef ConstVertexSet::const_iterator ConstVertexIter;
	
	typedef std::set<Triangle>             TriangleSet;
	
	typedef std::set<Tetrahedron>          TetraSet;
	typedef TetraSet::iterator             TetraIter;
	
	typedef std::map<Tetrahedron, bool>    TetraMap;
	
	
private:
	// ======================================
	// LU分解による三元一次連立方程式の解法
	// ======================================
	static double lu(double a[3][3], int ip[3])
	{
		const int n = 3;
		double weight[n];
		
		for(int k = 0; k < n; ++k)
		{
			ip[k] = k;
			double u = 0;
			for(int j = 0; j < n; ++j)
			{
				double t = fabs(a[k][j]);
				if (t > u) u = t;
			}
			if (u == 0) return 0;
			weight[k] = 1/u;
		}
		double det = 1;
		for(int k = 0; k < n; ++k)
		{
			double u = -1;
			int m = 0;
			for(int i = k; i < n; ++i)
			{
				int ii = ip[i];
				double t = fabs(a[ii][k]) * weight[ii];
				if(t>u)
				{
					u = t;
					m = i;
				}
			}
			int ik = ip[m];
			if (m != k)
			{
				ip[m] = ip[k]; ip[k] = ik;
				det = -det;
			}
			u = a[ik][k]; det *= u;
			if (u == 0) return 0;
			for (int i = k+1; i < n; ++i)
			{
				int ii = ip[i];
				double t = (a[ii][k] /= u);
				for(int j = k+1; j < n; ++j)
					a[ii][j] -= t * a[ik][j];
			}
		}
		return det;
	}
	
	static void solve(double a[3][3], double b[3], int ip[3], double x[3])
	{
		const int n = 3;
		
		for(int i = 0; i < n; ++i)
		{
			int ii = ip[i];
			double t = b[ii];
			for (int j = 0; j < i; ++j)
				t -= a[ii][j] * x[j];
			x[i] = t;
		}
		
		for (int i = n-1; i >= 0; --i)
		{
			double t = x[i];
			int ii = ip[i];
			for(int j = i+1; j < n; ++j)
				t -= a[ii][j] * x[j];
			x[i] = t / a[ii][i];
		}
	}
	
	static double gauss(double a[3][3], double b[3], double x[3])
	{
		const int n = 3;
		int ip[n];
		double det = lu(a, ip);
		
		if(det != 0) solve(a, b, ip, x);
		return det;
	}
	
	// ======================================
	// 与えられた四面体の外接球を求める
	// ======================================
	static void getCircumcircle(const Tetrahedron& tetra, Circle *dst)
	{
		const Vector* p0 = tetra.p[0];
		const Vector* p1 = tetra.p[1];
		const Vector* p2 = tetra.p[2];
		const Vector* p3 = tetra.p[3];
		
		double A[3][3] = {
			{p1->x - p0->x, p1->y - p0->y, p1->z - p0->z},
			{p2->x - p0->x, p2->y - p0->y, p2->z - p0->z},
			{p3->x - p0->x, p3->y - p0->y, p3->z - p0->z}
		};
		
		double b[3] = {
			0.5 * (p1->x*p1->x - p0->x*p0->x + p1->y*p1->y - p0->y*p0->y + p1->z*p1->z - p0->z*p0->z),
			0.5 * (p2->x*p2->x - p0->x*p0->x + p2->y*p2->y - p0->y*p0->y + p2->z*p2->z - p0->z*p0->z),
			0.5 * (p3->x*p3->x - p0->x*p0->x + p3->y*p3->y - p0->y*p0->y + p3->z*p3->z - p0->z*p0->z)
		};
		
		double x[3] = {0.0, 0.0, 0.0};
		
		if(gauss(A, b, x) == 0)
		{
			dst->center.x = dst->center.y = dst->center.z = 0;
			dst->radius   = -1;
		}
		else
		{
			dst->center.x = x[0];
			dst->center.y = x[1];
			dst->center.z = x[2];
			
			double dx = x[0] - p0->x;
			double dy = x[1] - p0->y;
			double dz = x[2] - p0->z;
			dst->radius =  sqrt(dx * dx + dy * dy + dz * dz);
		}
	}
	
	// ======================================
	// 四面体の重複管理
	// ======================================
	static inline void addElementToRedundanciesMap(TetraMap* pRddcMap,
												   const Tetrahedron& t)
	{
		TetraMap::iterator it = pRddcMap->find(t);
		if(it != pRddcMap->end() && it->second)
		{
			// 値を (t, true) から (t, false) に置換
			pRddcMap->erase(it);
			pRddcMap->insert(TetraMap::value_type(t, false));
		}
		else
			pRddcMap->insert(TetraMap::value_type(t, true));
	}
	
public:
	static void getDelaunayTriangles(vector<Vector>& pVertexSet,
									 TriangleSet* triangleSet)
	{
		
		TetraSet tetraSet;
		
		// ======================================
		// 巨大な外部四面体を作る
		// ======================================
		Vector p0, p1, p2, p3;
		Tetrahedron hugeTetrahedron = {&p0, &p1, &p2, &p3};
		//               !! 注意 !!
		// --------------------------------------
		// hugeTetrahedronの要素はローカル変数への
		// ポインタなので、これらの頂点を共有して
		// いるTetrahedronオブジェクトは、この関数
		// を抜ける前に必ず除去しておく必要がある。
		{
			double maxX, maxY, maxZ;
			double minX, minY, minZ;
			maxX = maxY = maxZ = DBL_MIN;
			minX = minY = minZ = DBL_MAX;
			
			// まず、全ての頂点を包含する球を得る
			
			// 中心座標を求める
			for (int i=0; i<pVertexSet.size(); i++)
			{
				Vector& it = pVertexSet.at(i);
				if(maxX  < it.x) maxX = it.x;
				if(it.x < minX ) minX = it.x;
				
				if(maxY  < it.y) maxY = it.y;
				if(it.y < minY ) minY = it.y;
				
				if(maxZ  < it.z) maxZ = it.z;
				if(it.z < minZ ) minZ = it.z;

			}
			
//			for(ConstVertexIter it = pVertexSet.begin();
//				it != pVertexSet.end(); ++it)
//			{
//				if(maxX  < it->x) maxX = it->x;
//				if(it->x < minX ) minX = it->x;
//				
//				if(maxY  < it->y) maxY = it->y;
//				if(it->y < minY ) minY = it->y;
//				
//				if(maxZ  < it->z) maxZ = it->z;
//				if(it->z < minZ ) minZ = it->z;
//			}
			double centerX = 0.5 * (maxX - minX);
			double centerY = 0.5 * (maxY - minY);
			double centerZ = 0.5 * (maxZ - minZ);
			
			// 半径を求める
			double dx      = centerX - minX;
			double dy      = centerY - minY;
			double dz      = centerZ - minZ;
			double radius  = sqrt(dx * dx + dy * dy + dz * dz) + 0.1;
			
			// 4つの頂点
			p0.x = centerX;
			p0.y = centerY + 3.0 * radius;
			p0.z = centerZ;
			
			p1.x = centerX - 2.0 * sqrt(2.0) * radius;
			p1.y = centerY - radius;
			p1.z = centerZ;
			
			p2.x = centerX + sqrt(2.0) * radius;
			p2.y = centerY - radius;
			p2.z = centerZ + sqrt(6.0) * radius;
			
			p3.x = centerX + sqrt(2.0) * radius;
			p3.y = centerY - radius;
			p3.z = centerZ - sqrt(6.0) * radius;
		}
		
		tetraSet.insert(hugeTetrahedron);
		
		// --------------------------------------
		// 点を逐次添加し、反復的に四面体分割を行う
		// --------------------------------------
		

		for(int i=0; i<pVertexSet.size(); i++)
		{
			const Vector* p = &pVertexSet.at(i);
			
			// --------------------------------------
			// 追加候補の四面体を保持する一時マップ
			// --------------------------------------
			TetraMap rddcMap;
			
			// --------------------------------------
			// 現在の四面体セットから要素を一つずつ取り出して、
			// 与えられた点が各々の四面体の外接球の中に含まれるかどうか判定
			// --------------------------------------
			for(TetraIter tIter = tetraSet.begin();
				tIter != tetraSet.end();)
			{
				// 四面体セットから要素を取りだして…
				Tetrahedron t = *tIter;
				
				// 外接球を求める
				Circle c;
				getCircumcircle(t, &c);
				double dx  = c.center.x - p->x;
				double dy  = c.center.y - p->y;
				double dz  = c.center.z - p->z;
				double len = sqrt(dx * dx + dy * dy + dz * dz);
				
				if(len < c.radius)
				{
					Tetrahedron t0 = {p, t.p[0], t.p[1], t.p[2]};
					addElementToRedundanciesMap(&rddcMap, t0);
					
					Tetrahedron t1 = {p, t.p[0], t.p[2], t.p[3]};
					addElementToRedundanciesMap(&rddcMap, t1);
					
					Tetrahedron t2 = {p, t.p[0], t.p[3], t.p[1]};
					addElementToRedundanciesMap(&rddcMap, t2);
					
					Tetrahedron t3 = {p, t.p[1], t.p[2], t.p[3]};
					addElementToRedundanciesMap(&rddcMap, t3);
					
					tetraSet.erase(tIter++);
				} else ++tIter;
			}
			
			for(TetraMap::iterator iter = rddcMap.begin();
				iter != rddcMap.end(); ++iter)
			{
				if(iter->second) tetraSet.insert(iter->first);
			}
		}

		
		
//		for(ConstVertexIter vIter = pVertexSet.begin();
//			vIter != pVertexSet.end(); ++vIter)
//		{
//			const Vector* p = &*vIter;
//			
//			// --------------------------------------
//			// 追加候補の四面体を保持する一時マップ
//			// --------------------------------------
//			TetraMap rddcMap;
//			
//			// --------------------------------------
//			// 現在の四面体セットから要素を一つずつ取り出して、
//			// 与えられた点が各々の四面体の外接球の中に含まれるかどうか判定
//			// --------------------------------------
//			for(TetraIter tIter = tetraSet.begin();
//				tIter != tetraSet.end();)
//			{
//				// 四面体セットから要素を取りだして…
//				Tetrahedron t = *tIter;
//				
//				// 外接球を求める
//				Circle c;
//				getCircumcircle(t, &c);
//				double dx  = c.center.x - p->x;
//				double dy  = c.center.y - p->y;
//				double dz  = c.center.z - p->z;
//				double len = sqrt(dx * dx + dy * dy + dz * dz);
//				
//				if(len < c.radius)
//				{
//					Tetrahedron t0 = {p, t.p[0], t.p[1], t.p[2]};
//					addElementToRedundanciesMap(&rddcMap, t0);
//					
//					Tetrahedron t1 = {p, t.p[0], t.p[2], t.p[3]};
//					addElementToRedundanciesMap(&rddcMap, t1);
//					
//					Tetrahedron t2 = {p, t.p[0], t.p[3], t.p[1]};
//					addElementToRedundanciesMap(&rddcMap, t2);
//					
//					Tetrahedron t3 = {p, t.p[1], t.p[2], t.p[3]};
//					addElementToRedundanciesMap(&rddcMap, t3);
//					
//					tetraSet.erase(tIter++);
//				} else ++tIter;
//			}
//			
//			for(TetraMap::iterator iter = rddcMap.begin();
//				iter != rddcMap.end(); ++iter)
//			{
//				if(iter->second) tetraSet.insert(iter->first);
//			}
//		}
		
		// --------------------------------------
		// 最後に、外部三角形の頂点を削除
		// --------------------------------------
		for(TetraIter tIter = tetraSet.begin();
			tIter != tetraSet.end(); )
		{
			if(hugeTetrahedron.hasCommonPoints(*tIter))
				tetraSet.erase(tIter++);
			else ++tIter;
		}
		
		
		for(TetraIter tIter = tetraSet.begin();
			tIter != tetraSet.end(); ++tIter)
		{
			Tetrahedron tetra = *tIter;
			
			Triangle t0 = {tetra.p[0], tetra.p[1], tetra.p[2]};
			triangleSet->insert(t0);
			
			Triangle t1 = {tetra.p[0], tetra.p[2], tetra.p[3]};
			triangleSet->insert(t1);
			
			Triangle t2 = {tetra.p[0], tetra.p[3], tetra.p[1]};
			triangleSet->insert(t2);
			
			Triangle t3 = {tetra.p[1], tetra.p[2], tetra.p[3]};
			triangleSet->insert(t3);
		}
	}
};


